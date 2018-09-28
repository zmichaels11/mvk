#pragma once

#include "volk.h"

#include <string>

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
        const Device * _device;
    
    public:
        ShaderModule() :
            _handle(VK_NULL_HANDLE) {}

        ShaderModule(const Device * device, const CreateInfo& info);

        ShaderModule(const ShaderModule&) = delete;

        ShaderModule(ShaderModule&&) = default;

        ~ShaderModule();

        ShaderModule& operator=(const ShaderModule&) = delete;
        
        ShaderModule& operator=(ShaderModule&&) = default;

        inline VkShaderModule getHandle() const {
            return _handle;
        }

        inline const CreateInfo& getInfo() const {
            return _info;
        }
    };

    inline bool operator==(const ShaderModule::CreateInfo& lhs, const ShaderModule::CreateInfo& rhs) {
        return lhs.flags == rhs.flags && lhs.path == rhs.path;
    }
}