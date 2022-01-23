#include "metal_texture.h"

namespace pgeditor::renderer::metal {
  static MTLPixelFormat MetalPixelFormat[] = {
    // 8-bit formats
    MTLPixelFormatA8Unorm,
    MTLPixelFormatR8Unorm,
    MTLPixelFormatR8Unorm_sRGB,
    MTLPixelFormatR8Snorm,
    MTLPixelFormatR8Uint,
    MTLPixelFormatR8Sint,

    // 16-bit ordinary formats
    MTLPixelFormatR16Unorm,
    MTLPixelFormatR16Snorm,
    MTLPixelFormatR16Uint,
    MTLPixelFormatR16Sint,
    MTLPixelFormatR16Float,
    MTLPixelFormatRG8Unorm,
    MTLPixelFormatRG8Unorm_sRGB,
    MTLPixelFormatRG8Snorm,
    MTLPixelFormatRG8Uint,
    MTLPixelFormatRG8Sint,

    // 16-bit packed formats
    MTLPixelFormatB5G6R5Unorm,
    MTLPixelFormatA1BGR5Unorm,
    MTLPixelFormatABGR4Unorm,
    MTLPixelFormatBGR5A1Unorm,

    // 32-bit ordinary formats
    MTLPixelFormatR32Uint,
    MTLPixelFormatR32Sint,
    MTLPixelFormatR32Float,
    MTLPixelFormatRG16Unorm,
    MTLPixelFormatRG16Snorm,
    MTLPixelFormatRG16Uint,
    MTLPixelFormatRG16Sint,
    MTLPixelFormatRG16Float,
    MTLPixelFormatRGBA8Unorm,
    MTLPixelFormatRGBA8Unorm_sRGB,
    MTLPixelFormatRGBA8Snorm,
    MTLPixelFormatRGBA8Uint,
    MTLPixelFormatRGBA8Sint,
    MTLPixelFormatBGRA8Unorm,
    MTLPixelFormatBGRA8Unorm_sRGB,

    // 32-bit packed formats
    MTLPixelFormatBGR10A2Unorm,
    MTLPixelFormatRGB10A2Unorm,
    MTLPixelFormatRGB10A2Uint,
    MTLPixelFormatRG11B10Float,
    MTLPixelFormatRGB9E5Float,

    // 64-bit ordinary formats
    MTLPixelFormatRG32Uint,
    MTLPixelFormatRG32Sint,
    MTLPixelFormatRG32Float,
    MTLPixelFormatRGBA16Unorm,
    MTLPixelFormatRGBA16Snorm,
    MTLPixelFormatRGBA16Uint,
    MTLPixelFormatRGBA16Sint,
    MTLPixelFormatRGBA16Float,

    // 128-bit ordinary formats
    MTLPixelFormatRGBA32Uint,
    MTLPixelFormatRGBA32Sint,
    MTLPixelFormatRGBA32Float,

    // Compressed PVRTC formats
    MTLPixelFormatPVRTC_RGB_2BPP,
    MTLPixelFormatPVRTC_RGB_2BPP_sRGB,
    MTLPixelFormatPVRTC_RGB_4BPP,
    MTLPixelFormatPVRTC_RGB_4BPP_sRGB,
    MTLPixelFormatPVRTC_RGBA_2BPP,
    MTLPixelFormatPVRTC_RGBA_2BPP_sRGB,
    MTLPixelFormatPVRTC_RGBA_4BPP,
    MTLPixelFormatPVRTC_RGBA_4BPP_sRGB,

    // Compressed EAC/ETC formats
    MTLPixelFormatEAC_R11Unorm,
    MTLPixelFormatEAC_R11Snorm,
    MTLPixelFormatEAC_RG11Unorm,
    MTLPixelFormatEAC_RG11Snorm,
    MTLPixelFormatEAC_RGBA8,
    MTLPixelFormatEAC_RGBA8_sRGB,
    MTLPixelFormatETC2_RGB8,
    MTLPixelFormatETC2_RGB8_sRGB,
    MTLPixelFormatETC2_RGB8A1,
    MTLPixelFormatETC2_RGB8A1_sRGB,

    // Compressed ASTC formats
    MTLPixelFormatASTC_4x4_sRGB,
    MTLPixelFormatASTC_5x4_sRGB,
    MTLPixelFormatASTC_5x5_sRGB,
    MTLPixelFormatASTC_6x5_sRGB,
    MTLPixelFormatASTC_6x6_sRGB,
    MTLPixelFormatASTC_8x5_sRGB,
    MTLPixelFormatASTC_8x6_sRGB,
    MTLPixelFormatASTC_8x8_sRGB,
    MTLPixelFormatASTC_10x5_sRGB,
    MTLPixelFormatASTC_10x6_sRGB,
    MTLPixelFormatASTC_10x8_sRGB,
    MTLPixelFormatASTC_10x10_sRGB,
    MTLPixelFormatASTC_12x10_sRGB,
    MTLPixelFormatASTC_12x12_sRGB,
    MTLPixelFormatASTC_4x4_LDR,
    MTLPixelFormatASTC_5x4_LDR,
    MTLPixelFormatASTC_5x5_LDR,
    MTLPixelFormatASTC_6x5_LDR,
    MTLPixelFormatASTC_6x6_LDR,
    MTLPixelFormatASTC_8x5_LDR,
    MTLPixelFormatASTC_8x6_LDR,
    MTLPixelFormatASTC_8x8_LDR,
    MTLPixelFormatASTC_10x5_LDR,
    MTLPixelFormatASTC_10x6_LDR,
    MTLPixelFormatASTC_10x8_LDR,
    MTLPixelFormatASTC_10x10_LDR,
    MTLPixelFormatASTC_12x10_LDR,
    MTLPixelFormatASTC_12x12_LDR,
    MTLPixelFormatASTC_4x4_HDR,
    MTLPixelFormatASTC_5x4_HDR,
    MTLPixelFormatASTC_5x5_HDR,
    MTLPixelFormatASTC_6x5_HDR,
    MTLPixelFormatASTC_6x6_HDR,
    MTLPixelFormatASTC_8x5_HDR,
    MTLPixelFormatASTC_8x6_HDR,
    MTLPixelFormatASTC_8x8_HDR,
    MTLPixelFormatASTC_10x5_HDR,
    MTLPixelFormatASTC_10x6_HDR,
    MTLPixelFormatASTC_10x8_HDR,
    MTLPixelFormatASTC_10x10_HDR,
    MTLPixelFormatASTC_12x10_HDR,
    MTLPixelFormatASTC_12x12_HDR,

    // Compressed BC pixel formats
    MTLPixelFormatBC1_RGBA,
    MTLPixelFormatBC1_RGBA_sRGB,
    MTLPixelFormatBC2_RGBA,
    MTLPixelFormatBC2_RGBA_sRGB,
    MTLPixelFormatBC3_RGBA,
    MTLPixelFormatBC3_RGBA_sRGB,
    MTLPixelFormatBC4_RUnorm,
    MTLPixelFormatBC4_RSnorm,
    MTLPixelFormatBC5_RGUnorm,
    MTLPixelFormatBC5_RGSnorm,
    MTLPixelFormatBC6H_RGBFloat,
    MTLPixelFormatBC6H_RGBUfloat,
    MTLPixelFormatBC7_RGBAUnorm,
    MTLPixelFormatBC7_RGBAUnorm_sRGB,

    // YUV pixel formats
    MTLPixelFormatGBGR422,
    MTLPixelFormatBGRG422,

    // Depth and stencil pixel formats
    MTLPixelFormatDepth16Unorm,
    MTLPixelFormatDepth32Float,
    MTLPixelFormatStencil8,
    MTLPixelFormatDepth24Unorm_Stencil8,
    MTLPixelFormatDepth32Float_Stencil8,
    MTLPixelFormatX32_Stencil8,
    MTLPixelFormatX24_Stencil8,

    // Extended range and wide color pixel formats
    MTLPixelFormatBGRA10_XR,
    MTLPixelFormatBGRA10_XR_sRGB,
    MTLPixelFormatBGR10_XR,
    MTLPixelFormatBGR10_XR_sRGB
  };

