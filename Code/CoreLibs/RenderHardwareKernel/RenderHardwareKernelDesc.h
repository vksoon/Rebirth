#pragma once
#include "Prerequisites.h"
#include "EnumClassFlags.h"

RB_NAMESPACE_BEGIN_EX(RHK)

class RBuffer;
class RCommandQueue;
class RCommandBuffer;
class RCommandAllocator;
class RDevice;
class RDescriptor;
class RDescriptorPool;
class RDescriptorSet;
class RFence;
class RFrameBuffer;
class RMemory;
class RPipelineLayout;
class RPipeline;
class RQueryPool;
class RTexture;

static const uint16 REMAINING_ARRAY_LAYERS = 0;
static const uint16 REMAINING_MIP_LEVELS = 0;
static const uint16 WHOLE_SIZE = 0;
static const uint32 WHOLE_DEVICE_GROUP = 0;
static const bool VARIABLE_DESCRIPTOR_NUM = true;
static const bool DESCRIPTOR_ARRAY = true;

typedef uint32 RMemoryType;

enum class EResult : uint8
{
	SUCCESS,
	FAILURE,
	INVALID_ARGUMENT,
	OUT_OF_MEMORY,
	UNSUPPORTED,
	DEVICE_LOST,
	SWAPCHAIN_RESIZE,

	MAX_NUM
};

enum class EVendor : uint8
{
	UNKNOWN,
	NVIDIA,
	AMD,
	INTEL,

	MAX_NUM
};

enum class EGraphicsAPI : uint8
{
	D3D11,
	D3D12,
	VULKAN,

	MAX_NUM
};

enum class ECommandQueueType : uint8
{
	GRAPHICS,
	COMPUTE,
	COPY,

	MAX_NUM
};

enum class EMemoryLocation : uint8
{
	DEVICE,
	HOST_UPLOAD,
	HOST_READBACK,

	MAX_NUM
};

enum class ETextureType : uint8
{
	TEXTURE_1D,
	TEXTURE_2D,
	TEXTURE_3D,

	MAX_NUM
};

enum class ETexture1DViewType : uint8
{
	SHADER_RESOURCE_1D,
	SHADER_RESOURCE_1D_ARRAY,
	SHADER_RESOURCE_STORAGE_1D,
	SHADER_RESOURCE_STORAGE_1D_ARRAY,
	COLOR_ATTACHMENT,
	DEPTH_STENCIL_ATTACHMENT,

	MAX_NUM
};

enum class ETexture2DViewType : uint8
{
	SHADER_RESOURCE_2D,
	SHADER_RESOURCE_2D_ARRAY,
	SHADER_RESOURCE_CUBE,
	SHADER_RESOURCE_CUBE_ARRAY,
	SHADER_RESOURCE_STORAGE_2D,
	SHADER_RESOURCE_STORAGE_2D_ARRAY,
	COLOR_ATTACHMENT,
	DEPTH_STENCIL_ATTACHMENT,

	MAX_NUM
};

enum class ETexture3DViewType : uint8
{
	SHADER_RESOURCE_3D,
	SHADER_RESOURCE_STORAGE_3D,
	COLOR_ATTACHMENT,

	MAX_NUM
};

enum class EBufferViewType : uint8
{
	SHADER_RESOURCE,
	SHADER_RESOURCE_STORAGE,
	CONSTANT,

	MAX_NUM
};

enum class EDescriptorType : uint8
{
	SAMPLER,
	CONSTANT_BUFFER,
	TEXTURE,
	STORAGE_TEXTURE,
	BUFFER,
	STORAGE_BUFFER,
	STRUCTURED_BUFFER,
	STORAGE_STRUCTURED_BUFFER,
	ACCELERATION_STRUCTURE,

	MAX_NUM
};

enum class EVertexStreamStepRate
{
	PER_VERTEX,
	PER_INSTANCE,

	MAX_NUM
};

enum class ETextureLayout : uint8
{
	GENERAL,
	COLOR_ATTACHMENT,
	DEPTH_STENCIL,
	DEPTH_STENCIL_READONLY,
	DEPTH_READONLY,
	STENCIL_READONLY,
	SHADER_RESOURCE,
	PRESENT,
	UNKNOWN,

	MAX_NUM
};

enum class EShaderStage : uint8
{
	ALL,
	VERTEX,
	TESS_CONTROL,
	TESS_EVALUATION,
	GEOMETRY,
	FRAGMENT,
	COMPUTE,

