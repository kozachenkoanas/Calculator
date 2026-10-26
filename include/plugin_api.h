// include/plugin_api.h
#pragma once

#ifdef _WIN32
#ifdef CALC_PLUGIN_BUILD
#define PLUGIN_API extern "C" __declspec(dllexport)
#else
#define PLUGIN_API extern "C" __declspec(dllimport)
#endif
#else
#define PLUGIN_API extern "C"
#endif

// Мы используем сигнатуру с двумя аргументами для универсальности.
// Если функция унарная (как sin), второй аргумент будет игнорироваться.
using MathFuncPtr = double(*)(double, double);

struct PluginDescriptor {
    const char* name;       // Имя функции: "sin"
    int arity;              // 1 для sin, 2 для pow
    MathFuncPtr func_ptr;   // Указатель на функцию
};

extern "C" {
    PLUGIN_API const PluginDescriptor* GetPluginDescriptor();
}