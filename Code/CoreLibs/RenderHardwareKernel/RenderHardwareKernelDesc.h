#pragma once
#include "Prerequisites.h"
#include "EnumClassFlags.h"

RB_NAMESPACE_BEGIN

// Entities
class RBuffer;
class RCommandQueue;
class RCommandBuffer;
class RCommandAllocator;
class RDevice;
class RDescriptor;
class RDescriptorPool;
class RDescriptorSet;
class RFence;
class RMemory;
class RPipelineLayout;
class RPipeline;
class RQueryPool;
class RTexture;

// Types
typedef uint32_t MemoryType;
typedef uint16_t Dim_t;
typedef uint8_t Mip_t;
typedef uint8_t Sample_t;

// Aliases
static const Dim_t REMAINING_ARRAY_LAYERS = 0; // only for "arraySize"
static const Mip_t REMAINING_MIP_LEVELS = 0; // only for "mipNum"
static const Dim_t WHOLE_SIZE = 0; // only for "Dim_t" and "size"
static const uint32 ALL_SAMPLES = 0; // only for "sampleMask"
static const uint32 ONE_VIEWPORT = 0; // only for "viewportNum"
static const bool VARIABLE_DESCRIPTOR_NUM = true;
static const bool DESCRIPTOR_ARRAY = true;
static const bool PARTIALLY_BOUND = true;

//===============================================================================================================================
// Common
//===============================================================================================================================
#pragma region [ Common ]

enum class EResult : uint8
{
	SUCCESS,
	FAILURE,
	INVALID_ARGUMENT,
	OUT_OF_MEMORY,
	UNSUPPORTED,
	DEVICE_LOST,
	OUT_OF_DATE, // VK only: swap chain is out of date

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

enum class EFormat : uint8
{
	UNKNOWN,

	// 8-bit
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

	// 16-bit
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

	// 32-bit
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

	// Packed
	R10_G10_B10_A2_UNORM,
	R10_G10_B10_A2_UINT,
	R11_G11_B10_UFLOAT,
	R9_G9_B9_E5_UFLOAT,

	// Block-compressed
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

	// DEPTH_STENCIL_ATTACHMENT views
	D16_UNORM,
	D24_UNORM_S8_UINT,
	D32_SFLOAT,
	D32_SFLOAT_S8_UINT_X24,

	// Depth-stencil specific SHADER_RESOURCE views
	R24_UNORM_X8,
	X24_R8_UINT,
	X32_R8_UINT_X24,
	R32_SFLOAT_X8_X24,

	MAX_NUM
};

enum class EFormatSupportBits : uint8
{
	UNSUPPORTED				 = 0,
	TEXTURE					 = RB_SET_BIT(0),
	STORAGE_TEXTURE			 = RB_SET_BIT(1),
	BUFFER					 = RB_SET_BIT(2),
	STORAGE_BUFFER			 = RB_SET_BIT(3),
	COLOR_ATTACHMENT		 = RB_SET_BIT(4),
	DEPTH_STENCIL_ATTACHMENT = RB_SET_BIT(5),
	VERTEX_BUFFER			 = RB_SET_BIT(6)
};
RB_ENUM_CLASS_FLAGS(EFormatSupportBits)

enum class EStageBits : uint32
{
	// Special
	ALL = 0,               // Lazy default for barriers
	NONE = 0x7FFFFFFF,

	// Graphics                                        // Invoked by "CmdDraw*"
	INDEX_INPUT					= RB_SET_BIT(0),  //    Index buffer consumption
	VERTEX_SHADER				= RB_SET_BIT(1),  //    Vertex shader
	TESS_CONTROL_SHADER			= RB_SET_BIT(2),  //    Tessellation control (hull) shader
	TESS_EVALUATION_SHADER		= RB_SET_BIT(3),  //    Tessellation evaluation (domain) shader
	GEOMETRY_SHADER				= RB_SET_BIT(4),  //    Geometry shader
	MESH_CONTROL_SHADER			= RB_SET_BIT(5),  //    Mesh control (task) shader
	MESH_EVALUATION_SHADER		= RB_SET_BIT(6),  //    Mesh evaluation (amplification) shader
	FRAGMENT_SHADER				= RB_SET_BIT(7),  //    Fragment (pixel) shader
	DEPTH_STENCIL_ATTACHMENT	= RB_SET_BIT(8),  //    Depth-stencil R/W operations
	COLOR_ATTACHMENT			= RB_SET_BIT(9),  //    Color R/W operations

