#pragma once

#include "volk.h"

#include <algorithm>
#include <string>

#include "mvk/AspectFlag.hpp"
#include "mvk/Format.hpp"
#include "mvk/InstanceLayer.hpp"

namespace mvk {
    namespace Util {
        template<typename T>
        constexpr T clamp(T value, T min, T max) {
            return std::max(min, std::min(max, value));
        }

        constexpr VkDeviceSize alignUp(VkDeviceSize a, VkDeviceSize b) {
            return (a + b - 1) / b * b;
        }

        constexpr VkDeviceSize alignDown(VkDeviceSize a, VkDeviceSize b) {
            return a & (~b - 1);
        }

        std::string translateVulkanResult(VkResult result);

        std::string name(const InstanceLayer& layer);

        void vkAssert(VkResult result);

        inline constexpr AspectFlag aspect(Format format) {
            switch (format) {
                case Format::D16_UNORM_S8_UINT:
                case Format::D24_UNORM_S8_UINT:
                case Format::D32_SFLOAT_S8_UINT:
                    return AspectFlag::DEPTH | AspectFlag::STENCIL;
                default:
                    return AspectFlag::COLOR;
            }
        }
    }
}
