#pragma once

#include "volk.h"

#include <memory>
#include <set>
#include <utility>
#include <vector>

#include "mvk/DescriptorSet.hpp"

namespace mvk {
    class DescriptorSetLayout;
    class Device;

    //! A pool object that holds DescriptorSet resources and can allocate DescriptorSets.
    /*!
        A descriptor pool maintains a pool of descriptors, from which DescriptorSets are allocated. 
        DescriptorPools are externally synchronized, meaning that the application must not allocate 
        and/or free DescriptorSets from the same pool in multiple threads simultaneously.
    */
    class DescriptorPool {
    public:
        //! DescriptorPool construction parameters.
        struct CreateInfo {
            unsigned int flags;                             /*!< Bitmask specifying support operations of the CommandPool. */
            unsigned int maxSets;                           /*!< The maximum number of DescriptorSets that can be allocated from the DescriptorPool. */
            std::vector<VkDescriptorPoolSize> poolSizes;    /*!< The number of descriptors for each DescriptorType to allocate in the DescriptorPool. */
        };

    private:
        struct Pool {
            VkDescriptorPool _handle;
            int _index;
            int _allocatedSets;

            Pool(VkDescriptorPool handle, int index) :
                _handle(handle),
                _index(index),
                _allocatedSets(0) {}
        };

        CreateInfo _info;
        DescriptorSetLayout * _descriptorSetLayout;
        std::vector<std::unique_ptr<Pool>> _pools;
        std::set<std::unique_ptr<DescriptorSet>> _allocatedDescriptorSets;
    
        Pool * allocatePool(Device * device);

        DescriptorPool(const DescriptorPool&) = delete;
        DescriptorPool& operator= (const DescriptorPool&) = delete;

    public:
        //! Constructs an empty DescriptorPool object.
        DescriptorPool() noexcept:
            _descriptorSetLayout(nullptr) {}

        //! Constructs a DescriptorPool object.
        /*!
            \param info the construction parameters.
            \param the target layout of all allocated DescriptorSets allocated by this DescriptorPool.
        */
        DescriptorPool(const CreateInfo& info, DescriptorSetLayout * layout) noexcept:
            _info(info),
            _descriptorSetLayout(layout) {}
        
        //! Move-constructs the DescriptorPool.
        /*!
            \param from the other DescriptorPool.
        */
        DescriptorPool(DescriptorPool&& from) noexcept:
            _info(std::move(from._info)),
            _descriptorSetLayout(std::move(from._descriptorSetLayout)),
            _pools(std::move(from._pools)),
            _allocatedDescriptorSets(std::move(from._allocatedDescriptorSets)) {}

        //! Deletes the DescriptorPool and releases all Vulkan resources.
        ~DescriptorPool() noexcept;

        //! Move-assigns the DescriptorPool.
        /*!
            \param from the other DescriptorPool.
        */
        DescriptorPool& operator= (DescriptorPool&& from) noexcept;

        //! Explicitly releases a DescriptorSet.
        /*!
            DescriptorSets are implicitly released on object deconstruction so explicit calling of this function is not recommended.

            \param set is the pointer to the DescriptorSet to release.
        */
        void releaseDescriptorSet(DescriptorSet * set);

        //! Allocates a new DescriptorSet.
        /*!
            \return the DescriptorSet.
        */
        DescriptorSet * allocate();

        //! Retrieves the Device.
        /*!
            \return the Device.
        */
        Device * getDevice() const noexcept;

        //! Retrieves the DescriptorSetLayout.
        /*!
            \return the DescriptorSetLayout.
        */
        inline DescriptorSetLayout * getDescriptorSetLayout() const noexcept {
            return _descriptorSetLayout;
        }
    };
}