	MAX_NUM
};

enum class EBarrierDependency : uint8
{
	ALL_STAGES,
	GRAPHICS_STAGE,
	COMPUTE_STAGE,
	COPY_STAGE,

	MAX_NUM
};

enum class ETopology : uint8
{
	POINT_LIST,
	LINE_LIST,
	LINE_STRIP,
	TRIANGLE_LIST,
	TRIANGLE_STRIP,
	LINE_LIST_WITH_ADJACENCY,
	LINE_STRIP_WITH_ADJACENCY,
	TRIANGLE_LIST_WITH_ADJACENCY,
	TRIANGLE_STRIP_WITH_ADJACENCY,
	PATCH_LIST,

	MAX_NUM
};

enum class EFillMode : uint8
{
	SOLID,
	WIREFRAME,

	MAX_NUM
};

enum class ECullMode : uint8
{
	NONE,
	FRONT,
	BACK,

	MAX_NUM
};

enum class ELogicFunc : uint8
{
	NONE,
	CLEAR,
	AND,
	AND_REVERSE,
	COPY,
	AND_INVERTED,
	XOR,
	OR,
	NOR,
	EQUIVALENT,
	INVERT,
	OR_REVERSE,
	COPY_INVERTED,
	OR_INVERTED,
	NAND,
	SET,

	MAX_NUM
};

enum class ECompareFunc : uint8
{
	NONE,
	ALWAYS,
	NEVER,
	LESS,
	LESS_EQUAL,
	EQUAL,
	GREATER_EQUAL,
	GREATER,
	NOT_EQUAL,

	MAX_NUM
};

enum class EStencilFunc : uint8
{
	KEEP,
	ZERO,
	REPLACE,
	INCREMENT_AND_CLAMP,
	DECREMENT_AND_CLAMP,
	INVERT,
	INCREMENT_AND_WRAP,
	DECREMENT_AND_WRAP,

	MAX_NUM
};

enum class EBlendFactor : uint8
{
	ZERO,
	ONE,
	SRC_COLOR,
	ONE_MINUS_SRC_COLOR,
	DST_COLOR,
	ONE_MINUS_DST_COLOR,
	SRC_ALPHA,
	ONE_MINUS_SRC_ALPHA,
	DST_ALPHA,
	ONE_MINUS_DST_ALPHA,
	CONSTANT_COLOR,
	ONE_MINUS_CONSTANT_COLOR,
	CONSTANT_ALPHA,
	ONE_MINUS_CONSTANT_ALPHA,
	SRC_ALPHA_SATURATE,
	SRC1_COLOR,
	ONE_MINUS_SRC1_COLOR,
	SRC1_ALPHA,
	ONE_MINUS_SRC1_ALPHA,

	MAX_NUM
};

enum class EBlendFunc : uint8
{
	ADD,
	SUBTRACT,
	REVERSE_SUBTRACT,
	MIN,
	MAX,

	MAX_NUM
};

enum class EIndexType : uint8
{
	UINT16,
	UINT32,

	MAX_NUM
};

enum class EQueryType : uint8
{
	TIMESTAMP,
	OCCLUSION,
	PIPELINE_STATISTICS,

	MAX_NUM
};

enum class EFilter : uint8
{
	NEAREST,
	LINEAR,

	MAX_NUM
};

enum class EFilterExt : uint8
{
	NONE,
	MIN,
	MAX,

	MAX_NUM
};

enum class EAddressMode : uint8
{
	REPEAT,
	MIRRORED_REPEAT,
	CLAMP_TO_EDGE,
	CLAMP_TO_BORDER,

	MAX_NUM
};

enum class EBorderColor : uint8
{
	FLOAT_TRANSPARENT_BLACK,
	FLOAT_OPAQUE_BLACK,
	FLOAT_OPAQUE_WHITE,
	INT_TRANSPARENT_BLACK,
	INT_OPAQUE_BLACK,
	INT_OPAQUE_WHITE,

	MAX_NUM
};

enum class EFormat : uint8
{
	UNKNOWN,

	R8_UNORM,
	R8_SNORM,
	R8_UINT,
	R8_SINT,

	RG8_UNORM,
	RG8_SNORM,
	RG8_UINT,
	RG8_SINT,

	BGRA8_UNORM,
	BGRA8_SRGB,