	// Compute Invoked by  "CmdDispatch*" (not Rays)
	COMPUTE_SHADER				= RB_SET_BIT(10), //    Compute shader

	// Ray tracing Invoked by "CmdDispatchRays"
	RAYGEN_SHADER				= RB_SET_BIT(11), //    Ray generation shader
	MISS_SHADER					= RB_SET_BIT(12), //    Miss shader
	INTERSECTION_SHADER			= RB_SET_BIT(13), //    Intersection shader
	CLOSEST_HIT_SHADER			= RB_SET_BIT(14), //    Closest hit shader
	ANY_HIT_SHADER				= RB_SET_BIT(15), //    Any hit shader
	CALLABLE_SHADER				= RB_SET_BIT(16), //    Callable shader

	// Other stages
	COPY						= RB_SET_BIT(17), // Invoked by "CmdCopy*", "CmdUpload*" and "CmdReadback*"
	CLEAR_STORAGE				= RB_SET_BIT(18), // Invoked by "CmdClearStorage*"
	ACCELERATION_STRUCTURE		= RB_SET_BIT(19), // Invoked by "Cmd*AccelerationStructure*"

	// Modifiers
	INDIRECT					= RB_SET_BIT(20), // Invoked by "Indirect" command (used as addition to other bits)

	// Umbrella stages
	TESSELLATION_SHADERS		= TESS_CONTROL_SHADER | TESS_EVALUATION_SHADER,
	MESH_SHADERS				= MESH_CONTROL_SHADER |	MESH_EVALUATION_SHADER,
	GRAPHICS_SHADERS			= VERTEX_SHADER | TESSELLATION_SHADERS | GEOMETRY_SHADER | MESH_SHADERS | FRAGMENT_SHADER,

	// Invoked by "CmdDispatchRays"
	RAY_TRACING_SHADERS			= RAYGEN_SHADER | MISS_SHADER | INTERSECTION_SHADER | CLOSEST_HIT_SHADER | ANY_HIT_SHADER |	CALLABLE_SHADER,

