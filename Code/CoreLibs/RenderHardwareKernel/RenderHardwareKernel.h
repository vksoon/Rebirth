#pragma once
#include "RenderHardwareKernelDesc.h"

RB_NAMESPACE_BEGIN_EX(RHK)

class RB_CORE_API RRenderHardwareKernel
{
public :

	// Get
	virtual const RDeviceDesc& GetDeviceDesc(const RDevice& Device) = 0;
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
	virtual EResult CreateFrameBuffer(RDevice& Device, const RFrameBufferDesc& FrameBufferDesc, RFrameBuffer*& FrameBuffer) = 0;
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
	virtual void DestroyFrameBuffer(RFrameBuffer& FrameBuffer) = 0;
	virtual void DestroyQueryPool(RQueryPool& QueryPool) = 0;
	virtual void DestroyFence(RFence& Fence) = 0;

	// Memory
	virtual EResult AllocateMemory(RDevice& Device, uint32 PhysicalDeviceMask, RMemoryType& MemoryType, uint64 Size, RMemory*& Memory) = 0;
	virtual EResult BindBufferMemory(RDevice& Device, const RBufferMemoryBindingDesc*& MemoryBindingDescs, uint32 MemoryBindingDescNum) = 0;
	virtual EResult BindTextureMemory(RDevice& Device, const RTextureMemoryBindingDesc*& MemoryBindingDescs, uint32 MemoryBindingDescNum) = 0;
	virtual void FreeMemory(RMemory& Memory) = 0;

	// Command buffer commands
	virtual EResult BeginCommandBuffer(RCommandBuffer& CommandBuffer, const RDescriptorPool* DescriptorPool, uint32 PhysicalDeviceIndex) = 0;
	virtual EResult EndCommandBuffer(RCommandBuffer& CommandBuffer) = 0;

	virtual void CmdSetPipeline(RCommandBuffer& CommandBuffer, const RPipeline& Pipeline) = 0;
	virtual void CmdSetPipelineLayout(RCommandBuffer& CommandBuffer, const RPipelineLayout& PipelineLayout) = 0; 
	void (RB_CALL* CmdSetDescriptorSet)(NRI_REF_NAME(CommandBuffer) commandBuffer, uint32_t setIndexInPipelineLayout, const NRI_REF_NAME(DescriptorSet) descriptorSet, const uint32_t* dynamicConstantBufferOffsets);
	void (RB_CALL* CmdSetConstants)(NRI_REF_NAME(CommandBuffer) commandBuffer, uint32_t pushConstantIndex, const void* data, uint32_t size);
	void (RB_CALL* CmdSetDescriptorPool)(NRI_REF_NAME(CommandBuffer) commandBuffer, const NRI_REF_NAME(DescriptorPool) descriptorPool);
	void (RB_CALL* CmdPipelineBarrier)(NRI_REF_NAME(CommandBuffer) commandBuffer, const NRI_NAME(TransitionBarrierDesc)* transitionBarriers, const NRI_NAME(AliasingBarrierDesc)* aliasingBarriers, NRI_NAME(BarrierDependency) dependency);

	void (RB_CALL* CmdBeginRenderPass)(NRI_REF_NAME(CommandBuffer) commandBuffer, const NRI_REF_NAME(FrameBuffer) frameBuffer, NRI_NAME(RenderPassBeginFlag) renderPassBeginFlag);
	void (RB_CALL* CmdEndRenderPass)(NRI_REF_NAME(CommandBuffer) commandBuffer);
	void (RB_CALL* CmdSetViewports)(NRI_REF_NAME(CommandBuffer) commandBuffer, const NRI_NAME(Viewport)* viewports, uint32_t viewportNum);
	void (RB_CALL* CmdSetScissors)(NRI_REF_NAME(CommandBuffer) commandBuffer, const NRI_NAME(Rect)* rects, uint32_t rectNum);
	void (RB_CALL* CmdSetDepthBounds)(NRI_REF_NAME(CommandBuffer) commandBuffer, float boundsMin, float boundsMax);
	void (RB_CALL* CmdSetStencilReference)(NRI_REF_NAME(CommandBuffer) commandBuffer, uint8_t reference);
	void (RB_CALL* CmdSetSamplePositions)(NRI_REF_NAME(CommandBuffer) commandBuffer, const NRI_NAME(SamplePosition)* positions, uint32_t positionNum);
	void (RB_CALL* CmdClearAttachments)(NRI_REF_NAME(CommandBuffer) commandBuffer, const NRI_NAME(ClearDesc)* clearDescs, uint32_t clearDescNum, const NRI_NAME(Rect)* rects, uint32_t rectNum);
	void (RB_CALL* CmdSetIndexBuffer)(NRI_REF_NAME(CommandBuffer) commandBuffer, const NRI_REF_NAME(Buffer) buffer, uint64_t offset, NRI_NAME(IndexType) indexType);
	void (RB_CALL* CmdSetVertexBuffers)(NRI_REF_NAME(CommandBuffer) commandBuffer, uint32_t baseSlot, uint32_t bufferNum, const NRI_NAME(Buffer)* const* buffers, const uint64_t* offsets);

