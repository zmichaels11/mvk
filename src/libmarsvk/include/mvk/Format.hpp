#pragma once

#include "volk.h"

namespace mvk {
    //! Supported formats
    enum class Format : unsigned int {
        D16_UNORM_S8_UINT = VK_FORMAT_D16_UNORM_S8_UINT,                /*!< Specifies a format with 16bit unsigned normalized depth and 8bit unsigned integer stencil. */
        D24_UNORM_S8_UINT = VK_FORMAT_D24_UNORM_S8_UINT,                /*!< Specifies a format with 24bit unsigned normalized depth and 8bit unsigned integer stencil. */
        D32_SFLOAT_S8_UINT = VK_FORMAT_D32_SFLOAT_S8_UINT,              /*!< Specifies a format with 32bit floating point depth and 8bit unsigned integer stencil. */
        R8_UNORM = VK_FORMAT_R8_UNORM,                                  /*!< Specifies a format with 8bit unsigned normalized R component. */
        R8_SNORM = VK_FORMAT_R8_SNORM,                                  /*!< Specifies a format with 8bit signed normalized R component. */
        R8_UINT = VK_FORMAT_R8_UINT,                                    /*!< Specifies a format with 8bit unsigned int R component. */
        R8_SINT = VK_FORMAT_R8_SINT,                                    /*!< Specifies a format with 8bit signed int R component. */
        R8G8_UNORM = VK_FORMAT_R8G8_UNORM,                              /*!< Specifies a format with 8bit unsigned normalized RG components. */
        R8G8_SNORM = VK_FORMAT_R8G8_SNORM,                              /*!< Specifies a format with 8bit signed normalized RG components. */
        R8G8B8A8_UNORM = VK_FORMAT_R8G8B8A8_UNORM,                      /*!< Specifies a format with 8bit unsigned normalized RGBA components. */
        R8G8B8A8_SNORM = VK_FORMAT_R8G8B8A8_SNORM,                      /*!< Specifies a format with 8bit signed normalized RGBA components. */
        B8G8R8A8_UNORM = VK_FORMAT_B8G8R8A8_UNORM,                      /*!< Specifies a format with 8bit unsigned normalized BGRA components. */
        B8G8R8A8_SNORM = VK_FORMAT_B8G8R8A8_SNORM,                      /*!< Specifies a format with 8bit signed normalized BGRA components. */
        R16_UNORM = VK_FORMAT_R16_UNORM,                                /*!< Specifies a format with 16bit unsigned normalized R component. */
        R16_SNORM = VK_FORMAT_R16_SNORM,                                /*!< Specifies a format with 16bit signed normalized R component. */
        R16_UINT = VK_FORMAT_R16_UINT,                                  /*!< Specifies a format with 16bit unsigned int R component. */
        R16_SINT = VK_FORMAT_R16_SINT,                                  /*!< Specifies a format with 16bit signed int R component. */
        R16_SFLOAT = VK_FORMAT_R16_SFLOAT,                              /*!< Specifies a format with 16bit float R component. */
        R16G16_UNORM = VK_FORMAT_R16G16_UNORM,                          /*!< Specifies a format with 16bit unsigned normalized RG components. */
        R16G16_SNORM = VK_FORMAT_R16G16_SNORM,                          /*!< Specifies a format with 16bit signed  normalized RG components. */
        R16G16B16A16_UNORM = VK_FORMAT_R16G16B16A16_UNORM,              /*!< Specifies a format with 16bit unsigned normalized RGBA components. */
        R16G16B16A16_SNORM = VK_FORMAT_R16G16B16A16_SNORM,              /*!< Specifies a format with 16bit signed normalized RGBA components. */
        R32_UINT = VK_FORMAT_R32_UINT,                                  /*!< Specifies a format with 32bit unsigned int R component. */
        R32_SINT = VK_FORMAT_R32_SINT,                                  /*!< Specifies a format with 32bit signed int R component. */
        R32_SFLOAT = VK_FORMAT_R32_SFLOAT,                              /*!< Specifies a format with 32bit float R component. */
        R32G32_UINT = VK_FORMAT_R32G32_UINT,                            /*!< Specifies a format with 32bit unsigned int RG components. */
        R32G32_SINT = VK_FORMAT_R32G32_SINT,                            /*!< Specifies a format with 32bit signed int RG components. */
        R32G32_SFLOAT = VK_FORMAT_R32G32_SFLOAT,                        /*!< Specifies a format with 32bit float RG components. */
        R32G32B32A32_UINT = VK_FORMAT_R32G32B32A32_UINT,                /*!< Specifies a format with 32bit unsigned int RGBA components. */
        R32G32B32A32_SINT = VK_FORMAT_R32G32B32A32_SINT,                /*!< Specifies a format with 32bit signed int RGBA components. */
        R32G32B32A32_SFLOAT = VK_FORMAT_R32G32B32A32_SFLOAT,            /*!< Specifies a format with 32bit float RGBA components. */
        A2B10G10R10_UNORM_PACK32 = VK_FORMAT_A2B10G10R10_UNORM_PACK32,  /*!< Specifies a format with 2bit unsigned normalized A component and 10bit unsigned normalized BGR components. */
        A2B10G10R10_UINT_PACK32 = VK_FORMAT_A2B10G10R10_UINT_PACK32     /*!< Specifies a format with 2bit unsigned int A component and 10bit unsigned int BGR components. */
    };
}
