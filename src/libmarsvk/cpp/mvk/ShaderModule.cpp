#include "mvk/ShaderModule.hpp"

#include <cstddef>
#include <cstdint>

#include <sys/mman.h>
#include <sys/stat.h>

#include <fcntl.h>
#include <unistd.h>

#include "volk.h"

#include <stdexcept>

#include "mvk/Device.hpp"
#include "mvk/Util.hpp"

namespace mvk {
    namespace {
        std::size_t getFileSize(const char * fileName) {
            struct stat st;

            stat(fileName, &st);

            return st.st_size;
        }
    }

    ShaderModule::ShaderModule(Device * device, const ShaderModule::CreateInfo& info) {
        _device = device;
        _info = info;

        auto fileName = info.path.c_str();
        int fd = open(fileName, O_RDONLY, 0);

        if (-1 == fd) {
            throw std::runtime_error("Unable to open file: " + info.path);
        }
        
        auto fileSize = getFileSize(fileName);

#ifdef __APPLE__
        auto pData = mmap(nullptr, fileSize, PROT_READ, MAP_PRIVATE, fd, 0);
#else
        auto pData = mmap(nullptr, fileSize, PROT_READ, MAP_PRIVATE | MAP_POPULATE, fd, 0);
#endif

        if (MAP_FAILED == pData) {
            throw std::runtime_error("Failed to map file: " + info.path);
        }

        VkShaderModuleCreateInfo shaderModuleCI {};

        shaderModuleCI.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        shaderModuleCI.flags = info.flags;
        shaderModuleCI.pCode = static_cast<std::uint32_t * >(pData);
        shaderModuleCI.codeSize = fileSize;

        Util::vkAssert(vkCreateShaderModule(device->getHandle(), &shaderModuleCI, nullptr, &_handle));

        munmap(pData, fileSize);
        close(fd);
    }

    ShaderModule::~ShaderModule() {
        if (VK_NULL_HANDLE != _handle) {
            vkDestroyShaderModule(_device->getHandle(), _handle, nullptr);
        }
    }
}