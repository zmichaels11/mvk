#pragma once

#include <utility>

namespace mvk {
    class DescriptorSet;

    class ManagedDescriptorSet {
        DescriptorSet * _pSet;

        ManagedDescriptorSet(const ManagedDescriptorSet&) = delete;
        ManagedDescriptorSet& operator= (const ManagedDescriptorSet&) = delete;

    public:
        ManagedDescriptorSet() noexcept:
            _pSet(nullptr) {}

        ManagedDescriptorSet(DescriptorSet * pSet) noexcept:
            _pSet(pSet) {}

        ManagedDescriptorSet(ManagedDescriptorSet&& from) noexcept:
            _pSet(std::exchange(from._pSet, nullptr)) {}

        ~ManagedDescriptorSet() noexcept;

        ManagedDescriptorSet& operator= (ManagedDescriptorSet&& from) noexcept;

        ManagedDescriptorSet& operator= (DescriptorSet * pSet) noexcept;

        inline DescriptorSet * get() const noexcept {
            return _pSet;
        }

        inline DescriptorSet * operator->() const noexcept {
            return _pSet;
        }

        inline operator DescriptorSet*() const noexcept {
            return _pSet;
        }
    };
}