	void (RB_CALL* CmdDraw)(NRI_REF_NAME(CommandBuffer) commandBuffer, uint32_t vertexNum, uint32_t instanceNum, uint32_t baseVertex, uint32_t baseInstance);
	void (RB_CALL* CmdDrawIndexed)(NRI_REF_NAME(CommandBuffer) commandBuffer, uint32_t indexNum, uint32_t instanceNum, uint32_t baseIndex, uint32_t baseVertex, uint32_t baseInstance);
	void (RB_CALL* CmdDrawIndirect)(NRI_REF_NAME(CommandBuffer) commandBuffer, const NRI_REF_NAME(Buffer) buffer, uint64_t offset, uint32_t drawNum, uint32_t stride);
	void (RB_CALL* CmdDrawIndexedIndirect)(NRI_REF_NAME(CommandBuffer) commandBuffer, const NRI_REF_NAME(Buffer) buffer, uint64_t offset, uint32_t drawNum, uint32_t stride);
	void (RB_CALL* CmdDispatch)(NRI_REF_NAME(CommandBuffer) commandBuffer, uint32_t x, uint32_t y, uint32_t z);
	void (RB_CALL* CmdDispatchIndirect)(NRI_REF_NAME(CommandBuffer) commandBuffer, const NRI_REF_NAME(Buffer) buffer, uint64_t offset);
	void (RB_CALL* CmdBeginQuery)(NRI_REF_NAME(CommandBuffer) commandBuffer, const NRI_REF_NAME(QueryPool) queryPool, uint32_t offset);
	void (RB_CALL* CmdEndQuery)(NRI_REF_NAME(CommandBuffer) commandBuffer, const NRI_REF_NAME(QueryPool) queryPool, uint32_t offset);
	void (RB_CALL* CmdBeginAnnotation)(NRI_REF_NAME(CommandBuffer) commandBuffer, const char* name);
	void (RB_CALL* CmdEndAnnotation)(NRI_REF_NAME(CommandBuffer) commandBuffer);

	void (RB_CALL* CmdClearStorageBuffer)(NRI_REF_NAME(CommandBuffer) commandBuffer, const NRI_REF_NAME(ClearStorageBufferDesc) clearDesc);
	void (RB_CALL* CmdClearStorageTexture)(NRI_REF_NAME(CommandBuffer) commandBuffer, const NRI_REF_NAME(ClearStorageTextureDesc) clearDesc);
	void (RB_CALL* CmdCopyBuffer)(NRI_REF_NAME(CommandBuffer) commandBuffer, NRI_REF_NAME(Buffer) dstBuffer, uint32_t dstPhysicalDeviceIndex, uint64_t dstOffset, const NRI_REF_NAME(Buffer) srcBuffer, uint32_t srcPhysicalDeviceIndex, uint64_t srcOffset, uint64_t size);
	void (RB_CALL* CmdCopyTexture)(NRI_REF_NAME(CommandBuffer) commandBuffer, NRI_REF_NAME(Texture) dstTexture, uint32_t dstPhysicalDeviceIndex, const NRI_NAME(TextureRegionDesc)* dstRegionDesc, const NRI_REF_NAME(Texture) srcTexture, uint32_t srcPhysicalDeviceIndex, const NRI_NAME(TextureRegionDesc)* srcRegionDesc);
	void (RB_CALL* CmdUploadBufferToTexture)(NRI_REF_NAME(CommandBuffer) commandBuffer, NRI_REF_NAME(Texture) dstTexture, const NRI_REF_NAME(TextureRegionDesc) dstRegionDesc, const NRI_REF_NAME(Buffer) srcBuffer, const NRI_REF_NAME(TextureDataLayoutDesc) srcDataLayoutDesc);
	void (RB_CALL* CmdReadbackTextureToBuffer)(NRI_REF_NAME(CommandBuffer) commandBuffer, NRI_REF_NAME(Buffer) dstBuffer, NRI_REF_NAME(TextureDataLayoutDesc) dstDataLayoutDesc, const NRI_REF_NAME(Texture) srcTexture, const NRI_REF_NAME(TextureRegionDesc) srcRegionDesc);
	void (RB_CALL* CmdCopyQueries)(NRI_REF_NAME(CommandBuffer) commandBuffer, const NRI_REF_NAME(QueryPool) queryPool, uint32_t offset, uint32_t num, NRI_REF_NAME(Buffer) dstBuffer, uint64_t dstOffset);
	void (RB_CALL* CmdResetQueries)(NRI_REF_NAME(CommandBuffer) commandBuffer, const NRI_REF_NAME(QueryPool) queryPool, uint32_t offset, uint32_t num);

	// Synchronization
	uint64 (RB_CALL* GetFenceValue)(NRI_REF_NAME(Fence) fence);
	void (RB_CALL* QueueSignal)(NRI_REF_NAME(CommandQueue) commandQueue, NRI_REF_NAME(Fence) fence, uint64_t value);
	void (RB_CALL* QueueWait)(NRI_REF_NAME(CommandQueue) commandQueue, NRI_REF_NAME(Fence) fence, uint64_t value);
	void (RB_CALL* Wait)(NRI_REF_NAME(Fence) fence, uint64_t value);

