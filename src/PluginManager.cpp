#include "PluginManager.h"
#include <iostream>
#include <filesystem>

void PluginManager::LoadAll(const std::string& dirPath) {
    if (!std::filesystem::exists(dirPath)) {
        std::cerr << "[Warning] Plugins directory not found: " << dirPath << "\n";
        return;
    }

    for (const auto& entry : std::filesystem::directory_iterator(dirPath)) {
        if (entry.path().extension() == ".dll") {
            HMODULE hMod = LoadLibraryW(entry.path().wstring().c_str());
            if (!hMod) {
                std::cerr << "[Warning] Failed to load DLL: " << entry.path().filename() << "\n";
                continue;
            }

            std::unique_ptr<std::remove_pointer_t<HMODULE>, ModuleDeleter> handle(hMod);

            using GetDescFunc = const PluginDescriptor* (*)();
            auto getDesc = reinterpret_cast<GetDescFunc>(
                GetProcAddress(hMod, "GetPluginDescriptor")
                );

            if (!getDesc) {
                std::cerr << "[Warning] Not a valid plugin: " << entry.path().filename() << "\n";
                continue;
            }

            const PluginDescriptor* desc = nullptr;
            try {
                desc = getDesc();
            }
            catch (...) {
                std::cerr << "[Warning] Exception in plugin init: " << entry.path().filename() << "\n";
                continue;
            }

            if (!desc || !desc->name || !desc->func_ptr) {
                std::cerr << "[Warning] Invalid descriptor in: " << entry.path().filename() << "\n";
                continue;
            }

            if (m_plugins.count(desc->name)) {
                std::cerr << "[Warning] Duplicate function: " << desc->name << "\n";
                continue;
            }

            m_plugins.emplace(desc->name, LoadedPlugin{ std::move(handle), desc });
            std::cout << "Loaded plugin: " << desc->name << "\n";
        }
    }
}

const PluginDescriptor* PluginManager::FindFunction(const std::string& name) const {
    auto it = m_plugins.find(name);
    return (it != m_plugins.end()) ? it->second.desc : nullptr;
}