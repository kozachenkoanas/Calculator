#pragma once
#include <windows.h>
#include <string>
#include <map>
#include <memory>
#include "plugin_api.h"

struct ModuleDeleter {
    void operator()(HMODULE h) const {
        if (h) FreeLibrary(h);
    }
};

class PluginManager {
public:
    void LoadAll(const std::string& dirPath);
    const PluginDescriptor* FindFunction(const std::string& name) const;

private:
    struct LoadedPlugin {
        std::unique_ptr<std::remove_pointer_t<HMODULE>, ModuleDeleter> handle;
        const PluginDescriptor* desc;
    };
    std::map<std::string, LoadedPlugin> m_plugins;
};