	RGBA8_UNORM,
	RGBA8_SNORM,
	RGBA8_UINT,
	RGBA8_SINT,
	RGBA8_SRGB,

	R16_UNORM,
	R16_SNORM,
	R16_UINT,
	R16_SINT,
	R16_SFLOAT,

	RG16_UNORM,
	RG16_SNORM,
	RG16_UINT,
	RG16_SINT,
	RG16_SFLOAT,

	RGBA16_UNORM,
	RGBA16_SNORM,
	RGBA16_UINT,
	RGBA16_SINT,
	RGBA16_SFLOAT,

	R32_UINT,
	R32_SINT,
	R32_SFLOAT,

	RG32_UINT,
	RG32_SINT,
	RG32_SFLOAT,

	RGB32_UINT,
	RGB32_SINT,
	RGB32_SFLOAT,

	RGBA32_UINT,
	RGBA32_SINT,
	RGBA32_SFLOAT,

	R10_G10_B10_A2_UNORM,
	R10_G10_B10_A2_UINT,
	R11_G11_B10_UFLOAT,
	R9_G9_B9_E5_UFLOAT,

	BC1_RGBA_UNORM,
	BC1_RGBA_SRGB,
	BC2_RGBA_UNORM,
	BC2_RGBA_SRGB,
	BC3_RGBA_UNORM,
	BC3_RGBA_SRGB,
	BC4_R_UNORM,
	BC4_R_SNORM,
	BC5_RG_UNORM,
	BC5_RG_SNORM,
	BC6H_RGB_UFLOAT,
	BC6H_RGB_SFLOAT,
	BC7_RGBA_UNORM,
	BC7_RGBA_SRGB,

	D16_UNORM,
	D24_UNORM_S8_UINT,
	D32_SFLOAT,
	D32_SFLOAT_S8_UINT_X24,

	R24_UNORM_X8,
	X24_R8_UINT,
	X32_R8_UINT_X24,
	R32_SFLOAT_X8_X24,

	MAX_NUM
};

enum class EAttachmentContentType : uint8
{
	COLOR,
	DEPTH,
	STENCIL,
	DEPTH_STENCIL,

	MAX_NUM
};

enum class ERenderPassBeginFlag : uint8
{
	NONE,
	SKIP_FRAME_BUFFER_CLEAR,

	MAX_NUM
};

enum class EPrimitiveRestart : uint8
{
	DISABLED,
	INDICES_UINT16,
	INDICES_UINT32,

	MAX_NUM
};

enum class ETextureUsageBits : uint16
{
	NONE = 0,
	SHADER_RESOURCE = RB_SET_BIT(0),
	SHADER_RESOURCE_STORAGE = RB_SET_BIT(1),
	COLOR_ATTACHMENT = RB_SET_BIT(2),
	DEPTH_STENCIL_ATTACHMENT = RB_SET_BIT(3)
};
RB_ENUM_CLASS_FLAGS(ETextureUsageBits)

enum class EBufferUsageBits : uint16
{
	NONE = 0,
	SHADER_RESOURCE = RB_SET_BIT(0),
	SHADER_RESOURCE_STORAGE = RB_SET_BIT(1),
	VERTEX_BUFFER = RB_SET_BIT(2),
	INDEX_BUFFER = RB_SET_BIT(3),
	CONSTANT_BUFFER = RB_SET_BIT(4),
	ARGUMENT_BUFFER = RB_SET_BIT(5),
};
RB_ENUM_CLASS_FLAGS(EBufferUsageBits)

enum class EAccessBits : uint16
{
	UNKNOWN = 0,
	VERTEX_BUFFER = RB_SET_BIT(0),
	INDEX_BUFFER = RB_SET_BIT(1),
	CONSTANT_BUFFER = RB_SET_BIT(2),
	ARGUMENT_BUFFER = RB_SET_BIT(3),
	SHADER_RESOURCE = RB_SET_BIT(4),
	SHADER_RESOURCE_STORAGE = RB_SET_BIT(5),
	COLOR_ATTACHMENT = RB_SET_BIT(6),
	DEPTH_STENCIL_WRITE = RB_SET_BIT(7),
	DEPTH_STENCIL_READ = RB_SET_BIT(8),
	COPY_SOURCE = RB_SET_BIT(9),
	COPY_DESTINATION = RB_SET_BIT(10),
};
RB_ENUM_CLASS_FLAGS(EAccessBits)

