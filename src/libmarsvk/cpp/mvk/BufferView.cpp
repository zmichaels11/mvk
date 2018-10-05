#include "mvk/BufferView.hpp"

#include "volk.h"

#include "mvk/Buffer.hpp"
#include "mvk/Device.hpp"
#include "mvk/Util.hpp"

namespace mvk {
    BufferView::BufferView(const Buffer * buffer, const BufferView::CreateInfo& info) {
        _buffer = buffer;
        _info = info;

        VkBufferViewCreateInfo bufferViewCI {};
        bufferViewCI.sType = VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO;
        bufferViewCI.flags = info.flags;
        bufferViewCI.offset = info.offset;
        bufferViewCI.range = info.range;
        bufferViewCI.format = static_cast<VkFormat> (info.format);

        Util::vkAssert(vkCreateBufferView(buffer->getDevice()->getHandle(), &bufferViewCI, nullptr, &_handle));
    }

    BufferView::~BufferView() {
        if (VK_NULL_HANDLE == _handle) {
            return;
        }

        vkDestroyBufferView(getDevice()->getHandle(), _handle, nullptr);
    }

    Device * BufferView::getDevice() const {
        return _buffer->getDevice();
    }
}
