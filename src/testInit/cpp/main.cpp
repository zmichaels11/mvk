#include "mvk/Instance.hpp"

#include <iostream>

int main(int argc, char ** argv) {
    //mvk::Instance::enableLayer("VK_LAYER_LUNARG_standard_validation");
    //mvk::Instance::enableLayer("VK_LAYER_LUNARG_api_dump");
    
    auto& instance = mvk::Instance::getCurrent();
    auto physicalDevices = instance.getPhysicalDevices();

    for (auto& pd : physicalDevices) {
        std::cout << pd->toString() << std::endl;
    }
}