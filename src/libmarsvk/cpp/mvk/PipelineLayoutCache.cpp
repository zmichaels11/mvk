#include "mvk/PipelineLayoutCache.hpp"

#include <stdexcept>

namespace mvk {
    void PipelineLayoutCache::releasePipelineLayout(PipelineLayout * layout) {
        auto it = _layouts.begin();
        Layout * pLayout = nullptr;

        for(; it != _layouts.end(); ++it) {
            if (&it->get()->instance == layout) {
                pLayout = it->get();
                break;
            }
        }

        if (nullptr == pLayout) {
            throw std::runtime_error("Unable to release Pipeline! PipelineLayout does not belong to PipelineLayoutCache!");
        }

        pLayout->references -= 1;

        if (0 == pLayout->references) {
            _layouts.erase(it);
        }
    }

    PipelineLayout * PipelineLayoutCache::allocatePipelineLayout(const PipelineLayout::CreateInfo& createInfo) {
        auto it = _layouts.begin();
        Layout * pLayout = nullptr;

        for (; it != _layouts.end(); ++it) {
            if (createInfo == it->get()->instance.getInfo()) {
                pLayout = it->get();
                break;
            }
        }

        if (nullptr == pLayout) {
            auto ptr = std::make_unique<Layout> (this, createInfo);
            pLayout = ptr.get();

            _layouts.push_back(std::move(ptr));
        }

        pLayout->references += 1;

        return &pLayout->instance;
    }
}
