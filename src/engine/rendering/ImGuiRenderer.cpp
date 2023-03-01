//
// Created by Diego S. Seabra on 22/07/22.
//

#include "ImGuiRenderer.h"
#include "../input/Input.h"
//#include "../animation/tweening/Tweener.h"
//#include <tweeny/tweeny.h>
#include <imgui/IconsFontAwesome6.h>
#include "Samplers.h"
#include "../entities/ECS.h"
#include <imgui_internal.h>
#include "../editor/EditorDraw.h"

#include "../components/Components.h"

namespace Feather
{
	// checking for vulkan error
	static void check_vk_result(VkResult err) {
		if (err == 0) return;
		fprintf(stderr, "[vulkan] Error: VkResult = %d\n", err);
		if (err < 0) abort();
	}

	// TODO: Refactor the code so that we don't use raw pointers. Instead we want to use smart pointers
	//       See more here: https://stackoverflow.com/questions/106508/what-is-a-smart-pointer-and-when-should-i-use-one
	ImGuiRendererImpl* mImGuiRendererImpl = nullptr;

	//
	// Initialization/Destruction
	//

	void ImGuiRenderer::Init()
	{
#ifndef NDEBUG
		mImGuiRendererImpl = new ImGuiRendererImpl;
#endif
	}

	void ImGuiRenderer::Shutdown()
	{
#ifndef NDEBUG
		delete mImGuiRendererImpl;
#endif
	}

	//
	// External
	//

	void ImGuiRenderer::NewFrame()
	{
		ZoneScopedC();

#ifndef NDEBUG
		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();

		// begin command buffer
		auto commandBuffer = GetCurrentCommandBuffer();
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		VK_CHECK(vkBeginCommandBuffer(commandBuffer, &beginInfo));
#endif
	}

	void ImGuiRenderer::RenderGizmos()
	{
		ZoneScopedC();

#ifndef NDEBUG
		mImGuiRendererImpl->DrawGizmos();
#endif
	}


	void ImGuiRenderer::Render()
	{
		ZoneScopedC();

#ifndef NDEBUG
		mImGuiRendererImpl->Draw();
		ImGui::Render();
		ImDrawData* drawdata = ImGui::GetDrawData();

		// draw ImGui stuff
		ImGui_ImplVulkan_RenderDrawData(drawdata, GetCurrentCommandBuffer());
#endif
	}

	void ImGuiRenderer::EndFrame()
	{
#ifndef NDEBUG
		// end command buffer
		VK_CHECK(vkEndCommandBuffer(GetCurrentCommandBuffer()));
#endif
	}

	VkCommandBuffer ImGuiRenderer::GetCurrentCommandBuffer()
	{
		return mImGuiRendererImpl->GetCurrentCommandBuffer();
	}

	//
	// Implementation
	//

