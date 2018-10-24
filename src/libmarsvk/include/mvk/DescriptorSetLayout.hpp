#pragma once

#include "volk.h"

#include <memory>
#include <utility>
#include <vector>

#include "mvk/DescriptorPool.hpp"
#include "mvk/DescriptorSet.hpp"
#include "mvk/DescriptorType.hpp"
#include "mvk/ShaderStage.hpp"

namespace mvk {
    class DescriptorSetLayoutCache;
    class Device;

    //! The DescriptorSetLayout is a group of Samplers, Images, and Buffers that describe a DescriptorSet.
    class DescriptorSetLayout {
    public:
        //! The Binding description struct
        struct Binding {
            unsigned int binding;           /*!< The shader binding location. */ 
            DescriptorType descriptorType;  /*!< The type of descriptor. */
            unsigned int descriptorCount;   /*!< The amount of descriptors. Should be 1 unless the descriptor is an array. */
            ShaderStage stages;             /*!< Bitmask of the ShaderStages the descriptor is active. */
        };

        //! Construction parameters for a DescriptorSetLayout.
        /*!
            \param flags is a bitmask specifying additional options for DescriptorSetLayout.
            \param bindings is a list of all Bindings in the DescriptorSet.
        */
        struct CreateInfo {
            unsigned int flags;
            std::vector<Binding> bindings;
        };

    private:
        VkDescriptorSetLayout _handle;
        CreateInfo _info;
        DescriptorSetLayoutCache * _cache;
        std::unique_ptr<DescriptorPool> _pool;

        DescriptorSetLayout(const DescriptorSetLayout&) = delete;

        DescriptorSetLayout& operator= (const DescriptorSetLayout&) = delete;

    public:
        //! Constructs an empty DescriptorSetLayout.
        DescriptorSetLayout() noexcept:
            _handle(nullptr),
            _cache(nullptr) {}

        //! Constructs a DescriptorSetLayout
        /*!
            \param cache is the DescriptorSetLayoutCache to allocate the DescriptorSetLayout from.
            \param info is the parameters to use for construction of the DescriptorSetLayout.
        */
        DescriptorSetLayout(DescriptorSetLayoutCache * cache, const CreateInfo& info);

        //! Move-constructs a DescriptorSetLayout.
        /*!
            \param from the other DescriptorSetLayout.
        */
        DescriptorSetLayout(DescriptorSetLayout&& from) noexcept:
            _handle(std::exchange(from._handle, nullptr)),
            _info(std::move(from._info)),
            _cache(std::move(from._cache)),
            _pool(std::move(from._pool)) {}

        //! Deletes the DescriptorSetLayout and releases and held Vulkan resources.
        ~DescriptorSetLayout() noexcept;

        //! Move-assigns a DescriptorSetLayout.
        /*!
            \param from the other DescriptorSetLayout.
        */
        DescriptorSetLayout& operator= (DescriptorSetLayout&&) noexcept;

        //! Explicitly releases the DescriptorSetLayout.
        /*!
            DescriptorSetLayouts are automatically released when the deconstructor is called, 
            so manually calling this function is not recommended.
        */
        void release();

        //! Retrieves the Device.
        /*!
            \return the Device.
        */
        Device * getDevice() const noexcept;

        //! Retrieves the underlying Vulkan handle.
        /*!
            \return the Vulkan handle.
        */
        inline VkDescriptorSetLayout getHandle() const noexcept {
            return _handle;
        }

        //! Implicitly casts to the underlying Vulkan handle.
        inline operator VkDescriptorSetLayout() const noexcept {
            return _handle;
        }

        //! Retrieves the construction parameters for the DescriptorSetLayout.
        /*!
            \return a const reference to an immutable copy of the initial construction parameters.
        */
        inline const CreateInfo getInfo() const noexcept {
            return _info;
        }

        //! Retrieves the DescriptorSetLayoutCache this DescriptorSetLayout was allocated from.
        /*!
            \return the DescriptorSetLayoutCache.
        */
        inline DescriptorSetLayoutCache * getDescriptorSetLayoutCache() const noexcept {
            return _cache;
        }

        //! Retrieves the DescriptorPool assigned to this DescriptorSetLayout.
        /*!
            \return the DescriptorPool.
        */
        inline DescriptorPool * getDescriptorPool() const noexcept {
            return _pool.get();
        }

        //! Allocates a DescriptorSet from the assigned DescriptorPool.
        /*!
            \return the allocated DescriptorSet.
        */
        inline DescriptorSet * allocate() {
            return _pool->allocate();
        }
    };

    inline constexpr bool operator== (const DescriptorSetLayout::Binding& lhs, const DescriptorSetLayout::Binding& rhs) noexcept {
        return lhs.binding == rhs.binding 
            && lhs.descriptorType == rhs.descriptorType 
            && lhs.descriptorCount == rhs.descriptorCount 
            && lhs.stages == rhs.stages;
    }

    inline constexpr bool operator== (const DescriptorSetLayout::CreateInfo& lhs, const DescriptorSetLayout::CreateInfo& rhs) noexcept {
        return lhs.flags == rhs.flags 
                && lhs.bindings == rhs.bindings;
    }
}
