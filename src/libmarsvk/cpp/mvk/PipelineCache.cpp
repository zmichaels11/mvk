#include "mvk/PipelineCache.hpp"

#include "volk.h"

#include "mvk/Device.hpp"

namespace mvk {
    PipelineCache::PipelineCache(Device * device) {
        _device = device;

        //TODO: restore old PipelineCache (if possible)

        auto pipelineCacheCI = VkPipelineCacheCreateInfo {};
        pipelineCacheCI.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
        
        _handle = VK_NULL_HANDLE;

        auto result = vkCreatePipelineCache(device->getHandle(), &pipelineCacheCI, nullptr, &_handle);

        if (VK_SUCCESS != result) {
            if (VK_NULL_HANDLE != _handle) {
                vkDestroyPipelineCache(device->getHandle(), _handle, nullptr);
                _handle = VK_NULL_HANDLE;
            }
        }
    }

    PipelineCache::~PipelineCache() {
        if (VK_NULL_HANDLE == _handle) {
            return;
        }

        //TODO: serialize PipelineCache to file

        vkDestroyPipelineCache(_device->getHandle(), _handle, nullptr);
    }

    
}