enum class EPipelineLayoutShaderStageBits : uint16
{
	NONE = 0,
	VERTEX = RB_SET_BIT(1),
	TESS_CONTROL = RB_SET_BIT(2),
	TESS_EVALUATION = RB_SET_BIT(3),
	GEOMETRY = RB_SET_BIT(4),
	FRAGMENT = RB_SET_BIT(5),
	COMPUTE = RB_SET_BIT(6),

	ALL_GRAPHICS = VERTEX | TESS_CONTROL | TESS_EVALUATION | GEOMETRY | FRAGMENT
};
RB_ENUM_CLASS_FLAGS(EPipelineLayoutShaderStageBits)

enum class EPipelineStatsBits : uint16
{
	INPUT_ASSEMBLY_VERTICES = RB_SET_BIT(0),
	INPUT_ASSEMBLY_PRIMITIVES = RB_SET_BIT(1),
	VERTEX_SHADER_INVOCATIONS = RB_SET_BIT(2),
	GEOMETRY_SHADER_INVOCATIONS = RB_SET_BIT(3),
	GEOMETRY_SHADER_PRIMITIVES = RB_SET_BIT(4),
	CLIPPING_INVOCATIONS = RB_SET_BIT(5),
	CLIPPING_PRIMITIVES = RB_SET_BIT(6),
	FRAGMENT_SHADER_INVOCATIONS = RB_SET_BIT(7),
	TESS_CONTROL_SHADER_PATCHES = RB_SET_BIT(8),
	TESS_EVALUATION_SHADER_INVOCATIONS = RB_SET_BIT(9),
	COMPUTE_SHADER_INVOCATIONS = RB_SET_BIT(10)
};
RB_ENUM_CLASS_FLAGS(EPipelineStatsBits)

enum class EColorWriteBits : uint8
{
	R = RB_SET_BIT(0),
	G = RB_SET_BIT(1),
	B = RB_SET_BIT(2),
	A = RB_SET_BIT(3),

	RGBA = R | G | B | A
};
RB_ENUM_CLASS_FLAGS(EColorWriteBits)

enum class EResourceViewBits : uint8
{
	READONLY_DEPTH = RB_SET_BIT(0),
	READONLY_STENCIL = RB_SET_BIT(1)
};
RB_ENUM_CLASS_FLAGS(EResourceViewBits)

enum class EFormatSupportBits : uint8
{
	UNSUPPORTED = 0,
	TEXTURE = RB_SET_BIT(0),
	STORAGE_TEXTURE = RB_SET_BIT(1),
	BUFFER = RB_SET_BIT(2),
	STORAGE_BUFFER = RB_SET_BIT(3),
	COLOR_ATTACHMENT = RB_SET_BIT(4),
	DEPTH_STENCIL_ATTACHMENT = RB_SET_BIT(5),
	VERTEX_BUFFER = RB_SET_BIT(6)
};
RB_ENUM_CLASS_FLAGS(EFormatSupportBits)

enum class EDescriptorSetBindingBits : uint8
{
	DEFAULT = 0,
	PARTIALLY_BOUND = RB_SET_BIT(0)
};
RB_ENUM_CLASS_FLAGS(EDescriptorSetBindingBits)

struct RRect
{
	int32 Left;
	int32 Top;
	uint32 Width;
	uint32 Height;
};

struct RViewport
{
	float Offset[2];
	float Size[2];
	float DepthRangeMin;
	float DepthRangeMax;
};

struct RColor32f
{
	float X, Y, Z, W;
};

struct RColor32ui
{
	uint32 X, Y, Z, W;
};

struct RColor32i
{
	int32 X, Y, Z, W;
};

struct RDepthStencilClearValue
{
	float Depth;
	uint8 Stencil;
};

union RClearValueDesc
{
	RDepthStencilClearValue DepthStencil;
	RColor32f Color32f;
	RColor32ui Color32ui;
	RColor32i Color32i;
};

struct RClearDesc
{
	RClearValueDesc Value;
	EAttachmentContentType AttachmentContentType;
	uint32 ColorAttachmentIndex;
};

struct RClearStorageBufferDesc
{
	const RDescriptor* StorageBuffer;
	uint32 Value;
	uint32 SetIndexInPipelineLayout;
	uint32 RangeIndex;
	uint32 OffsetInRange;
};

