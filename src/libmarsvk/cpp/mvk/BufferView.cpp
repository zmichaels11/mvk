#include "mvk/BufferView.hpp"

#include "volk.h"

#include "mvk/Buffer.hpp"
#include "mvk/Device.hpp"
#include "mvk/Util.hpp"

namespace mvk {
    BufferView::BufferView(const Buffer * buffer, const BufferView::CreateInfo& info) {
        _buffer = buffer;
        _info = info;

        auto bufferViewCI = VkBufferViewCreateInfo {};
        bufferViewCI.sType = VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO;
        bufferViewCI.flags = static_cast<VkBufferViewCreateFlags> (info.flags);
        bufferViewCI.offset = static_cast<VkDeviceSize> (info.offset);
        bufferViewCI.range = static_cast<VkDeviceSize> (info.range);
        bufferViewCI.format = static_cast<VkFormat> (info.format);

        Util::vkAssert(vkCreateBufferView(buffer->getDevice()->getHandle(), &bufferViewCI, nullptr, &_handle));
    }

    BufferView::~BufferView() noexcept {
        if (VK_NULL_HANDLE == _handle) {
            return;
        }

        vkDestroyBufferView(getDevice()->getHandle(), _handle, nullptr);
    }

    BufferView& BufferView::operator= (BufferView&& from) noexcept {
        std::swap(_buffer, from._buffer);
        std::swap(_info, from._info);
        std::swap(_handle, from._handle);

        return *this;
    }

    Device * BufferView::getDevice() const noexcept {
        return _buffer->getDevice();
    }
}
