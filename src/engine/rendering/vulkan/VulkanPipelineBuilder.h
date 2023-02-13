//
// Created by Diego S. Seabra on 27/08/22.
//

#ifndef FEATHER_VULKANPIPELINEBUILDER_H
#define FEATHER_VULKANPIPELINEBUILDER_H

#include <vulkan/vulkan.h>
#include <vector>
#include "../Shader.h"
#include "VulkanCommon.h"
#include "VulkanSwapchain.h"
#include "../shapes/Vertex.h"

namespace Feather
{
	struct PipelineBuilderConfig
	{
		//    std::string vertexShaderFilePath;
		//    std::string fragmentShaderFilePath;
		VkPipelineLayout pipelineLayout;
		VkPrimitiveTopology topology;
		VkPolygonMode polygonMode;
		VkBool32 enableBlending;
	};

	class VulkanPipelineBuilder
	{
	public:
		//    VkPipeline Build(PipelineBuilderConfig pipelineBuilderConfig);
		void Build(PipelineSet set);
		void Cleanup();

	private:
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
		VkPipelineViewportStateCreateInfo viewportInfo;
		VkPipelineRasterizationStateCreateInfo rasterizationInfo;
		VkPipelineMultisampleStateCreateInfo multisampleInfo;
		VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
		VkPipelineColorBlendAttachmentState colorBlendAttachment;
		VkPipelineColorBlendStateCreateInfo colorBlendInfo;
		std::vector<VkDynamicState> dynamicStateEnables;
		VkPipelineDynamicStateCreateInfo dynamicStateInfo;
		VkRenderPass renderPass = nullptr;
		uint32_t subpass = 0;

		VkShaderModule vertShaderModule;
		VkShaderModule fragShaderModule;

		// Helpers
		void CreateShaderModule(const std::vector<char>& shaderCode, VkShaderModule* shaderModule);
	};
}

#endif //FEATHER_VULKANPIPELINEBUILDER_H