struct RClearStorageTextureDesc
{
	const RDescriptor* StorageTexture;
	RClearValueDesc Value;
	uint32 SetIndexInPipelineLayout;
	uint32 RangeIndex;
	uint32 OffsetInRange;
};

struct RTextureRegionDesc
{
	uint16 Offset[3];
	uint16 Size[3];
	uint16 MipOffset;
	uint16 ArrayOffset;
};

struct RTextureDataLayoutDesc
{
	uint64 Offset;
	uint32 RowPitch;
	uint32 SlicePitch;
};

struct RQueueSubmitDesc
{
	const RCommandBuffer* const* CommandBuffers;
	uint32 CommandBufferNum;
	uint32 PhysicalDeviceIndex;
};

struct RBufferMemoryBindingDesc
{
	RMemory* Memory;
	RBuffer* Buffer;
	uint64 Offset;
	uint32 PhysicalDeviceMask;
};

struct RTextureMemoryBindingDesc
{
	RMemory* Memory;
	RTexture* Texture;
	uint64 Offset;
	uint32 PhysicalDeviceMask;
};

struct RMemoryDesc
{
	uint64 Size;
	uint32 Alignment;
	RMemoryType Type;
	bool bMustBeDedicated;
};

struct RAddressModes
{
	EAddressMode U;
	EAddressMode V;
	EAddressMode W;
};

struct RSamplerDesc
{
	EFilter Magnification;
	EFilter Minification;
	EFilter Mip;
	EFilterExt FilterExt;
	uint32 Anisotropy;
	float MipBias;
	float MipMin;
	float MipMax;
	RAddressModes AddressModes;
	ECompareFunc CompareFunc;
	EBorderColor BorderColor;
	bool bUnnormalizedCoordinates;
};

struct RTextureDesc
{
	ETextureType Type;
	ETextureUsageBits UsageMask;
	EFormat Format;
	uint16 Size[3];
	uint16 MipNum;
	uint16 ArraySize;
	uint8 SampleNum;
	uint32 PhysicalDeviceMask;
};

struct RBufferDesc
{
	uint64 Size;
	uint32 StructureStride;
	EBufferUsageBits UsageMask;
	uint32 PhysicalDeviceMask;
};

struct RTexture1DViewDesc
{
	const RTexture* Texture;
	ETexture1DViewType ViewType;
	EFormat Format;
	uint16 MipOffset;
	uint16 MipNum;
	uint16 ArrayOffset;
	uint16 ArraySize;
	uint32 PhysicalDeviceMask;
	EResourceViewBits Flags;
};

struct RTexture2DViewDesc
{
	const RTexture* Texture;
	ETexture2DViewType ViewType;
	EFormat Format;
	uint16 MipOffset;
	uint16 MipNum;
	uint16 ArrayOffset;
	uint16 ArraySize;
	uint32 PhysicalDeviceMask;
	EResourceViewBits Flags;
};

struct RTexture3DViewDesc
{
	const RTexture* Texture;
	ETexture3DViewType ViewType;
	EFormat Format;
	uint16 MipOffset;
	uint16 MipNum;
	uint16 SliceOffset;
	uint16 SliceNum;
	uint32 PhysicalDeviceMask;
	EResourceViewBits Flags;
};

struct RBufferViewDesc
{
	const RBuffer* Buffer;
	EBufferViewType ViewType;
	EFormat Format;
	uint64 Offset;
	uint64 Size;
	uint32 PhysicalDeviceMask;
};

struct RDescriptorPoolDesc
{
	uint32 PhysicalDeviceMask;
	uint32 DescriptorSetMaxNum;
	uint32 SamplerMaxNum;
	uint32 ConstantBufferMaxNum;
	uint32 DynamicConstantBufferMaxNum;
	uint32 TextureMaxNum;
	uint32 StorageTextureMaxNum;
	uint32 BufferMaxNum;
	uint32 StorageBufferMaxNum;
	uint32 StructuredBufferMaxNum;
	uint32 StorageStructuredBufferMaxNum;
};

struct RTextureTransitionBarrierDesc
{
	const RTexture* Texture;
	uint16 MipOffset;
	uint16 MipNum;
	uint16 ArrayOffset;
	uint16 ArraySize;
	EAccessBits PrevAccess;
	EAccessBits NextAccess;
	ETextureLayout PrevLayout;
	ETextureLayout NextLayout;
};

struct RBufferTransitionBarrierDesc
{
	const RBuffer* Buffer;
	EAccessBits PrevAccess;
	EAccessBits NextAccess;
};

