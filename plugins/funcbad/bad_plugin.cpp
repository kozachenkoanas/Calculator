#include <iostream>

extern "C" __declspec(dllexport)
void someOtherFunction() {
    std::cout << "Я не плагин!" << std::endl;
}