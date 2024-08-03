#pragma once
#include "RenderHardwareKernelDesc.h"

RB_NAMESPACE_BEGIN

class RB_CORE_API RRenderHardwareKernel
{
public :
	// Get
	virtual const RDeviceDesc& GetDeviceDesc(const RDevice& Device) = 0;
	virtual const RBufferDesc& GetBufferDesc(const RBuffer& Buffer) = 0;
	virtual const RTextureDesc& GetTextureDesc(const RTexture& Texture) = 0;
	virtual EFormatSupportBits GetFormatSupport(const RDevice& Device, EFormat Format) = 0;
	virtual uint32 GetQuerySize(const RQueryPool& QueryPool) = 0;
	virtual void GetBufferMemoryInfo(const RBuffer& Buffer, EMemoryLocation MemoryLocation, RMemoryDesc& MemoryDesc) = 0;
	virtual void GetTextureMemoryInfo(const RTexture& Texture, EMemoryLocation MemoryLocation, RMemoryDesc& MemoryDesc) = 0;
	virtual EResult GetCommandQueue(RDevice& Device, ECommandQueueType CommandQueueType, RCommandQueue*& CommandQueue) = 0;

	// Create
	virtual EResult CreateCommandAllocator(const RCommandQueue& CommandQueue, uint32 PhysicalDeviceMask, RCommandAllocator*& CommandAllocator) = 0;
	virtual EResult CreateCommandBuffer(RCommandAllocator& CommandAllocator, RCommandBuffer*& CommandBuffer) = 0;
	virtual EResult CreateDescriptorPool(RDevice& Device, const RDescriptorPoolDesc& DescriptorPoolDesc, RDescriptorPool*& DescriptorPool) = 0;
	virtual EResult CreateBuffer(RDevice& Device, const RBufferDesc& BufferDesc, RBuffer*& Buffer) = 0;
	virtual EResult CreateTexture(RDevice& Device, const RTextureDesc& TextureDesc, RTexture*& Texture) = 0;
	virtual EResult CreateBufferView(const RBufferViewDesc& BufferViewDesc, RDescriptor*& BufferView) = 0;
	virtual EResult CreateTexture1DView(const RTexture1DViewDesc& TextureViewDesc, RDescriptor*& TextureView) = 0;
	virtual EResult CreateTexture2DView(const RTexture2DViewDesc& TextureViewDesc, RDescriptor*& TextureView) = 0;
	virtual EResult CreateTexture3DView(const RTexture3DViewDesc& TextureViewDesc, RDescriptor*& TextureView) = 0;
	virtual EResult CreateSampler(RDevice& Device, const RSamplerDesc& SamplerDesc, RDescriptor*& Sampler) = 0;
	virtual EResult CreatePipelineLayout(RDevice& Device, const RPipelineLayoutDesc& PipelineLayoutDesc, RPipelineLayout*& PipelineLayout) = 0;
	virtual EResult CreateGraphicsPipeline(RDevice& Device, const RGraphicsPipelineDesc& GraphicsPipelineDesc, RPipeline*& Pipeline) = 0;
	virtual EResult CreateComputePipeline(RDevice& Device, const RComputePipelineDesc& ComputePipelineDesc, RPipeline*& Pipeline) = 0;
	virtual EResult CreateQueryPool(RDevice& Device, const RQueryPoolDesc& QueryPoolDesc, RQueryPool*& QueryPool) = 0;
	virtual EResult CreateFence(RDevice& Device, uint64 initialValue, RFence*& Fence) = 0;

	// Destroy
	virtual void DestroyCommandAllocator(RCommandAllocator& CommandAllocator) = 0;
	virtual void DestroyCommandBuffer(RCommandBuffer& CommandBuffer) = 0;
	virtual void DestroyDescriptorPool(RDescriptorPool& DescriptorPool) = 0;
	virtual void DestroyBuffer(RBuffer& Buffer) = 0;
	virtual void DestroyTexture(RTexture& Texture) = 0;
	virtual void DestroyDescriptor(RDescriptor& Descriptor) = 0;
	virtual void DestroyPipelineLayout(RPipelineLayout& PipelineLayout) = 0;
	virtual void DestroyPipeline(RPipeline& Pipeline) = 0;
	virtual void DestroyQueryPool(RQueryPool& QueryPool) = 0;
	virtual void DestroyFence(RFence& Fence) = 0;

