#pragma once

#include "volk.h"

#include <vector>

namespace mvk {
    class Framebuffer {
    public:
        struct CreateInfo {
            int width;
            int height;
            int layers;
        };

    private:
        VkFramebuffer _handle;
        CreateInfo _info;
        std::vector<ImageView
    };
}
