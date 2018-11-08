#pragma once

#include <cstddef>

#include <memory>
#include <utility>

#include "volk.h"

#include "mvk/Pipeline.hpp"
#include "mvk/PipelineLayout.hpp"
#include "mvk/PipelineShaderStageCreateInfo.hpp"

namespace mvk {
    class Device;
    class PipelineCache;
    class DescriptorSetLayout;

    class ComputePipeline;
    
    //! A unique-ptr wrapper for a ComputePipeline object.
    using UPtrComputePipeline = std::unique_ptr<ComputePipeline>;

    //! A Pipeline object for Compute operations.
    /*!
        ComputePipelines consist of a single static compute shader stage and the pipeline layout.
        
        See: <a href="https://vulkan.lunarg.com/doc/view/1.0.33.0/linux/vkspec.chunked/ch09s01.html">Compute Pipelines</a>
    */
    class ComputePipeline : public virtual Pipeline {
    public:
        //! ComputePipeline construction parameters.
        struct CreateInfo {
            unsigned int flags;                     /*!< Additional construction parameters. */
            PipelineShaderStageCreateInfo stage;    /*!< The stage describing the compute shader. */
            PipelineLayout::CreateInfo layoutInfo;  /*!< The PipelineLayout descriptor struct. */
        };

        //! Constructs a ComputePipeline-typed unique_ptr pointing to null.
        /*!
            \return a unique_ptr<ComputePipeline> pointing to nullptr.
        */
        static inline UPtrComputePipeline unique_null() {
            return std::unique_ptr<ComputePipeline>();
        }

    private:
        CreateInfo _info;
        VkPipeline _handle;
        PipelineCache * _cache;
        PipelineLayout * _layout;

        ComputePipeline(const ComputePipeline&) = delete;

        ComputePipeline& operator= (const ComputePipeline&) = delete;

    public:
        //! Constructs a ComputePipeline object holding nothing.
        ComputePipeline() :
            _handle(VK_NULL_HANDLE),
            _cache(nullptr),
            _layout(nullptr) {}

        //! Constructs a ComputePipeline object.
        /*!
            \param cache is the PipelineCache to allocate from.
            \param createInfo is the construction parameters.
        */
        ComputePipeline(PipelineCache * cache, const CreateInfo& createInfo);

        //! Move-constructs a ComputePipeline.
        /*!
            \param from the other ComputePipeline.
        */
        ComputePipeline(ComputePipeline&& from) noexcept:
            _info(std::move(from._info)),
            _handle(std::exchange(from._handle, nullptr)),
            _cache(std::move(from._cache)),
            _layout(std::move(from._layout)) {}

        //! Deletes the ComputePipeline and releases any resources.
        ~ComputePipeline() noexcept;

        //! Move-assigns a ComputePipeline.
        /*!
            \param from the other ComputePipeline.
        */
        ComputePipeline& operator= (ComputePipeline&& from) noexcept;

        //! Retrieves the construction parameters.
        /*!
            \return a const reference to an immutable copy of the construction parameters.
        */
        inline const CreateInfo& getInfo() const noexcept {
            return _info;
        }

        //! Retrieves the PipelineBindPoint
        /*!
            \return PipelineBindPoint::COMPUTE
        */
        virtual PipelineBindPoint getBindPoint() const noexcept;

        //! Explicitly releases the ComputePipleline resources back to the PipelineCache.
        /*!
            This method is called implicitly by the deconstructor if the Vulkan resources still exist on object deconstruction.
        */
        virtual void release();

        //! Retrieves the Device.
        /*!
            \return the Device.
        */
        virtual Device * getDevice() const noexcept;

        //! Retrieves the PipelineCache this ComputePipeline was allocated from.
        /*!
            \return the PipelineCache.
        */
        virtual PipelineCache * getPipelineCache() const noexcept;

        //! Retrieves the PipelineLayout describing inputs and outputs of this ComputePipeline.
        virtual PipelineLayout * getPipelineLayout() const noexcept;

        //! Retrieves a DescriptorSetLayout from this Pipeline.
        /*!
            \param index is the index of the DescriptorSetLayout if there is more than 1.
            \return the DescriptorSetLayout.
        */
        virtual DescriptorSetLayout * getDescriptorSetLayout(std::ptrdiff_t index) const noexcept;

        //! Retrieves a list of all DescriptorSetLayouts of this ComputePipeline in order.
        /*!
            \return vector of all DescriptorSetLayouts
        */
        virtual std::vector<DescriptorSetLayout * > getDescriptorSetLayouts() const noexcept;

        //! Retrieves the underlying Vulkan handle.
        /*!
            \return the handle
        */
        virtual VkPipeline getHandle() const noexcept;

        //! Implicitly casts to the underlying Vulkan handle.
        inline operator VkPipeline() const noexcept {
            return getHandle();
        }
    };
}