	// Memory
	virtual EResult AllocateMemory(RDevice& Device, uint32 MemoryType, uint64 Size, RMemory*& Memory) = 0;
	virtual EResult BindBufferMemory(RDevice& Device, const RBufferMemoryBindingDesc* MemoryBindingDescs, uint32 MemoryBindingDescNum) = 0;
	virtual EResult BindTextureMemory(RDevice& Device, const RTextureMemoryBindingDesc* MemoryBindingDescs, uint32 MemoryBindingDescNum) = 0;
	virtual void FreeMemory(RMemory& Memory) = 0;

	// Command buffer commands
	virtual EResult BeginCommandBuffer(RCommandBuffer& CommandBuffer, const RDescriptorPool* DescriptorPool) = 0;
	// {
		// Setup
		virtual void CmdSetDescriptorPool(RCommandBuffer& CommandBuffer, const RDescriptorPool& DescriptorPool) = 0;
		virtual void CmdSetDescriptorSet(RCommandBuffer& CommandBuffer, uint32 SetIndexInPipelineLayout, const RDescriptorSet& DescriptorSet, const uint32* DynamicConstantBufferOffsets) = 0;
		virtual void CmdSetPipelineLayout(RCommandBuffer& CommandBuffer, const RPipelineLayout& PipelineLayout) = 0;
		virtual void CmdSetPipeline(RCommandBuffer& CommandBuffer, const RPipeline& Pipeline) = 0;
		virtual void CmdSetConstants(RCommandBuffer& CommandBuffer, uint32 PushConstantIndex, const void* Data, uint32 Size) = 0;

		// Barrier (can be used inside "Graphics" with limited functionality)
		virtual void CmdBarrier(RCommandBuffer& CommandBuffer, const RBarrierGroupDesc& BarrierGroupDesc) = 0;
	
		// Mandatory state, if enabled (can be set only once)
		// Interacts with PSL enabled pipelines. Affects any depth-stencil operations, including clear and copy
		virtual void CmdSetSamplePositions(RCommandBuffer& CommandBuffer, const RSamplePosition* Positions, uint32 PositionNum) = 0;

		// Graphics
		virtual void CmdBeginRenderPass(RCommandBuffer& CommandBuffer, const RAttachmentsDesc& AttachmentsDesc) = 0;
		// {
			// Fast clear
			virtual void CmdClearAttachments(RCommandBuffer& CommandBuffer, const RClearDesc* ClearDescs, uint32 ClearDescNum, const RRect* Rects, uint32 RectNum) = 0;

			// Mandatory state (can be set only once)
			virtual void CmdSetViewports(RCommandBuffer& CommandBuffer, const RViewport* Viewports, uint32 ViewportNum) = 0;
			virtual void CmdSetScissors(RCommandBuffer& CommandBuffer, const RRect* Rects, uint32 RectNum) = 0;

			// Mandatory state, if enabled (can be set only once)
			virtual void CmdSetStencilReference(RCommandBuffer& CommandBuffer, uint8 FrontRef, uint8 BackRef) = 0; // "backRef" requires "isIndependentFrontAndBackStencilReferenceAndMasksSupported"
			virtual void CmdSetDepthBounds(RCommandBuffer& CommandBuffer, float BoundsMin, float BoundsMax) = 0;
			virtual void CmdSetBlendConstants(RCommandBuffer& CommandBuffer, const RColor32f& Color) = 0;

			// Input assembly
			virtual void CmdSetIndexBuffer(RCommandBuffer& CommandBuffer, const RBuffer& Buffer, uint64 Offset, EIndexType IndexType) = 0;
			virtual void CmdSetVertexBuffers(RCommandBuffer& CommandBuffer, uint32 BaseSlot, uint32 BufferNum, const RBuffer* const* Buffers, const uint64* Offsets) = 0;

