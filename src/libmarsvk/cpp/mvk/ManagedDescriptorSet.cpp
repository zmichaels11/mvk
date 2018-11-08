#include "mvk/ManagedDescriptorSet.hpp"

#include "mvk/DescriptorSet.hpp"

namespace mvk {
    ManagedDescriptorSet::~ManagedDescriptorSet() noexcept {
        if (_pSet) {
            _pSet->release();
        }
    }

    ManagedDescriptorSet& ManagedDescriptorSet::operator= (DescriptorSet * pSet) noexcept {
        if (_pSet) {
            _pSet->release();
        }

        _pSet = pSet;

        return *this;
    }

    ManagedDescriptorSet& ManagedDescriptorSet::operator= (ManagedDescriptorSet&& from) noexcept {
        std::swap(this->_pSet, from._pSet);

        return *this;
    }
}
