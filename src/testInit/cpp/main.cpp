#include "mvk/Instance.hpp"

#include <iostream>
#include <string>

int main(int argc, char ** argv) {
    mvk::Instance::enableLayer("VK_LAYER_LUNARG_standard_validation");
    //mvk::Instance::enableLayer("VK_LAYER_LUNARG_api_dump");
    
    auto& instance = mvk::Instance::getCurrent();
    const auto& physicalDevices = instance.getPhysicalDevices();

    for (auto& pd : physicalDevices) {
        std::cout << pd.toString() << std::endl;
    }

    auto device = instance.getPhysicalDevice(0).createDevice();

    std::cout << "QueueFamilies: " << device->getQueueFamilyCount() << std::endl;

    return 0;
}