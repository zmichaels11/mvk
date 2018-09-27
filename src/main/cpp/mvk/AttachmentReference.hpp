#pragma once

#include "mvk/ImageLayout.hpp"

namespace mvk {
    struct AttachmentReference {
        int attachment;
        ImageLayout layout;
    };
}
