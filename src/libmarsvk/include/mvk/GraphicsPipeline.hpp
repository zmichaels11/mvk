#pragma once

#include <cstddef>

#include "volk.h"

#include <memory>
#include <utility>
#include <vector>

#include "mvk/Pipeline.hpp"
#include "mvk/PipelineColorBlendStateCreateInfo.hpp"
#include "mvk/PipelineDepthStencilStateCreateInfo.hpp"
#include "mvk/PipelineInputAssemblyStateCreateInfo.hpp"
#include "mvk/PipelineLayout.hpp"
#include "mvk/PipelineMultisampleStateCreateInfo.hpp"
#include "mvk/PipelineRasterizationStateCreateInfo.hpp"
#include "mvk/PipelineShaderStageCreateInfo.hpp"
#include "mvk/PipelineTessellationStateCreateInfo.hpp"
#include "mvk/PipelineVertexInputStateCreateInfo.hpp"

namespace mvk {
    class Device;
    class PipelineCache;
    class RenderPass;

    class GraphicsPipeline;

    using UPtrGraphicsPipeline = std::unique_ptr<GraphicsPipeline>;

    //! A Pipeline object for use in a GraphicsPipeline.
    class GraphicsPipeline : public virtual Pipeline {
    public:
        struct CreateInfo {
            unsigned int flags;
            std::vector<PipelineShaderStageCreateInfo> stages;
            PipelineLayout::CreateInfo layoutInfo;
            PipelineColorBlendStateCreateInfo colorBlendState;
            PipelineDepthStencilStateCreateInfo depthStencilState;
            PipelineInputAssemblyStateCreateInfo inputAssemblyState;
            PipelineMultisampleStateCreateInfo multisampleState;
            PipelineRasterizationStateCreateInfo rasterizationState;
            PipelineTessellationStateCreateInfo tessellationState;
            PipelineVertexInputStateCreateInfo vertexInputState;
            int subpass;
        };

        static inline UPtrGraphicsPipeline unique_null() {
            return std::unique_ptr<GraphicsPipeline> ();
        }

    private:
        VkPipeline _handle;
        CreateInfo _info;
        PipelineCache * _cache;
        PipelineLayout * _layout;

        GraphicsPipeline(const GraphicsPipeline&) = delete;
        GraphicsPipeline& operator= (const GraphicsPipeline&) = delete;

    public:
        //! The user-defined metadata.
        std::shared_ptr<void> userData;

        //! Creates an empty GraphicsPipeline.
        GraphicsPipeline() noexcept:
            _handle(VK_NULL_HANDLE),
            _cache(nullptr),
            _layout(nullptr) {}

        //! Creates a GraphicsPipeline.
        /*!
            \param cache the PipelineCache that will own this GraphicsPipeline.
            \param createInfo is the construction parameters for the GraphicsPipeline.
            \param renderPass is the RenderPass that the GraphicsPipeline will be used in.
        */
        GraphicsPipeline(PipelineCache * cache, const CreateInfo& createInfo, const RenderPass * renderPass);

        GraphicsPipeline(GraphicsPipeline&& from) noexcept:
            _handle(std::exchange(from._handle, nullptr)),
            _info(std::move(from._info)),
            _cache(std::move(from._cache)),
            _layout(std::move(from._layout)) {}

        GraphicsPipeline& operator= (GraphicsPipeline&& from) noexcept;

        //! Deletes the GraphicsPipeline and releases all resources.
        ~GraphicsPipeline() noexcept;

        //! Retrieves the PipelineBindPoint.
        /*!
            \return PipelineBindPoint::GRAPHICS
        */
        virtual PipelineBindPoint getBindPoint() const noexcept;

        //! Releases any resources held by PipelineBindPoint.
        /*!
            This method is called implicitly by the deconstructor and it is not recommended for it to be manually called.
        */
        virtual void release();

        //! Retrieves the Device.
        /*!
            \return the Device.
        */
        virtual Device * getDevice() const noexcept;

        //! Retrieves the PipelineCache.
        /*!
            \return the PipelineCache.
        */
        virtual PipelineCache * getPipelineCache() const noexcept;

        //! Retrieves the PipelineLayout.
        /*!
            \return the PipelineLayout.
        */
        virtual PipelineLayout * getPipelineLayout() const noexcept;

        //! Retrieves the DescriptorSetLayout.
        /*!
            \param index is the index of the DescriptorSetLayout.
            \return the DescriptorSet.
        */
        virtual DescriptorSetLayout * getDescriptorSetLayout(std::ptrdiff_t index) const noexcept;

        //! Retrieves a list of DescriptorSetLayouts used by the GraphicsPipeline.
        /*!
            \return all of the DescriptorSetLayouts used by the GraphicsPipeline.
        */
        virtual std::vector<DescriptorSetLayout * > getDescriptorSetLayouts() const noexcept;
        
        //! Retrieves the underlying Vulkan handle.
        /*!
            \return the Vulkan handle.
        */
        virtual VkPipeline getHandle() const noexcept;

        //! Implicitly casts to the underlying Vulkan handle.
        inline operator VkPipeline() const noexcept {
            return getHandle();
        }
    };
}