	// Invoked by "CmdDraw*"
	DRAW						= INDEX_INPUT | GRAPHICS_SHADERS | DEPTH_STENCIL_ATTACHMENT | COLOR_ATTACHMENT
};

struct RRect
{
	int16 X;
	int16 Y;
	Dim_t Width;
	Dim_t Height;
};

struct RViewport
{
	float X;
	float Y;
	float Width;
	float Height;
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

struct RDepthStencil
{
	float Depth;
	uint8 Stencil;
};

struct RSamplePosition
{
	int8 X, Y; // [-8; 7]
};

#pragma endregion

//===============================================================================================================================
// Creation
//===============================================================================================================================
#pragma region [ Creation ]

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
	DEVICE_UPLOAD, // soft fallback to HOST_UPLOAD
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

enum class ETextureUsageBits : uint16
{
	NONE = 0,
	SHADER_RESOURCE			 = RB_SET_BIT(0),
	SHADER_RESOURCE_STORAGE  = RB_SET_BIT(1),
	COLOR_ATTACHMENT		 = RB_SET_BIT(2),
	DEPTH_STENCIL_ATTACHMENT = RB_SET_BIT(3)
};
RB_ENUM_CLASS_FLAGS(ETextureUsageBits)

enum class EBufferUsageBits : uint16
{
	NONE = 0,
	SHADER_RESOURCE					  = RB_SET_BIT(0),
	SHADER_RESOURCE_STORAGE			  = RB_SET_BIT(1),
	VERTEX_BUFFER					  = RB_SET_BIT(2),
	INDEX_BUFFER					  = RB_SET_BIT(3),
	CONSTANT_BUFFER					  = RB_SET_BIT(4),
	ARGUMENT_BUFFER					  = RB_SET_BIT(5),
	RAY_TRACING_BUFFER				  = RB_SET_BIT(6),
	ACCELERATION_STRUCTURE_BUILD_READ = RB_SET_BIT(7)
};
RB_ENUM_CLASS_FLAGS(EBufferUsageBits)

enum class EResourceViewBits : uint8
{
	READONLY_DEPTH = RB_SET_BIT(0),
	READONLY_STENCIL = RB_SET_BIT(1)
};
RB_ENUM_CLASS_FLAGS(EResourceViewBits)

struct RTextureDesc
{
	ETextureType Type;
	ETextureUsageBits UsageMask;
	EFormat Format;
	Dim_t Width;
	Dim_t Height;
	Dim_t Depth;
	Mip_t MipNum;
	Dim_t ArraySize;
	Sample_t SampleNum;
};

struct RBufferDesc
{
	uint64 Size;
	uint32 StructureStride;
	EBufferUsageBits UsageMask;
};

struct RTexture1DViewDesc
{
	const RTexture* Texture;
	ETexture1DViewType ViewType;
	EFormat Format;
	Mip_t MipOffset;
	Mip_t MipNum;
	Dim_t ArrayOffset;
	Dim_t ArraySize;
	EResourceViewBits Flags;
};

struct RTexture2DViewDesc
{
	const RTexture* Texture;
	ETexture2DViewType ViewType;
	EFormat Format;
	Mip_t MipOffset;
	Mip_t MipNum;
	Dim_t ArrayOffset;
	Dim_t ArraySize;
	EResourceViewBits Flags;
};

struct RTexture3DViewDesc
{
	const RTexture* Texture;
	ETexture3DViewType ViewType;
	EFormat Format;
	Mip_t MipOffset;
	Mip_t MipNum;
	Dim_t SliceOffset;
	Dim_t SliceNum;
	EResourceViewBits Flags;
};

struct RBufferViewDesc
{
	const RBuffer* Buffer;
	EBufferViewType ViewType;
	EFormat Format;
	uint64 Offset;
	uint64 Size;
};

struct RDescriptorPoolDesc
{
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
	uint32 AccelerationStructureMaxNum;
};

#pragma endregion

//===============================================================================================================================
// Pipeline layout
//===============================================================================================================================
#pragma region [ Pipeline layout ]

struct RPushConstantDesc
{
	uint32 RegisterIndex;
	uint32 Size;
	EStageBits ShaderStages;
};

struct RDescriptorRangeDesc
{
	uint32 BaseRegisterIndex;
	uint32 DescriptorNum;
	EDescriptorType DescriptorType;
	EStageBits ShaderStages;
	bool bIsDescriptorNumVariable;
	bool bIsArray;
};

struct RDynamicConstantBufferDesc
{
	uint32 RegisterIndex;
	EStageBits ShaderStages;
};

struct RDescriptorSetDesc
{
	uint32 RegisterSpace;
	const RDescriptorRangeDesc* Ranges;
	uint32 RangeNum;
	const RDynamicConstantBufferDesc* DynamicConstantBuffers;
	uint32 DynamicConstantBufferNum;
	bool PartiallyBound;
};

struct RPipelineLayoutDesc
{
	const RDescriptorSetDesc* DescriptorSets;
	const RPushConstantDesc* PushConstants;
	uint32 DescriptorSetNum;
	uint32 PushConstantNum;
	EStageBits StageMask;
	bool bIgnoreGlobalSPIRVOffsets;
};

#pragma endregion

//===============================================================================================================================
// Input assembly
//===============================================================================================================================
#pragma region [ Input assembly ]

enum class EVertexStreamStepRate : uint8
{
	PER_VERTEX,
	PER_INSTANCE,

	MAX_NUM
};

enum class EIndexType : uint8
{
	UINT16,
	UINT32,