struct RBufferAliasingBarrierDesc
{
	const RBuffer* Before;
	const RBuffer* After;
	EAccessBits NextAccess;
};

struct RTextureAliasingBarrierDesc
{
	const RBuffer* Before;
	const RBuffer* After;
	EAccessBits NextAccess;
	ETextureLayout NextLayout;
};

struct RTransitionBarrierDesc
{
	const RBufferTransitionBarrierDesc* Buffers;
	const RTextureTransitionBarrierDesc* Textures;
	uint32 BufferNum;
	uint32 TextureNum;
};

struct RAliasingBarrierDesc
{
	const RBufferAliasingBarrierDesc* Buffers;
	const RTextureAliasingBarrierDesc* Textures;
	uint32 BufferNum;
	uint32 TextureNum;
};

struct RDescriptorRangeDesc
{
	uint32 BaseRegisterIndex;
	uint32 DescriptorNum;
	EDescriptorType DescriptorType;
	EShaderStage Visibility;
	bool bIsDescriptorNumVariable;
	bool bIsArray;
};

struct RDynamicConstantBufferDesc
{
	uint32 RegisterIndex;
	EShaderStage Visibility;
};

struct RDescriptorSetDesc
{
	uint32 RegisterSpace;
	const RDescriptorRangeDesc* Ranges;
	uint32 RangeNum;
	const RDynamicConstantBufferDesc* DynamicConstantBuffers;
	uint32 DynamicConstantBufferNum;
	EDescriptorSetBindingBits BindingMask;
};

struct RDescriptorRangeUpdateDesc
{
	const RDescriptor* const* Descriptors;
	uint32 DescriptorNum;
	uint32 OffsetInRange;
};

struct RDescriptorSetCopyDesc
{
	const RDescriptorSet* SrcDescriptorSet;
	uint32 BaseSrcRange;
	uint32 BaseDstRange;
	uint32 RangeNum;
	uint32 BaseSrcDynamicConstantBuffer;
	uint32 BaseDstDynamicConstantBuffer;
	uint32 DynamicConstantBufferNum;
	uint32 PhysicalDeviceMask;
};

struct RPushConstantDesc
{
	uint32 RegisterIndex;
	uint32 Size;
	EShaderStage Visibility;
};

struct RSPIRVBindingOffsets
{
	uint32 SamplerOffset;
	uint32 TextureOffset;
	uint32 ConstantBufferOffset;
	uint32 StorageTextureAndBufferOffset;
};

struct RShaderDesc
{
	EShaderStage Stage;
	const void* Bytecode;
	uint64 Size;
	const char* EntryPointName;
};

struct RVertexAttributeD3D
{
	const char* SemanticName;
	uint32 SemanticIndex;
};

struct RVertexAttributeVulkan
{
	uint32 Location;
};

struct RVertexAttributeDesc
{
	RVertexAttributeD3D D3D;
	RVertexAttributeVulkan Vulkan;
	uint32 Offset;
	EFormat Format;
	uint16 StreamIndex;
};

struct RVertexStreamDesc
{
	uint32 Stride;
	uint16 BindingSlot;
	EVertexStreamStepRate StepRate;
};

struct RInputAssemblyDesc
{
	const RVertexAttributeDesc* Attributes;
	const RVertexStreamDesc* Streams;
	uint8 AttributeNum;
	uint8 StreamNum;
	ETopology Topology;
	uint8 TessControlPointNum;
	EPrimitiveRestart PrimitiveRestart;
};

struct RSamplePosition
{
	int8 X;
	int8 Y;
};

struct RRasterizationDesc
{
	uint32 ViewportNum;
	int32 DepthBiasConstantFactor;
	float DepthBiasClamp;
	float DepthBiasSlopeFactor;
	EFillMode FillMode;
	ECullMode CullMode;
	uint16 SampleMask;
	uint8 SampleNum;
	bool bAlphaToCoverage;
	bool bFrontCounterClockwise;
	bool bDepthClamp;
	bool bAntialiasedLines;
	bool bRasterizerDiscard;
	bool bConservativeRasterization;
};

struct RStencilDesc
{
	ECompareFunc CompareFunc;
	EStencilFunc Fail;
	EStencilFunc Pass;
	EStencilFunc DepthFail;
};

struct RBlendingDesc
{
	EBlendFactor SrcFactor;
	EBlendFactor DstFactor;
	EBlendFunc Func;
};

