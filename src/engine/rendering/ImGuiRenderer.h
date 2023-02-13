//
// Created by Diego S. Seabra on 22/07/22.
//

#ifndef FEATHER_IMGUIRENDERER_H
#define FEATHER_IMGUIRENDERER_H

#include <glm/gtc/type_ptr.hpp>
#include <vulkan/vulkan.h>
#include <imgui.h>
#include <imgui_impl_vulkan.h>
#include <imgui_impl_glfw.h>
#include <ImGuizmo.h>
#include <entt/entt.hpp>
#include "Window.h"
#include "vulkan/VulkanDevice.h"
#include "vulkan/VulkanSwapchain.h"
#include "vulkan/VulkanEngine.h"
#include "vulkan/VulkanPipeline.h"
#include "Camera.h"
#include "SpriteBatcher.h"
#include "../networking/NetClient.h"
#include <Tracy.hpp>
#include "../editor/panels/DebugLogPanel.h"
#include "../editor/panels/ProfilerPanel.h"
#include "../editor/panels/NetworkingPanel.h"
#include "../editor/panels/ViewportPanel.h"
#include "../editor/panels/EngineStatusPanel.h"
#include "../editor/panels/RaycastPanel.h"
#include "../editor/panels/WorldEditorPanel.h"
#include "../editor/panels/TweenPanel.h"
#include "../editor/panels/EntitiesPanel.h"
#include "../editor/panels/InspectorPanel.h"

namespace Feather
{
	struct ImGuiRendererImpl
	{
		ImGuiRendererImpl();
		~ImGuiRendererImpl();

		// using separate descriptor pool/command pool and command buffers for imgui
		VkDescriptorPool descriptorPool = nullptr;
		VkCommandPool commandPool = nullptr;
		VkDescriptorSet gameDescriptorSet = nullptr;
		std::vector<VkCommandBuffer> commandBuffers{};

		std::vector<VkImage> viewportImages{};
		std::vector<VkImageView> viewportImageViews{};
		VkRenderPass viewportRenderPass = nullptr;

		void CreateViewportImages();
		void CreateViewportImageViews();
		void CreateDescriptorPool();
		void CreateCommandPool();
		void CreateCommandBuffers();
		void SetupContext();
		void BindVulkanBackend();

		VkCommandBuffer GetCurrentCommandBuffer();

		void DrawGizmos();
		void Draw(); // here comes all the things we want to draw with ImGui

		void DrawMainMenuBar();
		void DrawSubMenuBar();
		void DrawStatusBar();
		void DrawCameraPanel();
		void DrawWindowPanel();
		void DrawTodoPanel();

		// global variables
		bool showDebugPanels = true;

		// control variables
		bool showCameraPanel = true;
		bool showEntitiesPanel = true;
		bool showInspectorPanel = true;
		bool showDebugLogPanel = true;
		bool showProfilerPanel = false;
		bool showWorldEditorPanel = false;
		bool showEngineStatusPanel = true;
		bool showNetworkingPanel = true;
		bool showWindowPanel = false;
		bool showRaycastPanel = false;
		bool showTweenPanel = false;
		bool showGizmos = false;
		bool showViewportPanel = false;
		bool showTodoPanel = true;

		bool gameFullscreen = false;
		bool enableVsync = false;

		InspectorPanel inspectorPanel = InspectorPanel();
		EntitiesPanel entitiesPanel = EntitiesPanel();
		TweenPanel tweenPanel = TweenPanel();
		RaycastPanel raycastPanel = RaycastPanel();
		EngineStatusPanel engineStatusPanel = EngineStatusPanel();
		ViewportPanel viewportPanel = ViewportPanel();
		NetworkingPanel networkingPanel = NetworkingPanel();
		ProfilerPanel profilerPanel = ProfilerPanel();
		DebugLogPanel debugLogPanel = DebugLogPanel();

		[[deprecated]]
		WorldEditorPanel worldEditorPanel = WorldEditorPanel();
	};

	class ImGuiRenderer
	{
	public:
		static void Init();
		static void Shutdown();

		static void RenderGizmos();

		static void NewFrame();
		static void Render(); // all that is drawed now must be rendered on the screen
		static void EndFrame();

		static VkCommandBuffer GetCurrentCommandBuffer();

	};
}

#endif //FEATHER_IMGUIRENDERER_H