	// Work submission
	void (RB_CALL* QueueSubmit)(NRI_REF_NAME(CommandQueue) commandQueue, const NRI_REF_NAME(QueueSubmitDesc) queueSubmitDesc);

	// Descriptor set
	void (RB_CALL* UpdateDescriptorRanges)(NRI_REF_NAME(DescriptorSet) descriptorSet, uint32_t physicalDeviceMask, uint32_t baseRange, uint32_t rangeNum, const NRI_NAME(DescriptorRangeUpdateDesc)* rangeUpdateDescs);
	void (RB_CALL* UpdateDynamicConstantBuffers)(NRI_REF_NAME(DescriptorSet) descriptorSet, uint32_t physicalDeviceMask, uint32_t baseBuffer, uint32_t bufferNum, const NRI_NAME(Descriptor)* const* descriptors);
	void (RB_CALL* CopyDescriptorSet)(NRI_REF_NAME(DescriptorSet) descriptorSet, const NRI_REF_NAME(DescriptorSetCopyDesc) descriptorSetCopyDesc);

	// Descriptor pool
	EResult (RB_CALL* AllocateDescriptorSets)(NRI_REF_NAME(DescriptorPool) descriptorPool, const NRI_REF_NAME(PipelineLayout) pipelineLayout, uint32_t setIndexInPipelineLayout, NRI_NAME(DescriptorSet)** descriptorSets, uint32_t instanceNum, uint32_t physicalDeviceMask, uint32_t variableDescriptorNum);
	void (RB_CALL* ResetDescriptorPool)(NRI_REF_NAME(DescriptorPool) descriptorPool);

	// Command allocator
	void (RB_CALL* ResetCommandAllocator)(NRI_REF_NAME(CommandAllocator) commandAllocator);

	// Map / Unmap
	void* (RB_CALL* MapBuffer)(NRI_REF_NAME(Buffer) buffer, uint64_t offset, uint64_t size);
	void (RB_CALL* UnmapBuffer)(NRI_REF_NAME(Buffer) buffer);

	// Debug name
	void (RB_CALL* SetDeviceDebugName)(NRI_REF_NAME(Device) device, const char* name);
	void (RB_CALL* SetCommandQueueDebugName)(NRI_REF_NAME(CommandQueue) commandQueue, const char* name);
	void (RB_CALL* SetFenceDebugName)(NRI_REF_NAME(Fence) fence, const char* name);
	void (RB_CALL* SetCommandAllocatorDebugName)(NRI_REF_NAME(CommandAllocator) commandAllocator, const char* name);
	void (RB_CALL* SetDescriptorPoolDebugName)(NRI_REF_NAME(DescriptorPool) descriptorPool, const char* name);
	void (RB_CALL* SetBufferDebugName)(NRI_REF_NAME(Buffer) buffer, const char* name);
	void (RB_CALL* SetTextureDebugName)(NRI_REF_NAME(Texture) texture, const char* name);
	void (RB_CALL* SetDescriptorDebugName)(NRI_REF_NAME(Descriptor) descriptor, const char* name);
	void (RB_CALL* SetPipelineLayoutDebugName)(NRI_REF_NAME(PipelineLayout) pipelineLayout, const char* name);
	void (RB_CALL* SetPipelineDebugName)(NRI_REF_NAME(Pipeline) pipeline, const char* name);
	void (RB_CALL* SetFrameBufferDebugName)(NRI_REF_NAME(FrameBuffer) frameBuffer, const char* name);
	void (RB_CALL* SetQueryPoolDebugName)(NRI_REF_NAME(QueryPool) queryPool, const char* name);
	void (RB_CALL* SetDescriptorSetDebugName)(NRI_REF_NAME(DescriptorSet) descriptorSet, const char* name);
	void (RB_CALL* SetCommandBufferDebugName)(NRI_REF_NAME(CommandBuffer) commandBuffer, const char* name);
	void (RB_CALL* SetMemoryDebugName)(NRI_REF_NAME(Memory) memory, const char* name);

	// Native objects
	void*  (RB_CALL* GetDeviceNativeObject)(const RDevice& device); // ID3D11Device*, ID3D12Device* or VkDevice
	void*  (RB_CALL* GetCommandBufferNativeObject)(const RCommandBuffer& commandBuffer); // ID3D11DeviceContext*, ID3D12GraphicsCommandList* or VkCommandBuffer
	uint64 (RB_CALL* GetBufferNativeObject)(const RBuffer& buffer, uint32 physicalDeviceIndex); // ID3D11Buffer*, ID3D12Resource* or VkBuffer
	uint64 (RB_CALL* GetTextureNativeObject)(const RTexture& texture, uint32 physicalDeviceIndex); // ID3D11Resource*, ID3D12Resource* or VkImage
	uint64 (RB_CALL* GetDescriptorNativeObject)(const RDescriptor& descriptor, uint32 physicalDeviceIndex); // ID3D11View*, D3D12_CPU_DESCRIPTOR_HANDLE or VkImageView/VkBufferView
};

RB_NAMESPACE_END_EX