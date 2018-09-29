#pragma once

#include "volk.h"
#include "vk_mem_alloc.h"

namespace mvk {
    enum class MemoryUsage {
        UNKNOWN = VMA_MEMORY_USAGE_UNKNOWN,
        GPU_ONLY = VMA_MEMORY_USAGE_GPU_ONLY,
        CPU_ONLY = VMA_MEMORY_USAGE_CPU_ONLY,
        CPU_TO_GPU = VMA_MEMORY_USAGE_CPU_TO_GPU,
        GPU_TO_CPU = VMA_MEMORY_USAGE_GPU_TO_CPU
    };
}
