#pragma once

#include <memory>
#include <vector>

#include "mvk/DescriptorSetLayout.hpp"

namespace mvk {
    class Device;

    //! A cache of DescriptorSetLayouts
    class DescriptorSetLayoutCache {
        Device * _device;

        struct Layout {
            DescriptorSetLayout instance;
            int references;

            Layout(DescriptorSetLayoutCache * pCache, const DescriptorSetLayout::CreateInfo& createInfo) noexcept:
                instance(pCache, createInfo),
                references(0) {}
        };

        std::vector<std::unique_ptr<Layout>> _layouts;

    public:
        //! Constructs an empty DescriptorSetLayoutCache.
        DescriptorSetLayoutCache() noexcept:
            _device(nullptr) {}

        //! Constructs a DescriptorSetLayoutCache 
        /*!
            \param device is the Device that will own the DescriptorSetLayoutCache.
        */
        DescriptorSetLayoutCache(Device * device) noexcept:
            _device(device) {}
        
        //! Move-assigns the DescriptorSetLayoutCache.
        /*!
            \param from the other DescriptorSetLayoutCache.
        */
        DescriptorSetLayoutCache& operator= (DescriptorSetLayoutCache&& from) = default;

        //! Allocates a DescriptorSetLayout
        /*!
            \param createInfo the construction parameters for the DescriptorSetLayout.
            \return the allocated DescriptorSetLayout. A compatible previously allocated DescriptorSetLayout may be returned instead.
        */
        DescriptorSetLayout * allocateDescriptorSetLayout(const DescriptorSetLayout::CreateInfo& createInfo);

        //! Releases a DescriptorSetLayout
        /*!
            \param layout the DescriptorSetLayout to return to the DescriptorSetLayoutCache.
        */
        void releaseDescriptorSetLayout(DescriptorSetLayout * layout);

        //! Retrieves the Device.
        /*!
            \return the Device.
        */
        inline Device * getDevice() const noexcept {
            return _device;
        }
    };
}
