#pragma once

#include "volk.h"

#include <string>
#include <utility>

namespace mvk {
    class Device;

    class ShaderModule {
    public:
        struct CreateInfo {
            int flags;
            std::string path;
        };

    private:
        VkShaderModule _handle;
        CreateInfo _info;
        Device * _device;

        ShaderModule(const ShaderModule&) = delete;
        ShaderModule& operator=(const ShaderModule&) = delete;
    
    public:
        ShaderModule() noexcept :
            _handle(VK_NULL_HANDLE),
            _device(nullptr) {}

        ShaderModule(Device * device, const CreateInfo& info);

        ShaderModule(ShaderModule&& from) noexcept: 
            _handle(std::exchange(from._handle, nullptr)),
            _info(std::move(from._info)),
            _device(std::move(from._device)) {}

        ~ShaderModule() noexcept;
        
        ShaderModule& operator=(ShaderModule&& from) noexcept;

        inline VkShaderModule getHandle() const noexcept {
            return _handle;
        }

        inline const CreateInfo& getInfo() const noexcept {
            return _info;
        }

        inline Device * getDevice() const noexcept {
            return _device;
        }
    };

    inline bool operator==(const ShaderModule::CreateInfo& lhs, const ShaderModule::CreateInfo& rhs) noexcept {
        return lhs.flags == rhs.flags && lhs.path == rhs.path;
    }
}