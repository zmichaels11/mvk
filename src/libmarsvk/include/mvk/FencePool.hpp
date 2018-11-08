#pragma once

#include <memory>
#include <queue>
#include <set>
#include <utility>

#include "mvk/Fence.hpp"

namespace mvk {
    class Device;

    //! A FencePool is a pool object that allocates and releases Fence objects.
    class FencePool {
        Device * _device;
        std::set<std::unique_ptr<Fence>> _allFences;
        std::queue<Fence * > _availableFences;
        
        Fence * allocateFence();

        FencePool(const FencePool&) = delete;
        FencePool& operator=(const FencePool&) = delete;
    
    public:
        //! Constructs an empty FencePool.
        FencePool() noexcept:
            _device(nullptr) {}

        //! Constructs a new FencePool for the specified Device.
        /*!
            \param device is the Device that will own the FencePool.
        */
        FencePool(Device * device) noexcept:
            _device(device) {}

        //! Move-constructs the FencePool
        /*!
            \param from the other FencePool.
        */
        FencePool(FencePool&& from) noexcept:
            _device(std::move(from._device)),
            _allFences(std::move(from._allFences)),
            _availableFences(std::move(from._availableFences)) {}

        //! Move-assigns the FencePool.
        /*!
            \param from the other FencePool.
        */
        FencePool& operator=(FencePool&& from) = default;

        //! Retrieves the Device.
        /*!
            \return the Device.
        */
        inline Device * getDevice() const noexcept {
            return _device;
        }

        //! Returns a Fence to this FencePool.
        /*!
            Returns a Fence to this FencePool. Only Fence objects allocated from
            this FencePool are valid. Fence objects should have any signaling 
            cleared prior to being returned to the FencePool.
            
            \param fence is the Fence to return to the FencePool.
        */
        void releaseFence(Fence * fence) noexcept;

        //! Allocates a Fence from the FencePool.
        /*!
            This function will allocate a Fence if there are no available Fence objects
            to recycle. Otherwise it will reuse a Fence.
            
            \return the Fence.
        */
        Fence * acquireFence();
    };
}
