#pragma once

#include <stdexcept>
#include <string>

namespace mvk {
    //! Layers that can be enabled for debugging.
    enum class InstanceLayer {
        API_DUMP,               /*!< Vulkan calls are dumped to log as they are executed. */
        ASSISTANT_LAYER,
        CORE_VALIDATION,        /*!< Vulkan function validation */
        DEVICE_SIMULATION,
        MONITOR,
        OBJECT_TRACKER,
        PARAMETER_VALIDATION,   /*!< Vulkan function parameter validation */
        SCREENSHOT,
        STANDARD_VALIDATION,    /*!< Full Vulkan function validation */
        VKTRACE
    };
}

namespace std {
    inline string to_string(mvk::InstanceLayer layer) {
        using namespace mvk;

        switch (layer) {
            case InstanceLayer::API_DUMP:
                return "VK_LAYER_LUNARG_api_dump";
            case InstanceLayer::ASSISTANT_LAYER:
                return "VK_LAYER_LUNARG_assistant_layer";
            case InstanceLayer::CORE_VALIDATION:
                return "VK_LAYER_LUNARG_core_validation";
            case InstanceLayer::DEVICE_SIMULATION:
                return "VK_LAYER_LUNARG_device_simulation";
            case InstanceLayer::MONITOR:
                return "VK_LAYER_LUNARG_monitor";
            case InstanceLayer::OBJECT_TRACKER:
                return "VK_LAYER_LUNARG_object_tracker";
            case InstanceLayer::PARAMETER_VALIDATION:
                return "VK_LAYER_LUNARG_parameter_validation";
            case InstanceLayer::SCREENSHOT:
                return "VK_LAYER_LUNARG_screenshot";
            case InstanceLayer::STANDARD_VALIDATION:
                return "VK_LAYER_LUNARG_standard_validation";
            case InstanceLayer::VKTRACE:
                return "VK_LAYER_LUNARG_vktrace";
            default:
                throw runtime_error("Unknown InstanceLayer!");
        }
    }
}
