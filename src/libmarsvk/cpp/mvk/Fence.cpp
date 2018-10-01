#include "mvk/Fence.hpp"

#include <stdexcept>

#include "mvk/Device.hpp"
#include "mvk/FencePool.hpp"
#include "mvk/Util.hpp"

namespace mvk {
    Fence::~Fence() {
        auto dh = getDevice()->getHandle();

        vkDestroyFence(dh, _handle, nullptr);
    }

    Device * Fence::getDevice() const {
        return getFencePool()->getDevice();
    }

    bool Fence::isSignaled() const {
        switch (vkGetFenceStatus(getDevice()->getHandle(), _handle)) {
            case VK_SUCCESS:
                return true;
            case VK_NOT_READY:
                return false;
            case VK_ERROR_DEVICE_LOST:
                throw std::runtime_error("Device was lost!");
            default:
                throw std::runtime_error("Unknown error!");
        }
    }

    void Fence::reset() {
        Util::vkAssert(vkResetFences(getDevice()->getHandle(), 1, &_handle));
    }

    void Fence::waitFor() {
        vkWaitForFences(getDevice()->getHandle(), 1, &_handle, VK_TRUE, ~0);
    }

    void Fence::release() {
        getFencePool()->releaseFence(this);
    }
}
