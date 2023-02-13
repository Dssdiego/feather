//
// Created by Diego S. Seabra on 21/07/22.
//

#include "VulkanEngine.h"
#include "descriptors/VulkanDescriptorWriter.h"
#include "../SpriteAtlas.h"
#include "../../common/Assets.h"
#include <Tracy.hpp>
#include "../Samplers.h"
#include "../ImGuiRenderer.h"

namespace Feather
{
	// TODO: Refactor the code so that we don't use raw pointers. Instead we want to use smart pointers
	//       See more here: https://stackoverflow.com/questions/106508/what-is-a-smart-pointer-and-when-should-i-use-one
	EngineRendererImpl* mEngineRendererImpl = nullptr;

	//
	// Initialization/Destruction
	//

	void VulkanEngine::Init()
	{
		mEngineRendererImpl = new EngineRendererImpl;
	}

	void VulkanEngine::Shutdown()
	{
		delete mEngineRendererImpl;
	}

	//
	// External
	//

	VkCommandBuffer VulkanEngine::GetCurrentCommandBuffer()
	{
		return mEngineRendererImpl->GetCurrentCommandBuffer();
	}

	std::vector<VkCommandBuffer>& VulkanEngine::GetCommandBuffers()
	{
		return mEngineRendererImpl->GetCommandBuffers();
	}

	std::vector<VkCommandBuffer> VulkanEngine::CreateCommandBuffers(VkCommandPool commandPool)
	{
		return mEngineRendererImpl->CreateCommandBuffers(commandPool);
	}

	std::vector<VkCommandBuffer>& VulkanEngine::GetSubmitCommandBuffers()
	{
		return mEngineRendererImpl->GetSubmitCommandBuffers();
	}

	VkCommandBuffer VulkanEngine::BeginFrame()
	{
		return mEngineRendererImpl->BeginFrame();
	}

	void VulkanEngine::EndFrame()
	{
		mEngineRendererImpl->EndFrame();
	}

	int VulkanEngine::GetFrameIndex()
	{
		return mEngineRendererImpl->GetFrameIndex();
	}

	void VulkanEngine::FreeCommandBuffers(std::vector<VkCommandBuffer>& buffers)
	{
		mEngineRendererImpl->FreeCommandBuffers(buffers);
	}

	void VulkanEngine::UpdateUniformBuffer(UniformBufferObject& ubo)
	{
		mEngineRendererImpl->UpdateUniformBuffer(ubo);
	}

	VkDescriptorSetLayout VulkanEngine::GetDescriptorSetLayout()
	{
		return mEngineRendererImpl->descriptorSetLayout->GetDescriptorSetLayout();
	}

	std::vector<VkDescriptorSet> VulkanEngine::GetDescriptorSets()
	{
		return mEngineRendererImpl->descriptorSets;
	}

	//
	// Implementation
	//

	EngineRendererImpl::EngineRendererImpl()
	{
		Logger::Info("Initializing engine renderer");

		VulkanDevice::Init();
		VulkanSwapchain::Init();

		Samplers::Init();
		Assets::Init();

		// REVIEW: Recreate swapchain on renderer init?

		// global stuff
		commandBuffers = CreateCommandBuffers(VulkanDevice::GetCommandPool());
		CreateUniformBuffers();
		CreateDescriptors();
	}

	EngineRendererImpl::~EngineRendererImpl()
	{
		Logger::Info("Shutting down engine renderer");

		// waiting for the device to finish operations before exiting and destroying stuff
		vkDeviceWaitIdle(VulkanDevice::GetDevice());

		// forcing descriptors unique_ptr's to be destroyed | FIXME: We shouldn't have to do this manually!
		descriptorPool = nullptr;
		descriptorSetLayout = nullptr;

		// forcing the uniform buffers unique ptr to be destroyed | FIXME: We shouldn't have to do this manually!
		uint32_t swapChainImageCount = VulkanSwapchain::GetImageCount();
		for (size_t i = 0; i < swapChainImageCount; i++)
		{
			uniformBuffers[i]->Destroy();
			uniformBuffers[i] = nullptr;
		}
		FreeCommandBuffers(commandBuffers);

		Samplers::Shutdown();
		Assets::Shutdown();

		VulkanSwapchain::Shutdown();
		VulkanDevice::Shutdown();
	}