			// Draw
			virtual void CmdDraw(RCommandBuffer& CommandBuffer, const RDrawDesc& DrawDesc) = 0;
			virtual void CmdDrawIndirect(RCommandBuffer& CommandBuffer, const RBuffer& Buffer, uint64 Offset, uint32 DrawNum, uint32 Stride) = 0; // buffer contains "DrawDesc" commands

			virtual void CmdDrawIndexed(RCommandBuffer& CommandBuffer, const RDrawIndexedDesc& DrawIndexedDesc) = 0;
			virtual void CmdDrawIndexedIndirect(RCommandBuffer& CommandBuffer, const RBuffer& Buffer, uint64 Offset, uint32 DrawNum, uint32 Stride) = 0; // buffer contains "DrawIndexedDesc" commands
		// }
		virtual void CmdEndRendering(RCommandBuffer& CommandBuffer) = 0;

		// Compute
		virtual void CmdDispatch(RCommandBuffer& CommandBuffer, const RDispatchDesc& DispatchDesc) = 0;
		virtual void CmdDispatchIndirect(RCommandBuffer& CommandBuffer, const RBuffer& Buffer, uint64 Offset) = 0; // buffer contains "DispatchDesc" commands

		// Copy
		virtual void CmdCopyBuffer(RCommandBuffer& CommandBuffer, RBuffer& DstBuffer, uint64 DstOffset, const RBuffer& SrcBuffer, uint64 SrcOffset, uint64 Size) = 0;
		virtual void CmdCopyTexture(RCommandBuffer& CommandBuffer, RTexture& DstTexture, const RTextureRegionDesc* DstRegionDesc, const RTexture& SrcTexture, const RTextureRegionDesc* SrcRegionDesc) = 0;
		virtual void CmdUploadBufferToTexture(RCommandBuffer& CommandBuffer, RTexture& DstTexture, const RTextureRegionDesc& DstRegionDesc, const RBuffer& SrcBuffer, const RTextureDataLayoutDesc& SrcDataLayoutDesc) = 0;
		virtual void CmdReadbackTextureToBuffer(RCommandBuffer& CommandBuffer, RBuffer& DstBuffer, RTextureDataLayoutDesc& DstDataLayoutDesc, const RTexture& SrcTexture, const RTextureRegionDesc& SrcRegionDesc) = 0;

		// Clear storage (slow clear)
		virtual void CmdClearStorageBuffer(RCommandBuffer& CommandBuffer, const RClearStorageBufferDesc& ClearDesc) = 0;
		virtual void CmdClearStorageTexture(RCommandBuffer& CommandBuffer, const RClearStorageTextureDesc& ClearDesc) = 0;

		// Query
		virtual void CmdResetQueries(RCommandBuffer& CommandBuffer, const RQueryPool& QueryPool, uint32 Offset, uint32 Num) = 0;
		virtual void CmdBeginQuery(RCommandBuffer& CommandBuffer, const RQueryPool& QueryPool, uint32 Offset) = 0;
		virtual void CmdEndQuery(RCommandBuffer& CommandBuffer, const RQueryPool& QueryPool, uint32 Offset) = 0;
		virtual void CmdCopyQueries(RCommandBuffer& CommandBuffer, const RQueryPool& QueryPool, uint32 Offset, uint32 Num, RBuffer& DstBuffer, uint64 DstOffset) = 0;

		// Annotation
		virtual void CmdBeginAnnotation(RCommandBuffer& CommandBuffer, const char* Name) = 0;
		virtual void CmdEndAnnotation(RCommandBuffer& CommandBuffer) = 0;
	// }
	virtual EResult EndCommandBuffer(RCommandBuffer& CommandBuffer) = 0;

	// Synchronization
	virtual uint64 GetFenceValue(RFence& Fence) = 0;
	virtual void Wait(RFence& Fence, uint64 Value) = 0;