	MAX_NUM
};

enum class EPrimitiveRestart : uint8
{
	DISABLED,
	INDICES_UINT16,
	INDICES_UINT32,

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


struct RVertexAttributeVulkan
{
	uint32 Location;
};

struct RVertexAttributeD3D
{
	const char* SemanticName;
	uint32 SemanticIndex;
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

struct RVertexInputDesc
{
	const RVertexAttributeDesc* Attributes;
	const RVertexStreamDesc* Streams;
	uint8 AttributeNum;
	uint8 StreamNum;
};

#pragma endregion

//===============================================================================================================================
// Rasterization
//===============================================================================================================================
#pragma region [ Rasterization ]

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

struct RRasterizationDesc
{
	uint32 ViewportNum;
	float DepthBias;
	float DepthBiasClamp;
	float DepthBiasSlopeFactor;
	EFillMode FillMode;
	ECullMode CullMode;
	bool FrontCounterClockwise;
	bool DepthClamp;
	bool AntialiasedLines; // Requires "isLineSmoothingSupported"
	bool ConservativeRasterization; // Requires "conservativeRasterTier > 0"
};

struct RMultisampleDesc
{
	uint32 SampleMask;
	Sample_t SampleNum;
	bool AlphaToCoverage;
	bool ProgrammableSampleLocations; // Requires "isProgrammableSampleLocationsSupported"
};

#pragma endregion

//===============================================================================================================================
// Output merger
//===============================================================================================================================
#pragma region [ Output merger ]

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

enum class EAttachmentContentType : uint8
{
	COLOR,
	DEPTH,
	STENCIL,
	DEPTH_STENCIL,

	MAX_NUM
};

enum class EColorWriteBits : uint8
{
	R = RB_SET_BIT(0),
	G = RB_SET_BIT(1),
	B = RB_SET_BIT(2),
	A = RB_SET_BIT(3),

	RGB	 = R | G | B,
	RGBA = RGB | A
};
RB_ENUM_CLASS_FLAGS(EColorWriteBits)

union RClearValue
{
	RDepthStencil DepthStencil;
	RColor32f Color32f;
	RColor32ui Color32ui;
	RColor32i Color32i;
};

struct RClearDesc
{
	RClearValue Value;
	EAttachmentContentType AttachmentContentType;
	uint32 ColorAttachmentIndex;
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

struct RAttachmentsDesc
{
	const RDescriptor* DepthStencil;
	const RDescriptor* const* Colors;
	uint32 ColorNum;
};

#pragma endregion

//===============================================================================================================================
// Sampler
//===============================================================================================================================
#pragma region [ Sampler ]

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

struct RAddressModes
{
	EAddressMode U;
	EAddressMode V;
	EAddressMode W;
};

struct RFilters
{
	EFilter Min, Mag, Mip;
	EFilterExt Ext;
};

struct RSamplerDesc
{
	RFilters Filters;
	uint8 Anisotropy;
	float MipBias;
	float MipMin;
	float MipMax;
	RAddressModes AddressModes;
	ECompareFunc CompareFunc;
	EBorderColor BorderColor;
};

#pragma endregion

//===============================================================================================================================
// Pipeline
//===============================================================================================================================
#pragma region [ Pipeline ]

struct RShaderDesc
{
	EStageBits Stage;
	const void* Bytecode;
	uint64 Size;
	const char* EntryPointName;
};

struct RGraphicsPipelineDesc
{
	const RPipelineLayout* PipelineLayout;
	const RVertexInputDesc* VertexInput; // optional
	RInputAssemblyDesc InputAssembly;
	RRasterizationDesc Rasterization;
	const RMultisampleDesc* Multisample; // optional
	ROutputMergerDesc OutputMerger;
	const RShaderDesc* Shaders;
	uint32 ShaderNum;
};

struct RComputePipelineDesc
{
	const RPipelineLayout* PipelineLayout;
	RShaderDesc ComputeShader;
};

#pragma endregion

//===============================================================================================================================
// Barrier
//===============================================================================================================================
#pragma region [ Barrier ]

enum class ELayout : uint8
{
	UNKNOWN,
	COLOR_ATTACHMENT,
	DEPTH_STENCIL,
	DEPTH_STENCIL_READONLY,
	SHADER_RESOURCE,
	SHADER_RESOURCE_STORAGE,
	COPY_SOURCE,
	COPY_DESTINATION,
	PRESENT,

