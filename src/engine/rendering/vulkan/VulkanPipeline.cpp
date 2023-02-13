//
// Created by Diego S. Seabra on 14/08/22.
//

#include "VulkanPipeline.h"
#include "../../input/Input.h"

namespace Feather
{
	// TODO: Refactor the code so that we don't use raw pointers. Instead we want to use smart pointers
	//       See more here: https://stackoverflow.com/questions/106508/what-is-a-smart-pointer-and-when-should-i-use-one
	VulkanPipelineImpl* mVulkanPipelineImpl = nullptr;

	//
	// Initialization/Destruction
	//

	void VulkanPipeline::Init()
	{
		mVulkanPipelineImpl = new VulkanPipelineImpl;
	}

	void VulkanPipeline::Bind()
	{
		mVulkanPipelineImpl->Bind();
	}

	void VulkanPipeline::Update()
	{
		if (Input::IsKeyboardKeyPressed(Keys::F1))
			SwitchToPipeline("grayscale");

		if (Input::IsKeyboardKeyPressed(Keys::F2))
			SwitchToPipeline("sprite");

		if (Input::IsKeyboardKeyPressed(Keys::F3))
			SwitchToPipeline("bounds");
	}

	void VulkanPipeline::Shutdown()
	{
		delete mVulkanPipelineImpl;
	}

	void VulkanPipeline::SwitchToPipeline(const std::string& name)
	{
		mVulkanPipelineImpl->SwitchToPipeline(name);
	}

	PipelineSet VulkanPipeline::GetCurrent()
	{
		return mVulkanPipelineImpl->GetCurrent();
	}

	//
	// Implementation
	//

	VulkanPipelineImpl::VulkanPipelineImpl()
	{
		CreatePipelineSets();
	}

	VulkanPipelineImpl::~VulkanPipelineImpl()
	{
		// TODO: We need to wait for commands to complete before destroying the pipeline
		vkDeviceWaitIdle(VulkanDevice::GetDevice());

		pipelineBuilder.Cleanup();

		for (auto& pipelineSet : pipelineSets)
		{
			std::stringstream ss;
			ss << "Destroying pipeline '" << pipelineSet.first << "'...";
			Logger::Debug(ss.str());

			Logger::Debug("Destroying pipeline layout");
			vkDestroyPipelineLayout(VulkanDevice::GetDevice(), pipelineSet.second.layout, nullptr);

			Logger::Debug("Destroying graphics pipeline");
			vkDestroyPipeline(VulkanDevice::GetDevice(), pipelineSet.second.pipeline, nullptr);
		}
	}

	//
	// Helpers
	//

	void VulkanPipelineImpl::CreatePipelineSets()
	{
		// Shape
		PipelineSet shapePipelineSet
		{
				"assets/shaders/shape.vert.spv",
				"assets/shaders/shape.frag.spv",
				shapePipeline,
				shapePipelineLayout,
				VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
				VK_POLYGON_MODE_FILL,
				VK_TRUE
		};
		pipelineSets.insert(std::pair<std::string, PipelineSet>("shape", shapePipelineSet));

		// Sprite
		PipelineSet spritePipelineSet
		{
				"assets/shaders/sprite.vert.spv",
				"assets/shaders/sprite.frag.spv",
				spritePipeline,
				spritePipelineLayout,
				VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
				VK_POLYGON_MODE_FILL,
				VK_TRUE
		};
		pipelineSets.insert(std::pair<std::string, PipelineSet>("sprite", spritePipelineSet));

		// Grayscale
		PipelineSet grayscalePipelineSet
		{
				"assets/shaders/sprite.vert.spv",
				"assets/shaders/grayscale.frag.spv",
				grayscalePipeline,
				grayscalePipelineLayout,
				VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
				VK_POLYGON_MODE_FILL,
				VK_TRUE
		};
		pipelineSets.insert(std::pair<std::string, PipelineSet>("grayscale", grayscalePipelineSet));

		// Bounds
		PipelineSet boundsPipelineSet
		{
				"assets/shaders/sprite.vert.spv",
				"assets/shaders/bounds.frag.spv",
				boundsPipeline,
				boundsPipelineLayout,
				VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
				VK_POLYGON_MODE_LINE,
				VK_TRUE
		};
		pipelineSets.insert(std::pair<std::string, PipelineSet>("bounds", boundsPipelineSet));

		// UI
		PipelineSet uiPipelineSet
		{
				"assets/shaders/ui.vert.spv",
				"assets/shaders/ui.frag.spv",
				uiPipeline,
				uiPipelineLayout,
				VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
				VK_POLYGON_MODE_FILL,
				VK_TRUE
		};
		pipelineSets.insert(std::pair<std::string, PipelineSet>("ui", uiPipelineSet));

		// Grid
		PipelineSet gridPipelineSet
		{
				"assets/shaders/grid.vert.spv",
				"assets/shaders/grid.frag.spv",
				gridPipeline,
				gridPipelineLayout,
				VK_PRIMITIVE_TOPOLOGY_LINE_STRIP,
				VK_POLYGON_MODE_LINE,
				VK_TRUE
		};
		pipelineSets.insert(std::pair<std::string, PipelineSet>("grid", gridPipelineSet));

		// Grid Plane
		PipelineSet gridPlanePipelineSet
		{
				"assets/shaders/gridPlane.vert.spv",
				"assets/shaders/gridPlane.frag.spv",
				gridPlanePipeline,
				gridPlanePipelineLayout,
				VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
				VK_POLYGON_MODE_FILL,
				VK_TRUE
		};
		pipelineSets.insert(std::pair<std::string, PipelineSet>("grid_plane", gridPlanePipelineSet));

		// Build all pipelines
		for (auto& pipelineSet : pipelineSets)
		{
			CreatePipelineLayout(pipelineSet.second.layout);
			pipelineBuilder.Build(pipelineSet.second);
		}

		// we always start with the sprite pipeline
		SwitchToPipeline("sprite");
	}

	// TODO: Make this method agnostic/dynamic
	void VulkanPipelineImpl::CreatePipelineLayout(VkPipelineLayout& layout)
	{
		VkPushConstantRange pushConstantRange{};
		pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		pushConstantRange.offset = 0;
		pushConstantRange.size = sizeof(PushConstantData);

		// TODO: Make it possible to pass any descriptor set layout we want in the pipeline
		std::vector<VkDescriptorSetLayout> descriptorSetLayouts{ VulkanEngine::GetDescriptorSetLayout() };

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
		pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
		pipelineLayoutInfo.pushConstantRangeCount = 1;
		pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

		VK_CHECK(vkCreatePipelineLayout(VulkanDevice::GetDevice(), &pipelineLayoutInfo, nullptr, &layout));

		Logger::Debug("Created pipeline layout");
	}

	void VulkanPipelineImpl::Bind()
	{
		// bind pipeline
		vkCmdBindPipeline(
			VulkanEngine::GetCurrentCommandBuffer(),
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			GetCurrent().pipeline
		);

		// bind descriptor sets
		vkCmdBindDescriptorSets(
			VulkanEngine::GetCurrentCommandBuffer(),
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			GetCurrent().layout,
			0,
			1,
			&VulkanEngine::GetDescriptorSets()[VulkanEngine::GetFrameIndex()], // REVIEW: My code is possibly wrong here!
			0,
			nullptr
		);
	}

	PipelineSet VulkanPipelineImpl::GetCurrent()
	{
		return pipelineSets.find(currentPipeline)->second;
	}

	void VulkanPipelineImpl::SwitchToPipeline(const std::string& name)
	{
		currentPipeline = name;
	}
}