	void EngineRendererImpl::CreateDescriptors()
	{
		descriptorPool = VulkanDescriptorPool::Builder()
			.SetMaxSets(VulkanSwapchain::GetNumberOfFramesInFlight())
			.AddPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VulkanSwapchain::GetNumberOfFramesInFlight())
			.AddPoolSize(VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, Assets::MAX_TEXTURES)
			.AddPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VulkanSwapchain::GetNumberOfFramesInFlight())
			.Build();

		descriptorSetLayout = VulkanDescriptorSetLayout::Builder()
			.AddBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT)
			.AddBinding(1, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, VK_SHADER_STAGE_FRAGMENT_BIT, Assets::MAX_TEXTURES)
			.AddBinding(2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
			.Build();

		descriptorSets.resize(VulkanSwapchain::GetNumberOfFramesInFlight());

		for (int i = 0; i < descriptorSets.size(); ++i)
		{
			// uniform buffer
			auto bufferInfo = uniformBuffers[i]->DescriptorInfo();

			// image sampler
			VulkanDescriptorWriter(*descriptorSetLayout, *descriptorPool) // unique_ptr, accessing contents
				.WriteBuffer(0, &bufferInfo)
				.WriteImageArray(1, Assets::GetDescriptors())
				//.WriteImage(2, &Assets::GetDefaultTexture()->DescriptorInfo())
				.Build(descriptorSets[i]);
		}
	}

	void EngineRendererImpl::CreateUniformBuffers()
	{
		VkDeviceSize bufferSize = sizeof(UniformBufferObject);
		uint32_t swapChainImageCount = VulkanSwapchain::GetImageCount();

		uniformBuffers.resize(swapChainImageCount);

		for (size_t i = 0; i < swapChainImageCount; i++)
		{
			uniformBuffers[i] = std::make_unique<VulkanBuffer>(
				sizeof(UniformBufferObject),
				1,
				VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
				);
			uniformBuffers[i]->Map();
		}
	}

	std::vector<VkCommandBuffer> EngineRendererImpl::CreateCommandBuffers(VkCommandPool commandPool)
	{
		std::vector<VkCommandBuffer> buffers{};
		buffers.resize(VulkanSwapchain::GetNumberOfFramesInFlight());

		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = commandPool;
		allocInfo.commandBufferCount = static_cast<uint32_t>(buffers.size());

		VK_CHECK(vkAllocateCommandBuffers(VulkanDevice::GetDevice(), &allocInfo, buffers.data()));

		Logger::Debug("Created command buffers");

		return buffers;
	}

	void EngineRendererImpl::FreeCommandBuffers(std::vector<VkCommandBuffer>& buffers)
	{
		vkFreeCommandBuffers(
			VulkanDevice::GetDevice(),
			VulkanDevice::GetCommandPool(),
			static_cast<uint32_t>(buffers.size()),
			buffers.data());
		buffers.clear();
	}

	VkCommandBuffer EngineRendererImpl::GetCurrentCommandBuffer()
	{
		assert(!commandBuffers.empty() && "There must be at least one command buffer present!");
		assert(frameHasStarted && "Cannot get command buffer when frame is not in progress!");

		// REVIEW: Assert the frame is already started? We can't get command buffer when a frame is not in progress!
		return commandBuffers[currentFrameIndex];
	}


	std::vector<VkCommandBuffer>& EngineRendererImpl::GetCommandBuffers()
	{
		return commandBuffers;
	}

	std::vector<VkCommandBuffer>& EngineRendererImpl::GetSubmitCommandBuffers()
	{
		// filling the submit submit command buffer right before returning it
		// using two command buffers for now: one for drawing and the other for imgui
		// REVIEW: Probably there is a better way to do this!
		submitCommandBuffers.clear();
		// TODO: Make a command buffer just for gizmo stuff?
		AddCommandBufferToSubmit(commandBuffers[currentFrameIndex]);
		AddCommandBufferToSubmit(ImGuiRenderer::GetCurrentCommandBuffer());

		return submitCommandBuffers;
	}

	void EngineRendererImpl::AddCommandBufferToSubmit(VkCommandBuffer buffer)
	{
		submitCommandBuffers.push_back(buffer);
	}

	int EngineRendererImpl::GetFrameIndex()
	{
		assert(frameHasStarted && "Cannot get frame index when frame is not in progress");
		return currentFrameIndex;
	}

	VkCommandBuffer EngineRendererImpl::BeginFrame()
	{
		ZoneScopedC();

		assert(!frameHasStarted && "Can't call BeginFrame() while a frame is already in progress");

		auto result = VulkanSwapchain::AcquireNextImage(&currentImageIdx);
		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			VulkanSwapchain::Recreate();
			return nullptr;
		}
		else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		{
			// REVIEW: Should this be a runtime error?
			Logger::Error("Failed to acquire swap chain image", "");
			throw std::runtime_error("failed to acquire swap chain image");
		}

		frameHasStarted = true;

		auto commandBuffer = GetCurrentCommandBuffer();
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		// REVIEW: Which flag to use here? Do we actually need a flag?
	//    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	//    VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT specifies that each recording of the command buffer will only be submitted once, and the command buffer will be reset and recorded again between each submission.
	//    VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT specifies that a secondary command buffer is considered to be entirely inside a render pass. If this is a primary command buffer, then this bit is ignored.
	//    VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT specifies that a command buffer can be resubmitted to a queue while it is in the pending state, and recorded into multiple primary command buffers.

		VK_CHECK(vkBeginCommandBuffer(commandBuffer, &beginInfo));

		// TODO: Document
		BeginSwapChainRenderPass();

		return commandBuffer;
	}

	void EngineRendererImpl::BeginSwapChainRenderPass()
	{
		assert(frameHasStarted && "Can't call BeginSwapChainRenderPass if frame is not in progreess");

		auto commandBuffer = GetCurrentCommandBuffer();

		VkRenderPassBeginInfo renderPassBeginInfo{};
		renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassBeginInfo.renderPass = VulkanSwapchain::GetRenderPass();
		renderPassBeginInfo.framebuffer = VulkanSwapchain::GetFrameBuffer(currentImageIdx);
		renderPassBeginInfo.renderArea.offset = { 0, 0 };
		renderPassBeginInfo.renderArea.extent = VulkanSwapchain::GetSwapChainExtent();

		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = Color::GetAsVulkanClear(Color::EngineClear);
		clearValues[1].depthStencil = { 1.0f, 0 };

		renderPassBeginInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassBeginInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(VulkanSwapchain::GetSwapChainExtent().width);
		viewport.height = static_cast<float>(VulkanSwapchain::GetSwapChainExtent().height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = VulkanSwapchain::GetSwapChainExtent();

		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
	}

	void EngineRendererImpl::EndSwapChainRenderPass()
	{
		assert(frameHasStarted && "Can't call EndSwapChainRenderPass if frame is not in progress");

		auto commandBuffer = GetCurrentCommandBuffer();
		vkCmdEndRenderPass(commandBuffer);
	}

	void EngineRendererImpl::EndFrame()
	{
		ZoneScopedC();

		// TODO: Document
		EndSwapChainRenderPass();

		assert(frameHasStarted && "Can't call EndFrame() while frame is not in progress");

		// REVIEW: End all CURRENT command buffers?
		VK_CHECK(vkEndCommandBuffer(GetCurrentCommandBuffer()));

		auto result = VulkanSwapchain::SubmitCommandBuffers(&currentImageIdx);
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
		{
			VulkanSwapchain::Recreate();
		}
		else if (result != VK_SUCCESS)
		{
			std::string err = "Failed to present swap chain image";

			Logger::Error(err, "");
			throw std::runtime_error(err);
		}

		// frame has ended
		frameHasStarted = false;

		// updating the current frame index (for the command buffers)
		currentFrameIndex = (currentFrameIndex + 1) % VulkanSwapchain::GetNumberOfFramesInFlight();
	}

	void EngineRendererImpl::UpdateUniformBuffer(UniformBufferObject& ubo)
	{
		ZoneScopedC(0x3498db);

		uniformBuffers[currentFrameIndex]->WriteToBuffer(&ubo);
		// NOTE: we don't need to flush at a given index because of the "COHERENT_HOST_BIT" in the buffer flag
	//    uniformBuffers[currentFrameIndex]->Flush();
	//    globalUboBuffer->FlushIndex(currentFrameIndex);
	}
}

