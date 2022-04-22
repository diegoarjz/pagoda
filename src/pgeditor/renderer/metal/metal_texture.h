#pragma once

#include <pagoda/math/vec_base.h>

#include <cstdint>

#include <MetalKit/MetalKit.h>

namespace pgeditor::renderer::metal {

class MetalTexture {
public:
  enum class PixelFormat {
    // 8-bit formats
    A8Unorm,
    R8Unorm,
    //R8Unorm_sRGB,
    R8Snorm,
    R8Uint,
    R8Sint,

    // 16-bit ordinary formats
    R16Unorm,
    R16Snorm,
    R16Uint,
    R16Sint,
    R16Float,
    RG8Unorm,
    //RG8Unorm_sRGB,
    RG8Snorm,
    RG8Uint,
    RG8Sint,

    // 16-bit packed formats
    //B5G6R5Unorm,
    //A1BGR5Unorm,
    //ABGR4Unorm,
    //BGR5A1Unorm,

    // 32-bit ordinary formats
    R32Uint,
    R32Sint,
    R32Float,
    RG16Unorm,
    RG16Snorm,
    RG16Uint,
    RG16Sint,
    RG16Float,
    RGBA8Unorm,
    RGBA8Unorm_sRGB,
    RGBA8Snorm,
    RGBA8Uint,
    RGBA8Sint,
    BGRA8Unorm,
    BGRA8Unorm_sRGB,

    // 32-bit packed formats
    BGR10A2Unorm,
    RGB10A2Unorm,
    RGB10A2Uint,
    RG11B10Float,
    RGB9E5Float,

    // 64-bit ordinary formats
    RG32Uint,
    RG32Sint,
    RG32Float,
    RGBA16Unorm,
    RGBA16Snorm,
    RGBA16Uint,
    RGBA16Sint,
    RGBA16Float,

    // 128-bit ordinary formats
    RGBA32Uint,
    RGBA32Sint,
    RGBA32Float,

    // Compressed PVRTC formats
    //PVRTC_RGB_2BPP,
    //PVRTC_RGB_2BPP_sRGB,
    //PVRTC_RGB_4BPP,
    //PVRTC_RGB_4BPP_sRGB,
    //PVRTC_RGBA_2BPP,
    //PVRTC_RGBA_2BPP_sRGB,
    //PVRTC_RGBA_4BPP,
    //PVRTC_RGBA_4BPP_sRGB,

    // Compressed EAC/ETC formats
    //EAC_R11Unorm,
    //EAC_R11Snorm,
    //EAC_RG11Unorm,
    //EAC_RG11Snorm,
    //EAC_RGBA8,
    //EAC_RGBA8_sRGB,
    //ETC2_RGB8,
    //ETC2_RGB8_sRGB,
    //ETC2_RGB8A1,
    //ETC2_RGB8A1_sRGB,

    // Compressed ASTC formats
    //ASTC_4x4_sRGB,
    //ASTC_5x4_sRGB,
    //ASTC_5x5_sRGB,
    //ASTC_6x5_sRGB,
    //ASTC_6x6_sRGB,
    //ASTC_8x5_sRGB,
    //ASTC_8x6_sRGB,
    //ASTC_8x8_sRGB,
    //ASTC_10x5_sRGB,
    //ASTC_10x6_sRGB,
    //ASTC_10x8_sRGB,
    //ASTC_10x10_sRGB,
    //ASTC_12x10_sRGB,
    //ASTC_12x12_sRGB,
    //ASTC_4x4_LDR,
    //ASTC_5x4_LDR,
    //ASTC_5x5_LDR,
    //ASTC_6x5_LDR,
    //ASTC_6x6_LDR,
    //ASTC_8x5_LDR,
    //ASTC_8x6_LDR,
    //ASTC_8x8_LDR,
    //ASTC_10x5_LDR,
    //ASTC_10x6_LDR,
    //ASTC_10x8_LDR,
    //ASTC_10x10_LDR,
    //ASTC_12x10_LDR,
    //ASTC_12x12_LDR,
    //ASTC_4x4_HDR,
    //ASTC_5x4_HDR,
    //ASTC_5x5_HDR,
    //ASTC_6x5_HDR,
    //ASTC_6x6_HDR,
    //ASTC_8x5_HDR,
    //ASTC_8x6_HDR,
    //ASTC_8x8_HDR,
    //ASTC_10x5_HDR,
    //ASTC_10x6_HDR,
    //ASTC_10x8_HDR,
    //ASTC_10x10_HDR,
    //ASTC_12x10_HDR,
    //ASTC_12x12_HDR,

    // Compressed BC pixel formats
    BC1_RGBA,
    BC1_RGBA_sRGB,
    BC2_RGBA,
    BC2_RGBA_sRGB,
    BC3_RGBA,
    BC3_RGBA_sRGB,
    BC4_RUnorm,
    BC4_RSnorm,
    BC5_RGUnorm,
    BC5_RGSnorm,
    BC6H_RGBFloat,
    BC6H_RGBUfloat,
    BC7_RGBAUnorm,
    BC7_RGBAUnorm_sRGB,

    // YUV pixel formats
    GBGR422,
    BGRG422,

    // Depth and stencil pixel formats
    Depth16Unorm,
    Depth32Float,
    Stencil8,
    Depth24Unorm_Stencil8,
    Depth32Float_Stencil8,
    X32_Stencil8,
    X24_Stencil8,

    // Extended range and wide color pixel formats
    //BGRA10_XR,
    //BGRA10_XR_sRGB,
    //BGR10_XR,
    //BGR10_XR_sRGB,

    Invalid
  };

  enum class Usage {
    Unknown,
    ShaderRead,
    ShaderWrite,
    RenderTarget,
    PixelFormatView,
    Invalid
  };

  enum class StorageMode { Shared, Managed, Private, /*Memoryless,*/ Invalid };

  ////////////////////////////////////////
  /// \name Constructors
  ////////////////////////////////////////
  MetalTexture(uint32_t width, uint32_t height);
  MetalTexture(uint32_t width, uint32_t height, PixelFormat pixelFormat,
               Usage usage);
  MetalTexture(uint32_t width, uint32_t height, PixelFormat pixelFormat,
               Usage usage, StorageMode storage);
  ~MetalTexture();

  ////////////////////////////////////////
  /// \name Getters & Setters
  ////////////////////////////////////////
  const pagoda::math::Vec2U &GetSize() const;
  PixelFormat GetPixelFormat() const;
  Usage GetUsage() const;
  StorageMode GetStorageMode() const;
  id<MTLTexture> GetNativeTexture() const;

  ////////////////////////////////////////
  /// \name Lifetime management
  ////////////////////////////////////////
  id<MTLTexture> Create(id<MTLDevice> device);
  void Destroy();

private:
  pagoda::math::Vec2U m_size;
  PixelFormat m_pixelFormat;
  Usage m_usage;
  StorageMode m_storageMode;
  id<MTLTexture> m_metalTexture;
};
} // namespace pgeditor::renderer::metal
