#define CALC_PLUGIN_BUILD
#include "plugin_api.h"
#include <cmath>
#include <stdexcept>

static double my_ln(double x, double) {
    if (x <= 0) {
        throw std::runtime_error("Аргумент логарифма должен быть больше нуля");
    }
    return std::log(x);
}

static const PluginDescriptor desc = { "ln", 1, my_ln };

PLUGIN_API const PluginDescriptor* GetPluginDescriptor() {
    return &desc;
}