	// Work submission (with queue synchronization)
	virtual void QueueSubmit(RCommandQueue& CommandQueue, const RQueueSubmitDesc& QueueSubmitDesc) = 0;

	// Descriptor set
	virtual void UpdateDescriptorRanges(RDescriptorSet& DescriptorSet, uint32 BaseRange, uint32 RangeNum, const RDescriptorRangeUpdateDesc* RangeUpdateDescs) = 0;
	virtual void UpdateDynamicConstantBuffers(RDescriptorSet& DescriptorSet, uint32 BaseBuffer, uint32 BufferNum, const RDescriptor* const* Descriptors) = 0;
	virtual void CopyDescriptorSet(RDescriptorSet& DescriptorSet, const RDescriptorSetCopyDesc& DescriptorSetCopyDesc) = 0;

	// Descriptor pool
	virtual EResult AllocateDescriptorSets(RDescriptorPool& DescriptorPool, const RPipelineLayout& PipelineLayout, uint32 SetIndexInPipelineLayout, RDescriptorSet** DescriptorSets, uint32 InstanceNum, uint32 VariableDescriptorNum) = 0;
	virtual void ResetDescriptorPool(RDescriptorPool& DescriptorPool) = 0;

	// Command allocator
	virtual void ResetCommandAllocator(RCommandAllocator& CommandAllocator) = 0;

	// Map / Unmap
	virtual void* MapBuffer(RBuffer& Buffer, uint64 Offset, uint64 Size) = 0;
	virtual void UnmapBuffer(RBuffer& Buffer) = 0;

	// Debug name
	virtual void SetDeviceDebugName(RDevice& Device, const char* Name) = 0;
	virtual void SetCommandQueueDebugName(RCommandQueue& CommandQueue, const char* Name) = 0;
	virtual void SetFenceDebugName(RFence& Fence, const char* Name) = 0;
	virtual void SetCommandAllocatorDebugName(RCommandAllocator& CommandAllocator, const char* Name) = 0;
	virtual void SetDescriptorPoolDebugName(RDescriptorPool& DescriptorPool, const char* Name) = 0;
	virtual void SetBufferDebugName(RBuffer& Buffer, const char* Name) = 0;
	virtual void SetTextureDebugName(RTexture& Texture, const char* Name) = 0;
	virtual void SetDescriptorDebugName(RDescriptor& Descriptor, const char* Name) = 0;
	virtual void SetPipelineLayoutDebugName(RPipelineLayout& PipelineLayout, const char* Name) = 0;
	virtual void SetPipelineDebugName(RPipeline& Pipeline, const char* Name) = 0;
	virtual void SetQueryPoolDebugName(RQueryPool& QueryPool, const char* Name) = 0;
	virtual void SetDescriptorSetDebugName(RDescriptorSet& DescriptorSet, const char* Name) = 0;
	virtual void SetCommandBufferDebugName(RCommandBuffer& CommandBuffer, const char* Name) = 0;
	virtual void SetMemoryDebugName(RMemory& Memory, const char* Name) = 0;

	// Native objects
	virtual void* GetDeviceNativeObject(const RDevice& Device) = 0; // ID3D11Device*, ID3D12Device* or VkDevice
	virtual void* GetCommandBufferNativeObject(const RCommandBuffer& CommandBuffer) = 0; // ID3D11DeviceContext*, ID3D12GraphicsCommandList* or VkCommandBuffer
	virtual uint64 GetBufferNativeObject(const RBuffer& Buffer) = 0; // ID3D11Buffer*, ID3D12Resource* or VkBuffer
	virtual uint64 GetTextureNativeObject(const RTexture& Texture) = 0; // ID3D11Resource*, ID3D12Resource* or VkImage
	virtual uint64 GetDescriptorNativeObject(const RDescriptor& Descriptor) = 0; // ID3D11View*, D3D12_CPU_DESCRIPTOR_HANDLE or VkImageView/VkBufferView
};

RB_NAMESPACE_END