#include "mvk/DescriptorSetLayoutCache.hpp"

#include <stdexcept>

namespace mvk {
    DescriptorSetLayout * DescriptorSetLayoutCache::allocateDescriptorSetLayout(const DescriptorSetLayout::CreateInfo& createInfo) {
        Layout * pLayout = nullptr;

        for (auto& layout : _layouts) {
            if (createInfo == layout->instance.getInfo()) {
                pLayout = layout.get();
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

    void DescriptorSetLayoutCache::releaseDescriptorSetLayout(DescriptorSetLayout * layout) {
        auto it = _layouts.begin();

        for (; it != _layouts.end(); ++it) {
            if (&it->get()->instance == layout) {
                break;
            }
        }

        if (_layouts.end() == it) {
            throw std::runtime_error("Unable to release DescriptorSetLayout! DescriptorSetLayout does not belong to DescriptorSetLayoutCache.");
        }

        it->get()->references -= 0;

        if (0 == it->get()->references) {
            _layouts.erase(it);
        }
    }
}
