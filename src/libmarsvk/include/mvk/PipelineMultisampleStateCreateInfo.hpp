#pragma once

namespace mvk {
    struct PipelineMultisampleStateCreateInfo {
        unsigned int flags;
        int rasterizationSamples;
        bool sampleShadingEnable;
        float minSampleShading;
        unsigned int * pSampleMask;
        bool alphaToCoverageEnable;
        bool alphaToOneEnable;
    };
}