struct RColorAttachmentDesc
{
	EFormat Format;
	RBlendingDesc ColorBlend;
	RBlendingDesc AlphaBlend;
	EColorWriteBits ColorWriteMask;
	bool bBlendEnabled;
};

// CompareFunc::NONE = depth/stencil test disabled
struct RDepthAttachmentDesc
{
	ECompareFunc CompareFunc;
	bool bWrite;
};

struct RStencilAttachmentDesc
{
	RStencilDesc Front;
	RStencilDesc Back;
	uint8 Reference;
	uint8 CompareMask;
	uint8 WriteMask;
};

struct ROutputMergerDesc
{
	const RColorAttachmentDesc* Color;
	RDepthAttachmentDesc Depth;
	RStencilAttachmentDesc Stencil;
	EFormat DepthStencilFormat;
	ELogicFunc ColorLogicFunc;
	uint32 ColorNum;
	RColor32f BlendConsts;
};

struct RPipelineLayoutDesc
{
	const RDescriptorSetDesc* DescriptorSets;
	const RPushConstantDesc* PushConstants;
	uint32 DescriptorSetNum;
	uint32 PushConstantNum;
	EPipelineLayoutShaderStageBits StageMask;
	bool bIgnoreGlobalSPIRVOffsets;
};

struct RGraphicsPipelineDesc
{
	const RPipelineLayout* PipelineLayout;
	const RInputAssemblyDesc* InputAssembly;
	const RRasterizationDesc* Rasterization;
	const ROutputMergerDesc* OutputMerger;
	const RShaderDesc* ShaderStages;
	uint32 ShaderStageNum;
};

struct RComputePipelineDesc
{
	const RPipelineLayout* PipelineLayout;
	RShaderDesc ComputeShader;
};

struct RFrameBufferDesc
{
	const RDescriptor* const* ColorAttachments;
	const RDescriptor* DepthStencilAttachment;
	const RClearValueDesc* ColorClearValues;
	const RClearValueDesc* DepthStencilClearValue;
	uint32 ColorAttachmentNum;
	uint32 PhysicalDeviceMask;
	uint16 Size[2];
	uint16 LayerNum;
};

struct RQueryPoolDesc
{
	EQueryType QueryType;
	uint32 Capacity;
	EPipelineStatsBits PipelineStatsMask;
	uint32 PhysicalDeviceMask;
};

struct RPipelineStatisticsDesc
{
	uint64 InputVertices;
	uint64 InputPrimitives;
	uint64 VertexShaderInvocations;
	uint64 GeometryShaderInvocations;
	uint64 GeometryShaderPrimitives;
	uint64 RasterizerInPrimitives;
	uint64 RasterizerOutPrimitives;
	uint64 FragmentShaderInvocations;
	uint64 TessControlInvocations;
	uint64 TessEvaluationInvocations;
	uint64 ComputeShaderInvocations;
};

struct RDeviceDesc
{
	// Common
	EGraphicsAPI GraphicsAPI;
	EVendor Vendor;
	uint16 VersionMajor;
	uint16 VersionMinor;

	// Viewports
	uint32 ViewportMaxNum;
	uint32 ViewportSubPixelBits;
	int32 ViewportBoundsRange[2];

	// Framebuffer
	uint32 FrameBufferMaxDim;
	uint32 FrameBufferLayerMaxNum;
	uint32 FramebufferColorAttachmentMaxNum;

	// Multi-sampling
	uint8 FrameBufferColorSampleMaxNum;
	uint8 FrameBufferDepthSampleMaxNum;
	uint8 FrameBufferStencilSampleMaxNum;
	uint8 FrameBufferNoAttachmentsSampleMaxNum;
	uint8 TextureColorSampleMaxNum;
	uint8 TextureIntegerSampleMaxNum;
	uint8 TextureDepthSampleMaxNum;
	uint8 TextureStencilSampleMaxNum;
	uint8 StorageTextureSampleMaxNum;

	// Resource dimensions
	uint32 Texture1DMaxDim;
	uint32 Texture2DMaxDim;
	uint32 Texture3DMaxDim;
	uint32 TextureArrayMaxDim;
	uint32 TexelBufferMaxDim;