  static MTLTextureUsage MetalTextureUsage[] = {
    MTLTextureUsageUnknown,
    MTLTextureUsageShaderRead,
    MTLTextureUsageShaderWrite,
    MTLTextureUsageRenderTarget,
    MTLTextureUsagePixelFormatView
  };

  static MTLStorageMode MetalStorageMode[] = { MTLStorageModeShared, MTLStorageModeManaged, MTLStorageModePrivate, MTLStorageModeMemoryless };

////////////////////////////////////////
/// \name Constructors
////////////////////////////////////////
MetalTexture::MetalTexture(uint32_t width, uint32_t height)
    : MetalTexture(width, height, PixelFormat::Invalid, Usage::Invalid,
                   StorageMode::Invalid) {}

MetalTexture::MetalTexture(uint32_t width, uint32_t height,
                           PixelFormat pixelFormat, Usage usage)
    : MetalTexture(width, height, pixelFormat, usage, StorageMode::Invalid) {}

MetalTexture::MetalTexture(uint32_t width, uint32_t height,
                           PixelFormat pixelFormat, Usage usage,
                           StorageMode storage)
    : m_size{width, height}, m_pixelFormat{pixelFormat}, m_usage{usage},
      m_storageMode{storage}, m_metalTexture{nullptr} {}

MetalTexture::~MetalTexture()
{}

////////////////////////////////////////
/// \name Getters & Setters
////////////////////////////////////////
const pagoda::math::Vec2U &MetalTexture::GetSize() const { return m_size; }
MetalTexture::PixelFormat MetalTexture::GetPixelFormat() const {return m_pixelFormat;}
MetalTexture::Usage MetalTexture::GetUsage() const { return m_usage; }
MetalTexture::StorageMode MetalTexture::GetStorageMode() const {return m_storageMode;}
id<MTLTexture> MetalTexture::GetNativeTexture() const { return m_metalTexture; }

////////////////////////////////////////
/// \name Lifetime management
////////////////////////////////////////
id<MTLTexture> MetalTexture::Create(id<MTLDevice> device)
{
  MTLTextureDescriptor *desc = [MTLTextureDescriptor new];
  desc.width = X(m_size);
  desc.height = Y(m_size);
  desc.pixelFormat = MetalPixelFormat[static_cast<uint32_t>(m_pixelFormat)];
  desc.storageMode = MetalStorageMode[static_cast<uint32_t>(m_storageMode)];
  desc.usage = MetalTextureUsage[static_cast<uint32_t>(m_usage)];
  m_metalTexture = [device newTextureWithDescriptor: desc];
  return m_metalTexture;
}

void MetalTexture::Destroy()
{
}

} // namespace pgeditor::renderer::metal
