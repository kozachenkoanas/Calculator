#define CALC_PLUGIN_BUILD
#include "plugin_api.h"
#include <cmath>

static double my_sin(double x, double /*unused*/) {
    constexpr double PI = 3.14159265358979323846;
    return std::sin(x * PI / 180.0);
}

static const PluginDescriptor desc = {
    "sin", 1, my_sin
};

PLUGIN_API const PluginDescriptor* GetPluginDescriptor() {
    return &desc;
}