	MAX_NUM
};

enum class EAccessBits : uint16
{
	UNKNOWN = 0,
	INDEX_BUFFER				 = RB_SET_BIT(0),  // INDEX_INPUT
	VERTEX_BUFFER				 = RB_SET_BIT(1),  // VERTEX_SHADER
	CONSTANT_BUFFER				 = RB_SET_BIT(2),  // GRAPHICS_SHADERS, COMPUTE_SHADER, RAY_TRACING_SHADERS
	SHADER_RESOURCE				 = RB_SET_BIT(3),  // GRAPHICS_SHADERS, COMPUTE_SHADER, RAY_TRACING_SHADERS
	SHADER_RESOURCE_STORAGE		 = RB_SET_BIT(4),  // GRAPHICS_SHADERS, COMPUTE_SHADER, RAY_TRACING_SHADERS, CLEAR_STORAGE
	ARGUMENT_BUFFER				 = RB_SET_BIT(5),  // INDIRECT
	COLOR_ATTACHMENT			 = RB_SET_BIT(6),  // COLOR_ATTACHMENT
	DEPTH_STENCIL_WRITE			 = RB_SET_BIT(7),  // DEPTH_STENCIL_ATTACHMENT
	DEPTH_STENCIL_READ			 = RB_SET_BIT(8),  // DEPTH_STENCIL_ATTACHMENT
	COPY_SOURCE					 = RB_SET_BIT(9),  // COPY
	COPY_DESTINATION			 = RB_SET_BIT(10), // COPY
	ACCELERATION_STRUCTURE_READ	 = RB_SET_BIT(11), // COMPUTE_SHADER, RAY_TRACING, ACCELERATION_STRUCTURE
	ACCELERATION_STRUCTURE_WRITE = RB_SET_BIT(12), // COMPUTE_SHADER, RAY_TRACING, ACCELERATION_STRUCTURE
	SHADING_RATE				 = RB_SET_BIT(13)  // FRAGMENT_SHADER // TODO: WIP
};
RB_ENUM_CLASS_FLAGS(EAccessBits)

struct RAccessStage
{
	EAccessBits Access;
	EStageBits Stages;
};

struct RAccessLayoutStage
{
	EAccessBits Access;
	ELayout Layout;
	EStageBits Stages;
};

struct RGlobalBarrierDesc
{
	RAccessStage Before;
	RAccessStage After;
};

struct RBufferBarrierDesc
{
	RBuffer* Buffer;
	RAccessStage Before;
	RAccessStage After;
};

struct RTextureBarrierDesc
{
	RTexture* Texture;
	RAccessLayoutStage Before;
	RAccessLayoutStage After;
	Mip_t MipOffset;
	Mip_t MipNum;
	Dim_t ArrayOffset;
	Dim_t ArraySize;
};

struct RBarrierGroupDesc
{
	const RGlobalBarrierDesc* Globals;
	const RBufferBarrierDesc* Buffers;
	const RTextureBarrierDesc* Textures;
	uint16 GlobalNum;
	uint16 BufferNum;
	uint16 TextureNum;
};

#pragma endregion

//===============================================================================================================================
// Other
//===============================================================================================================================
#pragma region [ Other ]

struct RTextureRegionDesc
{
	uint16 X, Y, Z;
	Dim_t Width;
	Dim_t Height;
	Dim_t Depth;
	Mip_t MipOffset;
	Dim_t ArrayOffset;
};

struct RTextureDataLayoutDesc
{
	uint64 Offset;
	uint32 RowPitch;
	uint32 SlicePitch;
};

// Work submission
struct RFenceSubmitDesc
{
	RFence* Fence;
	uint64 Value;
	EStageBits Stages;
};

struct RQueueSubmitDesc
{
	const RFenceSubmitDesc* WaitFences;
	uint32 WaitFenceNum;
	const RCommandBuffer* const* CommandBuffers;
	uint32 CommandBufferNum;
	const RFenceSubmitDesc* SignalFences;
	uint32 SignalFenceNum;
};

struct RBufferMemoryBindingDesc
{
	RMemory* Memory;
	RBuffer* Buffer;
	uint64 Offset;
};

struct RTextureMemoryBindingDesc
{
	RMemory* Memory;
	RTexture* Texture;
	uint64 Offset;
};

struct RMemoryDesc
{
	uint64 Size;
	uint32 Alignment;
	uint32 Type;
	bool bMustBeDedicated;
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
	RClearValue Value;
	uint32 SetIndexInPipelineLayout;
	uint32 RangeIndex;
	uint32 OffsetInRange;
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
};

// Command signatures
struct RDrawDesc
{
	uint32 VertexNum;
	uint32 InstanceNum;
	uint32 BbaseVertex; // vertex buffer offset = CmdSetVertexBuffers.offset + baseVertex * VertexStreamDesc::stride
	uint32 BaseInstance;
};

struct RDrawIndexedDesc
{
	uint32 IndexNum;
	uint32 InstanceNum;
	uint32 BaseIndex; // index buffer offset = CmdSetIndexBuffer.offset + baseIndex * sizeof(CmdSetIndexBuffer.indexType)
	int32 BaseVertex; // index += baseVertex
	uint32 BaseInstance;
};

struct RDispatchDesc
{
	uint32 X;
	uint32 Y;
	uint32 Z;
};

#pragma endregion

//===============================================================================================================================
// Queries
//===============================================================================================================================
#pragma region [ Queries ]


enum class EQueryType : uint8
{
	TIMESTAMP,
	TIMESTAMP_COPY_QUEUE, // requires "isCopyQueueTimestampSupported"
	OCCLUSION,
	PIPELINE_STATISTICS,
	ACCELERATION_STRUCTURE_COMPACTED_SIZE,

