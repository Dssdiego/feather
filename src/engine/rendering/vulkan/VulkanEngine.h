//
// Created by Diego S. Seabra on 21/07/22.
//

#ifndef FEATHER_VULKANENGINE_H
#define FEATHER_VULKANENGINE_H

#include <cassert>
#include <vulkan/vulkan.h>
#include <glm/glm.hpp>
#include "VulkanSwapchain.h"
#include "VulkanDevice.h"
#include "../../common/Color.h"
#include "VulkanBuffer.h"
#include "descriptors/VulkanDescriptorPool.h"
#include "descriptors/VulkanDescriptorSetLayout.h"
#include "../Texture.h"

namespace Feather
{
	struct UniformBufferObject
	{
		//    glm::mat4 projectView{1.f};
		alignas(16) glm::mat4 mvp;
		//    alignas(16) glm::mat4 model;
		//    alignas(16) glm::mat4 view;
		//    alignas(16) glm::mat4 projection;
	};

	struct EngineRendererImpl
	{
		EngineRendererImpl();
		~EngineRendererImpl();

		std::vector<VkCommandBuffer> commandBuffers;
		std::vector<VkCommandBuffer> submitCommandBuffers;

		uint32_t currentImageIdx{ 0 };
		int currentFrameIndex{ 0 };
		bool frameHasStarted{ false };

		std::unique_ptr<VulkanDescriptorPool> descriptorPool; // global descriptor pool
		std::unique_ptr<VulkanDescriptorSetLayout> descriptorSetLayout; // global descriptor set layout
		std::vector<std::unique_ptr<VulkanBuffer>> uniformBuffers; // global uniform buffers
		std::vector<VkDescriptorSet> descriptorSets; // global descriptor sets

		VkCommandBuffer GetCurrentCommandBuffer();
		std::vector<VkCommandBuffer>& GetCommandBuffers();
		std::vector<VkCommandBuffer>& GetSubmitCommandBuffers();
		void AddCommandBufferToSubmit(VkCommandBuffer buffer);
		int GetFrameIndex();

		void CreateUniformBuffers();
		void UpdateUniformBuffer(UniformBufferObject& ubo);

		std::vector<VkCommandBuffer> CreateCommandBuffers(VkCommandPool commandPool);
		void FreeCommandBuffers(std::vector<VkCommandBuffer>& buffers);

		void CreateDescriptors();

		VkCommandBuffer BeginFrame();
		void BeginSwapChainRenderPass();
		void EndSwapChainRenderPass();
		void EndFrame();
	};

	class VulkanEngine
	{
	public:
		static void Init();
		static void Shutdown();

		static VkDescriptorSetLayout GetDescriptorSetLayout();
		static std::vector<VkDescriptorSet> GetDescriptorSets();

		static VkCommandBuffer GetCurrentCommandBuffer();
		static std::vector<VkCommandBuffer>& GetCommandBuffers();
		static std::vector<VkCommandBuffer>& GetSubmitCommandBuffers();

		static std::vector<VkCommandBuffer> CreateCommandBuffers(VkCommandPool commandPool);
		static void FreeCommandBuffers(std::vector<VkCommandBuffer>& buffers);

		static int GetFrameIndex();
		static VkCommandBuffer BeginFrame();
		static void EndFrame();

		static void UpdateUniformBuffer(UniformBufferObject& ubo);
	};
}

#endif //FEATHER_VULKANENGINE_H
