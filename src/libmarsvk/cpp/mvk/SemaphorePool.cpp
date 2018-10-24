#include "mvk/SemaphorePool.hpp"

#include "volk.h"

#include <memory>

#include "mvk/Device.hpp"
#include "mvk/Util.hpp"

namespace mvk {
    Semaphore * SemaphorePool::allocateSemaphore() {
        auto semaphoreCI = VkSemaphoreCreateInfo {};
        semaphoreCI.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        
        auto exportSemaphoreCI = VkExportSemaphoreCreateInfo {};
        exportSemaphoreCI.sType = VK_STRUCTURE_TYPE_EXPORT_SEMAPHORE_CREATE_INFO;

        {
            const auto& enabledExtensions = _device->getEnabledExtensions();
            
            if (enabledExtensions.end() != enabledExtensions.find("VK_KHR_external_semaphore_fd")) {                
                exportSemaphoreCI.handleTypes = VK_EXTERNAL_SEMAPHORE_HANDLE_TYPE_OPAQUE_FD_BIT;

                semaphoreCI.pNext = &exportSemaphoreCI;
            }
        }

        VkSemaphore handle = VK_NULL_HANDLE;

        Util::vkAssert(vkCreateSemaphore(getDevice()->getHandle(), &semaphoreCI, nullptr, &handle));

        auto ptr = std::make_unique<Semaphore> (this, handle);
        auto out = ptr.get();

        _allSemaphores.insert(std::move(ptr));

        return out;
    }

    Semaphore * SemaphorePool::acquireSemaphore() {
        if (_availableSemaphores.empty()) {
            return allocateSemaphore();
        } else {
            auto out = _availableSemaphores.front();

            _availableSemaphores.pop();

            return out;
        }
    }

    void SemaphorePool::releaseSemaphore(Semaphore * semaphore) noexcept {
        _availableSemaphores.push(semaphore);
    }
}
