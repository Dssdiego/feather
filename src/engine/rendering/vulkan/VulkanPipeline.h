//
// Created by Diego S. Seabra on 14/08/22.
//

#ifndef FEATHER_VULKANPIPELINE_H
#define FEATHER_VULKANPIPELINE_H

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <vulkan/vulkan.h>
#include <sstream>
#include <string>
#include <map>

#include "VulkanSwapchain.h"
#include "VulkanPipelineBuilder.h"
#include "VulkanDevice.h"
#include "VulkanEngine.h"
#include "../Shader.h"
#include "../shapes/Vertex.h"

namespace Feather
{
	struct PushConstantData
	{
		glm::mat4 transform{ 1.f };
		alignas(32) glm::vec4 color;
	};

	struct VulkanPipelineImpl
	{
		VulkanPipelineImpl();
		~VulkanPipelineImpl();

		// builder
		VulkanPipelineBuilder pipelineBuilder;

		// sets
		std::map<std::string, PipelineSet> pipelineSets = {};
		std::string currentPipeline = "";

		// pipelines
		VkPipeline shapePipeline;
		VkPipelineLayout shapePipelineLayout;
		VkPipeline spritePipeline;
		VkPipelineLayout spritePipelineLayout;
		VkPipeline grayscalePipeline;
		VkPipelineLayout grayscalePipelineLayout;
		VkPipeline boundsPipeline;
		VkPipelineLayout boundsPipelineLayout;
		VkPipeline uiPipeline;
		VkPipelineLayout uiPipelineLayout;
		VkPipeline gridPipeline;
		VkPipelineLayout gridPipelineLayout;
		VkPipeline gridPlanePipeline;
		VkPipelineLayout gridPlanePipelineLayout;

		// Helpers
		void CreatePipelineSets();
		void CreatePipelineLayout(VkPipelineLayout& layout);
		void SwitchToPipeline(const std::string& name);
		PipelineSet GetCurrent();
		void Bind();
	};

	class VulkanPipeline
	{
	public:
		// REVIEW: Make it possible to pass a custom pipeline config at the creation of the pipeline?
		static void Init();
		static void Bind();
		static void Update();
		static void Shutdown();

		static void SwitchToPipeline(const std::string& name);
		static PipelineSet GetCurrent();
	};
}

#endif //FEATHER_VULKANPIPELINE_H