	ImGuiRendererImpl::ImGuiRendererImpl()
	{
		Logger::Debug("Initializing ImGuiRenderer");

		CreateViewportImages();
		CreateViewportImageViews();

		CreateDescriptorPool();
		CreateCommandPool();
		CreateCommandBuffers();

		SetupContext();
		BindVulkanBackend();

		gameDescriptorSet = ImGui_ImplVulkan_AddTexture(Samplers::Get("linear"), VulkanSwapchain::GetCurrentImageView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
	}

	ImGuiRendererImpl::~ImGuiRendererImpl()
	{
		Logger::Debug("Destroying ImGuiRenderer");

		vkDeviceWaitIdle(VulkanDevice::GetDevice());

		vkDestroyDescriptorPool(VulkanDevice::GetDevice(), descriptorPool, nullptr);
		VulkanEngine::FreeCommandBuffers(commandBuffers);
		vkDestroyCommandPool(VulkanDevice::GetDevice(), commandPool, nullptr);

		ImGui_ImplVulkan_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiRendererImpl::CreateViewportImages()
	{
		auto swapChainImageCount = VulkanSwapchain::GetImageCount();
		viewportImages.resize(swapChainImageCount);

		for (uint32_t i = 0; i < swapChainImageCount; i++)
		{
			// TODO:
			/*VulkanImage::CreateImage(0, 0,
				VK_FORMAT_B8G8R8A8_SRGB,
				VK_IMAGE_TILING_LINEAR,
				VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
				nullptr,
				nullptr,
				nullptr
			);*/
		}
	}

	void ImGuiRendererImpl::CreateViewportImageViews()
	{
		// resizing the list to fit all of the image views
		auto swapChainImageCount = VulkanSwapchain::GetImageCount();
		viewportImageViews.resize(swapChainImageCount);

		for (size_t i = 0; i < swapChainImageCount; i++)
		{
			// REVIEW: Use UNORM or SRGB?
			//viewportImageViews[i] = VulkanSwapchain::CreateImageView(viewportImages[i], VK_FORMAT_B8G8R8A8_UNORM, VK_IMAGE_ASPECT_COLOR_BIT); 
		}
	}

	void ImGuiRendererImpl::CreateDescriptorPool()
	{
		VkDescriptorPoolSize pool_sizes[] = {
				{VK_DESCRIPTOR_TYPE_SAMPLER, 1000},
				{VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000},
				{VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000},
				{VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000},
				{VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000},
				{VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000},
				{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000},
				{VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000},
				{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000},
				{VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000},
				{VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000} };
		VkDescriptorPoolCreateInfo pool_info = {};
		pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		pool_info.maxSets = 1000 * IM_ARRAYSIZE(pool_sizes);
		pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
		pool_info.pPoolSizes = pool_sizes;

		VK_CHECK(vkCreateDescriptorPool(VulkanDevice::GetDevice(), &pool_info, nullptr, &descriptorPool));
	}

	void ImGuiRendererImpl::CreateCommandPool()
	{
		QueueFamilyIndices queueFamilyIndices = VulkanDevice::FindPhysicalQueueFamilies();

		VkCommandPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT; // this allows the command buffer to be implicitly reset when 'vkBeginCommandBuffer' is called
		poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily; // since we want to record the commands for drawing, we must use the graphics queue family

		VK_CHECK(vkCreateCommandPool(VulkanDevice::GetDevice(), &poolInfo, nullptr, &commandPool));

		Logger::Debug("ImGui command pool created");
	}

	void ImGuiRendererImpl::CreateCommandBuffers()
	{
		commandBuffers = VulkanEngine::CreateCommandBuffers(commandPool);
	}

	void ImGuiRendererImpl::SetupContext()
	{
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		(void)io;
		io.ConfigFlags = ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange; // blocking ImGui to override the mouse cursor
		//if (io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange)
		//    return;
		// io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		// io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

		// load default font
		io.Fonts->AddFontDefault();

		// loading WorkSans custom font as default
		io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/WorkSans-Regular.ttf", 15.f);

		// merge in icons from Font Awesome
		static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
		ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = true;
		io.Fonts->AddFontFromFileTTF(FONT_ICON_FILE_NAME_FAS, 16.0f, &icons_config, icons_ranges);
		// use FONT_ICON_FILE_NAME_FAR if you want regular instead of solid

		io.Fonts->AddFontFromFileTTF("assets/fonts/WorkSans-Bold.ttf", 15.f);

		// Setup Dear ImGui style
		//ImGui::StyleColorsDark();
		ImGuiStyle& style = ImGui::GetStyle();
		style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
		style.Colors[ImGuiCol_ChildBg] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
		style.Colors[ImGuiCol_PopupBg] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
		style.Colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
		style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_FrameBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
		style.Colors[ImGuiCol_TitleBg] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
		style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
		style.Colors[ImGuiCol_CheckMark] = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
		style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.08f, 0.50f, 0.72f, 1.00f);
		style.Colors[ImGuiCol_Button] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
		style.Colors[ImGuiCol_Header] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
		style.Colors[ImGuiCol_Separator] = style.Colors[ImGuiCol_Border];
		style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.41f, 0.42f, 0.44f, 1.00f);
		style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
		style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.29f, 0.30f, 0.31f, 0.67f);
		style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
		style.Colors[ImGuiCol_Tab] = ImVec4(0.08f, 0.08f, 0.09f, 0.83f);
		style.Colors[ImGuiCol_TabHovered] = ImVec4(0.33f, 0.34f, 0.36f, 0.83f);
		style.Colors[ImGuiCol_TabActive] = ImVec4(0.23f, 0.23f, 0.24f, 1.00f);
		style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
		style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
		style.Colors[ImGuiCol_DockingPreview] = ImVec4(0.26f, 0.59f, 0.98f, 0.70f);
		style.Colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
		style.Colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
		style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
		style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
		style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
		style.GrabRounding = style.FrameRounding = 2.3f;
		style.WindowMenuButtonPosition = ImGuiDir_None; // disabling close and tab list buttons in tabbed windows
	}

	void ImGuiRendererImpl::BindVulkanBackend()
	{
		// Setup Platform/Renderer backends
		// Initialize imgui for vulkan
		ImGui_ImplGlfw_InitForVulkan(Window::GetWindow(), true);
		ImGui_ImplVulkan_InitInfo init_info = {};
		init_info.Instance = VulkanDevice::GetInstance();
		init_info.PhysicalDevice = VulkanDevice::GetPhysicalDevice();
		init_info.Device = VulkanDevice::GetDevice();
		init_info.QueueFamily = VulkanDevice::GetGraphicsQueueFamilyIdx();
		init_info.Queue = VulkanDevice::GetGraphicsQueue();

		// pipeline cache is a potential future optimization, ignoring for now
		init_info.PipelineCache = VK_NULL_HANDLE;
		init_info.DescriptorPool = descriptorPool;
		init_info.Allocator = VK_NULL_HANDLE; // not using an allocator right now
		init_info.MinImageCount = 2;
		init_info.ImageCount = VulkanSwapchain::GetImageCount();
		init_info.CheckVkResultFn = check_vk_result;
		ImGui_ImplVulkan_Init(&init_info, VulkanSwapchain::GetRenderPass());

		// upload fonts, this is done by recording and submitting a one time use command buffer
		auto commandBuffer = VulkanDevice::BeginSingleTimeCommands();
		ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);
		VulkanDevice::EndSingleTimeCommands(commandBuffer);
		ImGui_ImplVulkan_DestroyFontUploadObjects();
	}

	VkCommandBuffer ImGuiRendererImpl::GetCurrentCommandBuffer()
	{
		assert(!commandBuffers.empty() && "There must be at least one command buffer present!");

		return commandBuffers[VulkanEngine::GetFrameIndex()];
	}

	void ImGuiRendererImpl::DrawWindowPanel()
	{
		ImGui::Begin("Window", &showNetworkingPanel);

		auto items = Window::GetResolutions();
		static int idx = 0; // Here we store our selection data as an index.
		auto comboPreview = items[idx];  // Pass in the preview value visible before opening the combo (it could be anything)
		if (ImGui::BeginCombo("Resolution", comboPreview.c_str()))
		{
			for (int n = 0; n < items.size(); n++)
			{
				const bool is_selected = (idx == n);
				if (ImGui::Selectable(items[n].c_str(), is_selected))
					idx = n;

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		if (ImGui::Button("Apply"))
		{
			std::string s = items[idx];
			std::replace(s.begin(), s.end(), 'x', ' '); // replace all 'x' to 'y'

			std::string::size_type sz; // alias of size_t

			uint32_t width = std::stoi(s, &sz);
			uint32_t height = std::stoi(s.substr(sz));

			Window::Resize(width, height);
		}

		if (ImGui::Checkbox("Fullscreen", &gameFullscreen))
		{
			Window::SetFullscreen(gameFullscreen);
		}

		if (ImGui::Checkbox("Vsync", &enableVsync))
		{
			Window::EnableVSync(enableVsync);
		}

		ImGui::End();
	}

	void ImGuiRendererImpl::DrawTodoPanel()
	{
		ImGui::Begin("TODO'S", &showTodoPanel);
		ImGui::End();
	}

	void ImGuiRendererImpl::DrawGizmos()
	{
		if (showDebugPanels && showGizmos)
		{
			ImGuizmo::SetOrthographic(Camera::GetMode() == CameraMode::ORTOGRAPHIC ? true : false);
			//ImGuizmo::SetDrawlist(); // to draw inside a separate window
			ImGuizmo::SetRect(0, 0, Window::GetSize().width, Window::GetSize().height);
			ImGuizmo::DrawGrid(glm::value_ptr(Camera::GetViewForGizmos()), glm::value_ptr(Camera::GetProjection()), glm::value_ptr(glm::mat4(1.f)), 100);
			ImGuizmo::ViewManipulate((float*)
				&Camera::GetView(),
				10,
				ImVec2(Window::GetSize().width - 142, 32),
				ImVec2(128, 128),
				0x10101010
			);
		}
	}

	void ImGuiRendererImpl::Draw()
	{
		// profiling
		ZoneScopedC();

		// demo
		//ImGui::ShowDemoWindow((bool*) true);

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;
		ImGui::Begin("ImGui Window", (bool*)true, window_flags);
		ImGui::PopStyleVar(3);

		// dockspace
		ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

		// bars (main menu bar, sub menu bar, status bar)
		DrawMainMenuBar();
		DrawSubMenuBar();
		DrawStatusBar();

		// debug panels
		if (showDebugPanels)
		{
			if (showEntitiesPanel)
				entitiesPanel.Draw();

			if (showInspectorPanel)
				inspectorPanel.Draw(entitiesPanel.GetSelectedEntity());

			if (showProfilerPanel)
				profilerPanel.Draw();

			if (showCameraPanel)
				DrawCameraPanel();

			if (showDebugLogPanel)
				debugLogPanel.Draw();

			if (showEngineStatusPanel)
				engineStatusPanel.Draw();

			if (showNetworkingPanel)
				networkingPanel.Draw();

			if (showRaycastPanel)
				raycastPanel.Draw();

			if (showWindowPanel)
				DrawWindowPanel();

			//if (showTweenPanel)
			//	tweenPanel.Draw();

			if (showViewportPanel)
				viewportPanel.Draw();

			if (showTodoPanel)
				DrawTodoPanel();

			if (showWorldEditorPanel)
				worldEditorPanel.Draw();
		}

		ImGui::End();
	}

	void ImGuiRendererImpl::DrawMainMenuBar()
	{
		if (ImGui::BeginMainMenuBar())
		{
			// Engine
			if (ImGui::BeginMenu("Engine"))
			{
				// Engine >> Status
				if (ImGui::MenuItem("Status", nullptr, &showEngineStatusPanel)) {}

				// Engine >> Exit
				if (ImGui::MenuItem("Exit", "CTRL+Q")) { Window::Close(); }

				ImGui::EndMenu();
			}

			// Tools
			if (ImGui::BeginMenu("Window"))
			{
				// Tools >> World Editor
	//            if (ImGui::MenuItem("World Editor", nullptr, &showWorldEditorPanel)) {}

				// Tools >> Inspector
				//if (ImGui::MenuItem("Inspector", nullptr, &showInspectorPanel)) {}

				// Tools >> Profiler
				if (ImGui::MenuItem("Profiler", nullptr, &showProfilerPanel)) {}

				// Tools >> Camera
				if (ImGui::MenuItem("Camera", nullptr, &showCameraPanel)) {}

				// Tools >> Window
				if (ImGui::MenuItem("Window", nullptr, &showWindowPanel)) {}

				// Tools >> Gizmos
				if (ImGui::MenuItem("Gizmos", nullptr, &showGizmos)) {}

				// Tools >> Debug Log
				if (ImGui::MenuItem("Debug Log", nullptr, &showDebugLogPanel)) {}

				// Show/Hide windows
				ImGui::Separator();
				if (ImGui::MenuItem("Show/Hide Windows", nullptr, &showDebugPanels)) {}

				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}
	}

	void ImGuiRendererImpl::DrawSubMenuBar()
	{
		ImGuiViewportP* viewport = (ImGuiViewportP*)(void*)ImGui::GetMainViewport();
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar;
		float height = ImGui::GetFrameHeight();

		if (ImGui::BeginViewportSideBar("##SubMenuBar", viewport, ImGuiDir_Up, height, window_flags)) {
			if (ImGui::BeginMenuBar())
			{
				// load/save
				ImGui::Button(ICON_FA_FOLDER); // TODO: Implement
				ImGui::Button(ICON_FA_FLOPPY_DISK); // TODO: Implement
				ImGui::Separator();

				// toggle windows/panels
				if (ImGui::Button(ICON_FA_GAMEPAD))
					showViewportPanel = !showViewportPanel;
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Game Viewport");

				if (ImGui::Button(ICON_FA_GLOBE))
					showEntitiesPanel = !showEntitiesPanel;
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Entities");

				if (ImGui::Button(ICON_FA_CUBE))
					showInspectorPanel = !showInspectorPanel;
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Inspector");

				if (ImGui::Button(ICON_FA_CAMERA))
					showCameraPanel = !showCameraPanel;
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Camera");

				if (ImGui::Button(ICON_FA_CODE))
					showDebugLogPanel = !showDebugLogPanel;
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Debug Log");

				ImGui::Separator();

				// transformation mode (move, rotate, scale)
				ImGui::Button(ICON_FA_ARROW_POINTER); // TODO: Implement
				ImGui::Button(ICON_FA_UP_DOWN_LEFT_RIGHT); // TODO: Implement
				ImGui::Button(ICON_FA_ROTATE); // TODO: Implement
				ImGui::Button(ICON_FA_UP_RIGHT_AND_DOWN_LEFT_FROM_CENTER); // TODO: Implement
				ImGui::Separator();

				ImGui::EndMenuBar();
			}
			ImGui::End();
		}
	}

	void ImGuiRendererImpl::DrawStatusBar()
	{
		ImGuiViewportP* viewport = (ImGuiViewportP*)(void*)ImGui::GetMainViewport();
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar;
		float height = ImGui::GetFrameHeight();

		if (ImGui::BeginViewportSideBar("##StatuBar", viewport, ImGuiDir_Down, height, window_flags))
		{
			if (ImGui::BeginMenuBar()) {
				ImGui::Text("Engine Ready OK!");
				ImGui::EndMenuBar();
			}
			ImGui::End();
		}
	}

	void ImGuiRendererImpl::DrawCameraPanel()
	{
		ImGui::Begin(ICON_FA_CAMERA " Camera", &showCameraPanel);

		ImGui::Checkbox("Control Camera?", &Camera::takeControl);
		if (ImGui::Button("Reset"))
			Camera::ResetWorldPosition();

		std::string aspectStr = "Aspect Ratio: " + std::to_string(VulkanSwapchain::GetAspectRatio());
		ImGui::Text(aspectStr.c_str());

		if (ImGui::TreeNode("Matrices"))
		{
			if (ImGui::TreeNode("View"))
			{
				ImGui::InputFloat3("##label", (float*)&Camera::GetView()[0]);
				ImGui::InputFloat3("##label", (float*)&Camera::GetView()[1]);
				ImGui::InputFloat3("##label", (float*)&Camera::GetView()[2]);
				ImGui::InputFloat3("##label", (float*)&Camera::GetView()[3]);
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Projection"))
			{
				ImGui::InputFloat3("##label", (float*)&Camera::GetProjection()[0]);
				ImGui::InputFloat3("##label", (float*)&Camera::GetProjection()[1]);
				ImGui::InputFloat3("##label", (float*)&Camera::GetProjection()[2]);
				ImGui::InputFloat3("##label", (float*)&Camera::GetProjection()[3]);
				ImGui::TreePop();
			}

			ImGui::TreePop();
		}

		EditorDraw::Vec3Control("position", Camera::GetWorldPosition());
		//ImGui::InputFloat3("position", (float *) Camera::GetWorldPositionRef());
		if (!Camera::HasControl())
			Camera::SetWorldPosition(Camera::GetWorldPosition());

		EditorDraw::Vec3Control("direction", Camera::GetWorldDirection());
		if (!Camera::HasControl())
			Camera::SetWorldDirection(Camera::GetWorldDirection());

		const char* items[] = { "ortographic", "perspective" };
		static int idx = 0; // Here we store our selection data as an index.
		auto comboPreview = items[idx];  // Pass in the preview value visible before opening the combo (it could be anything)
		if (ImGui::BeginCombo("projection", comboPreview))
		{
			for (int n = 0; n < 2; n++)
			{
				const bool is_selected = (idx == n);
				if (ImGui::Selectable(items[n], is_selected))
				{
					idx = n;

					if (std::strcmp(items[n], "perspective") == 0)
						Camera::SetMode(CameraMode::PERSPECTIVE);

					if (std::strcmp(items[n], "ortographic") == 0)
						Camera::SetMode(CameraMode::ORTOGRAPHIC);
				}

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		if (ImGui::InputFloat("zoom", (float*)Camera::GetOrtographicZoomRef(), 0.5f, 2.0f))
		{
			Camera::UpdateProjection();
		}

		ImGui::End();
	}
}

