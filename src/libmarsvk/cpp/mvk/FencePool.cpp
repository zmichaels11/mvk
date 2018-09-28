#include "mvk/FencePool.hpp"

#include "volk.h"

#include "mvk/Device.hpp"
#include "mvk/Util.hpp"

namespace mvk {
    Fence * FencePool::acquireFence() {
        if (_availableFences.empty()) {
            return allocateFence();
        } else {
            auto out = _availableFences.front();

            _availableFences.pop();

            return out;
        }
    }

    void FencePool::releaseFence(Fence * fence) {
        _availableFences.push(fence);
    }

    Fence * FencePool::allocateFence() {
        VkFenceCreateInfo fenceCI {};
        fenceCI.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;

        VkFence handle = VK_NULL_HANDLE;

        Util::vkAssert(vkCreateFence(getDevice()->getHandle(), &fenceCI, nullptr, &handle));

        auto ptr = std::make_unique<Fence>(this, handle);
        auto out = ptr.get();

        _allFences.insert(std::move(ptr));

        return out;
    }
}
