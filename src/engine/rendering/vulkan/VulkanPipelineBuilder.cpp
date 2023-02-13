//
// Created by Diego S. Seabra on 27/08/22.
//

#include "VulkanPipelineBuilder.h"

namespace Feather
{
	//VkPipeline VulkanPipelineBuilder::Build(PipelineBuilderConfig pipelineBuilderConfig)
	void VulkanPipelineBuilder::Build(PipelineSet pipelineSet)
	{
		// SECTION: Shaders
		auto vertShaderCode = Shader::ReadFile(pipelineSet.vertexShader);
		auto fragShaderCode = Shader::ReadFile(pipelineSet.fragmentShader);

		CreateShaderModule(vertShaderCode, &vertShaderModule);
		CreateShaderModule(fragShaderCode, &fragShaderModule);

		// assigning vertex shader to the pipeline stage
		VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
		vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
		vertShaderStageInfo.module = vertShaderModule;
		vertShaderStageInfo.pName = "main"; // this allows us to combine multiple shaders in a single shader module (using multiple entry points)

		// assigning fragment shader to the pipeline stage
		VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
		fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		fragShaderStageInfo.module = fragShaderModule;
		fragShaderStageInfo.pName = "main";

		VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

		// SECTION: Vertex Input
		VkPipelineVertexInputStateCreateInfo vertexInputInfo{}; // type of vertex data that will be passed to the vertex shader
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

		// REVIEW: Vertex information should come from "model" information/class?
		auto bindingDescription = Vertex::GetBindingDescription();
		auto attributeDescriptions = Vertex::GetAttributeDescriptions();

		// bindings: spacing between data and whether the data is per-vertex or per-instance
		vertexInputInfo.vertexBindingDescriptionCount = 1;
		vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;

		// attribute descriptions: type of attributes passed to the vertex shader, which binding and at which offset
		vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
		vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

		// SECTION: Input Assembly
		inputAssemblyInfo =
		{
				VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
				nullptr,
				0,
				pipelineSet.topology, // because we want to draw a triangle for now
				false
		};

		// SECTION: Viewport
		viewportInfo =
		{
				VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
				nullptr,
				0,
				1, // REVIEW: More than one viewport makes a split screen game?
				nullptr, // viewport is defined by the dynamic states (below)
				1,
				nullptr // scissor is defined by the dynamic states (below)
		};

		// SECTION: Rasterization
		rasterizationInfo =
		{
				VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
				nullptr,
				0,
				false,
				false, // setting this to VK_TRUE disabled any output to the framebuffer
				pipelineSet.polygonMode,
				VK_CULL_MODE_BACK_BIT, // for now, we'll always cull the back face
				VK_FRONT_FACE_CLOCKWISE, // order for faces to be considered front-facing (in our case is counter clockwise because of MVP Y-flip in the shader)
				false,
				0.0f,
				0.0f,
				0.0f,
				1.0f
		};

		// SECTION: Multisampling
		multisampleInfo =
		{
				VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
				nullptr,
				0,
				VK_SAMPLE_COUNT_1_BIT,
				false,
				1.0f,
				nullptr,
				false,
				false
		};

		// SECTION: Depth and Stencil Testing
		depthStencilInfo =
		{
				VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
				nullptr,
				0,
				true,
				true,
				VK_COMPARE_OP_LESS,
				false,
				false,
				{},
				{},
				0.0f,
				1.0f
		};

		// SECTION: Color Blending
		colorBlendAttachment =
		{
				pipelineSet.enableBlending,
				VK_BLEND_FACTOR_SRC_ALPHA,
				VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
				VK_BLEND_OP_ADD,
				VK_BLEND_FACTOR_SRC_ALPHA,
				VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
				VK_BLEND_OP_SUBTRACT,
				VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
		};

		colorBlendInfo =
		{
				VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
				nullptr,
				0,
				false,
				VK_LOGIC_OP_COPY,
				1,
				&colorBlendAttachment,
				{0.0f, 0.0f, 0.0f, 0.0f}
		};

		// SECTION: Dynamic States (Viewport and Scissor)
		dynamicStateEnables = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };

		dynamicStateInfo =
		{
				VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
				nullptr,
				0,
				static_cast<uint32_t>(dynamicStateEnables.size()),
				dynamicStateEnables.data()
		};

		// pipeline definition
		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = 2;
		pipelineInfo.pStages = shaderStages;
		pipelineInfo.pVertexInputState = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &inputAssemblyInfo;
		pipelineInfo.pViewportState = &viewportInfo;
		pipelineInfo.pRasterizationState = &rasterizationInfo;
		pipelineInfo.pMultisampleState = &multisampleInfo;
		pipelineInfo.pDepthStencilState = &depthStencilInfo;
		pipelineInfo.pColorBlendState = &colorBlendInfo;
		pipelineInfo.pDynamicState = &dynamicStateInfo;
		pipelineInfo.layout = pipelineSet.layout;
		pipelineInfo.renderPass = VulkanSwapchain::GetRenderPass();
		pipelineInfo.subpass = 0; // not using subpasses for now
		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
		pipelineInfo.basePipelineIndex = -1;

		VK_CHECK(vkCreateGraphicsPipelines(VulkanDevice::GetDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipelineSet.pipeline));

		Logger::Debug("Custom pipeline created");
	}

	void VulkanPipelineBuilder::CreateShaderModule(const std::vector<char>& shaderCode, VkShaderModule* shaderModule)
	{
		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = shaderCode.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(shaderCode.data());

		VK_CHECK(vkCreateShaderModule(VulkanDevice::GetDevice(), &createInfo, nullptr, shaderModule));

		Logger::Debug("Created shader module");
	}

	void VulkanPipelineBuilder::Cleanup()
	{
		Logger::Debug("Destroying shader modules");
		vkDestroyShaderModule(VulkanDevice::GetDevice(), vertShaderModule, nullptr);
		vkDestroyShaderModule(VulkanDevice::GetDevice(), fragShaderModule, nullptr);
	}
}
