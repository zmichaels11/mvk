#pragma once

#include <memory>
#include <vector>

#include "mvk/PipelineLayout.hpp"

namespace mvk {
    class Device;

    class PipelineLayoutCache {
    private:
        struct Layout {
            PipelineLayout instance;
            int references;

            Layout(PipelineLayoutCache * cache, const PipelineLayout::CreateInfo& info) :
                instance(cache, info),
                references(0) {}
        };

        Device * _device;
        std::vector<std::unique_ptr<Layout>> _layouts;

        PipelineLayoutCache(const PipelineLayoutCache&) = delete;
        PipelineLayoutCache& operator= (const PipelineLayoutCache&) = delete;

    public:
        PipelineLayoutCache() noexcept:
            _device(nullptr) {}

        PipelineLayoutCache(Device * device) noexcept:
            _device(device) {}

        PipelineLayoutCache(PipelineLayoutCache&&) = default;

        PipelineLayoutCache& operator= (PipelineLayoutCache&& from) = default;
        
        void releasePipelineLayout(PipelineLayout * layout);

        PipelineLayout * allocatePipelineLayout(const PipelineLayout::CreateInfo& createInfo);

        inline Device * getDevice() const noexcept {
            return _device;
        }
    };
}