	MAX_NUM
};

struct RQueryPoolDesc
{
	EQueryType QueryType;
	uint32 Capacity;
};

// Data layout for QueryType::PIPELINE_STATISTICS
// Never used, only describes the data layout for various cases
struct RPipelineStatisticsDesc
{
	// Common part
	uint64 InputVertexNum;
	uint64 InputPrimitiveNum;
	uint64 VertexShaderInvocationNum;
	uint64 GeometryShaderInvocationNum;
	uint64 GeometryShaderPrimitiveNum;
	uint64 RasterizerInPrimitiveNum;
	uint64 RasterizerOutPrimitiveNum;
	uint64 FragmentShaderInvocationNum;
	uint64 TessControlShaderInvocationNum;
	uint64 TessEvaluationShaderInvocationNum;
	uint64 ComputeShaderInvocationNum;

	// If "isMeshShaderPipelineStatsSupported"
	uint64 MeshControlShaderInvocationNum;
	uint64 MeshEvaluationShaderInvocationNum;

	// If "isMeshShaderPipelineStatsSupported" and D3D12
	uint64 MeshEvaluationShaderPrimitiveNum;
};

#pragma endregion

//===============================================================================================================================
// Device desc
//===============================================================================================================================
#pragma region [ Device desc ]

struct RAdapterDesc
{
	char Description[128];
	uint64 LUID;
	uint64 VideoMemorySize;
	uint64 SystemMemorySize;
	uint32 DeviceId;
	EVendor Vendor;
};

struct RDeviceDesc
{
	// Common
	RAdapterDesc AdapterDesc;
	EGraphicsAPI GraphicsAPI;
	uint16 VersionMajor;
	uint16 VersionMinor;

	// Viewports
	uint32 ViewportMaxNum;
	uint32 ViewportSubPixelBits;
	int32 ViewportBoundsRange[2];

