#pragma once

#include "volk.h"

namespace mvk {
    class Device {
        VkDevice _handle;

    public:
        Device() : _handle(VK_NULL_HANDLE) {}

        Device(VkDevice handle);

        ~Device();

        Device(const Device&) = delete;

        Device(Device&&) = default;

        Device& operator=(const Device&) = delete;

        Device& operator=(Device&&) = default;
    };
}