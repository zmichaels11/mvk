#pragma once

#include "mvk/Format.hpp"
#include "mvk/ImageLayout.hpp"
#include "mvk/LoadOp.hpp"
#include "mvk/StoreOp.hpp"

namespace mvk {
    struct AttachmentDescription {
        unsigned int flags;
        Format format;
        LoadOp loadOp;
        StoreOp storeOp;
        LoadOp stencilLoadOp;
        StoreOp stencilStoreOp;
        ImageLayout initialLayout;
        ImageLayout finalLayout;
        int samples;
    };
}