	// Memory
	uint32 MemoryAllocationMaxNum;
	uint32 SamplerAllocationMaxNum;
	uint32 UploadBufferTextureRowAlignment;
	uint32 UploadBufferTextureSliceAlignment;
	uint32 TypedBufferOffsetAlignment;
	uint32 ConstantBufferOffsetAlignment;
	uint32 ConstantBufferMaxRange;
	uint32 StorageBufferOffsetAlignment;
	uint32 StorageBufferMaxRange;
	uint32 BufferTextureGranularity;
	uint64 BufferMaxSize;
	uint32 PushConstantsMaxSize;

	// Shader resources
	uint32 BoundDescriptorSetMaxNum;
	uint32 PerStageDescriptorSamplerMaxNum;
	uint32 PerStageDescriptorConstantBufferMaxNum;
	uint32 PerStageDescriptorStorageBufferMaxNum;
	uint32 PerStageDescriptorTextureMaxNum;
	uint32 PerStageDescriptorStorageTextureMaxNum;
	uint32 PerStageResourceMaxNum;

	// Descriptor set
	uint32 DescriptorSetSamplerMaxNum;
	uint32 DescriptorSetConstantBufferMaxNum;
	uint32 DescriptorSetStorageBufferMaxNum;
	uint32 DescriptorSetTextureMaxNum;
	uint32 DescriptorSetStorageTextureMaxNum;

	// Vertex shader
	uint32 VertexShaderAttributeMaxNum;
	uint32 VertexShaderStreamMaxNum;
	uint32 VertexShaderOutputComponentMaxNum;

	// Tessellation control shader
	float TessControlShaderGenerationMaxLevel;
	uint32 TessControlShaderPatchPointMaxNum;
	uint32 TessControlShaderPerVertexInputComponentMaxNum;
	uint32 TessControlShaderPerVertexOutputComponentMaxNum;
	uint32 TessControlShaderPerPatchOutputComponentMaxNum;
	uint32 TessControlShaderTotalOutputComponentMaxNum;

	// Tessellation evaluation shader
	uint32 TessEvaluationShaderInputComponentMaxNum;
	uint32 TessEvaluationShaderOutputComponentMaxNum;

	// Geometry shader
	uint32 GeometryShaderInvocationMaxNum;
	uint32 GeometryShaderInputComponentMaxNum;
	uint32 GeometryShaderOutputComponentMaxNum;
	uint32 GeometryShaderOutputVertexMaxNum;
	uint32 GeometryShaderTotalOutputComponentMaxNum;

	// Fragment shader
	uint32 FragmentShaderInputComponentMaxNum;
	uint32 FragmentShaderOutputAttachmentMaxNum;
	uint32 FragmentShaderDualSourceAttachmentMaxNum;
	uint32 FragmentShaderCombinedOutputResourceMaxNum;

	// Compute shader
	uint32 ComputeShaderSharedMemoryMaxSize;
	uint32 ComputeShaderWorkGroupMaxNum[3];
	uint32 ComputeShaderWorkGroupInvocationMaxNum;
	uint32 ComputeShaderWorkGroupMaxDim[3];

	// Other
	uint64 TimestampFrequencyHz;
	uint32 SubPixelPrecisionBits;
	uint32 SubTexelPrecisionBits;
	uint32 MipmapPrecisionBits;
	uint32 DrawIndexedIndex16ValueMax;
	uint32 DrawIndexedIndex32ValueMax;
	uint32 DrawIndirectMaxNum;
	float SamplerLodBiasMin;
	float SamplerLodBiasMax;
	float SamplerAnisotropyMax;
	int32 TexelOffsetMin;
	uint32 TexelOffsetMax;
	int32 TexelGatherOffsetMin;
	uint32 TexelGatherOffsetMax;
	uint32 ClipDistanceMaxNum;
	uint32 CullDistanceMaxNum;
	uint32 CombinedClipAndCullDistanceMaxNum;
	uint8 ConservativeRasterTier;
	uint8 PhysicalDeviceNum;

	// Features support
	bool bIsAPIValidationEnabled;
	bool bIsTextureFilterMinMaxSupported;
	bool bIsLogicOpSupported;
	bool bIsDepthBoundsTestSupported;
	bool bIsProgrammableSampleLocationsSupported;
	bool bIsComputeQueueSupported;
	bool bIsCopyQueueSupported;
	bool bIsCopyQueueTimestampSupported;
	bool bIsRegisterAliasingSupported;
	bool bIsSubsetAllocationSupported;
	bool bIsFloat16Supported;
};

RB_NAMESPACE_END_EX