	// Attachments
	Dim_t AttachmentMaxDim;
	Dim_t AttachmentLayerMaxNum;
	Dim_t ColorAttachmentMaxNum;

	// Multi-sampling
	Sample_t ColorSampleMaxNum;
	Sample_t DepthSampleMaxNum;
	Sample_t StencilSampleMaxNum;
	Sample_t ZeroAttachmentsSampleMaxNum;
	Sample_t TextureColorSampleMaxNum;
	Sample_t TextureIntegerSampleMaxNum;
	Sample_t TextureDepthSampleMaxNum;
	Sample_t TextureStencilSampleMaxNum;
	Sample_t StorageTextureSampleMaxNum;

	// Resource dimensions
	Dim_t Texture1DMaxDim;
	Dim_t Texture2DMaxDim;
	Dim_t Texture3DMaxDim;
	Dim_t TextureArrayMaxDim;
	uint32 TexelBufferMaxDim;

	// Memory
	uint64 DeviceUploadHeapSize; // ReBAR
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
	uint32_t descriptorSetSamplerMaxNum;
	uint32_t descriptorSetConstantBufferMaxNum;
	uint32_t descriptorSetStorageBufferMaxNum;
	uint32_t descriptorSetTextureMaxNum;
	uint32_t descriptorSetStorageTextureMaxNum;

	// Vertex shader
	uint32 VertexShaderAttributeMaxNum;
	uint32 VertexShaderStreamMaxNum;
	uint32 VertexShaderOutputComponentMaxNum;

	// Tessellation shaders
	float TessControlShaderGenerationMaxLevel;
	uint32 TessControlShaderPatchPointMaxNum;
	uint32 TessControlShaderPerVertexInputComponentMaxNum;
	uint32 TessControlShaderPerVertexOutputComponentMaxNum;
	uint32 TessControlShaderPerPatchOutputComponentMaxNum;
	uint32 TessControlShaderTotalOutputComponentMaxNum;
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

	// Ray tracing
	uint64 RayTracingShaderGroupIdentifierSize;
	uint64 RayTracingShaderTableAligment;
	uint64 RayTracingShaderTableMaxStride;
	uint32 RayTracingShaderRecursionMaxDepth;
	uint32 RayTracingGeometryObjectMaxNum;

	// Mesh shaders
	uint32 MeshControlSharedMemoryMaxSize;
	uint32 MeshControlWorkGroupInvocationMaxNum;
	uint32 MeshControlPayloadMaxSize;
	uint32 MeshEvaluationOutputVerticesMaxNum;
	uint32 MeshEvaluationOutputPrimitiveMaxNum;
	uint32 MeshEvaluationOutputComponentMaxNum;
	uint32 MeshEvaluationSharedMemoryMaxSize;
	uint32 MeshEvaluationWorkGroupInvocationMaxNum;

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

	// Features
	uint32 IsComputeQueueSupported : 1;
	uint32 IsCopyQueueSupported : 1;
	uint32 IsTextureFilterMinMaxSupported : 1;
	uint32 IsLogicOpSupported : 1;
	uint32 IsDepthBoundsTestSupported : 1;
	uint32 IsProgrammableSampleLocationsSupported : 1;
	uint32 IsRegisterAliasingSupported : 1;
	uint32 IsFloat16Supported : 1;
	uint32 IsIndependentFrontAndBackStencilReferenceAndMasksSupported : 1;
	uint32 IsLineSmoothingSupported : 1;
	uint32 IsCopyQueueTimestampSupported : 1;
	uint32 IsDispatchRaysIndirectSupported : 1;
	uint32 IsDrawMeshTasksIndirectSupported : 1;
	uint32 IsMeshShaderPipelineStatsSupported : 1;

	// Extensions (unexposed are always supported)
	uint32 IsSwapChainSupported : 1;  // SwapChain
	uint32 IsRayTracingSupported : 1; // RayTracing
	uint32 IsMeshShaderSupported : 1; // MeshShader
	uint32 IsLowLatencySupported : 1; // LowLatency
};

#pragma endregion

RB_NAMESPACE_END