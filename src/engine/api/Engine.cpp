//
// Created by Diego S. Seabra on 08/03/22.
//

#include "Engine.h"
#include "../rendering/ImGuiRenderer.h"
#include "../core/Time.h"
#include "../animation/SpriteAnimator.h"
//#include "../sdks/Steam.h"
#include "../networking/NetClient.h"
#include "../rendering/vulkan/VulkanPipeline.h"
#include "../input/Mouse.h"
#include "../animation/tweening/Tweener.h"
#include "../entities/ECS.h"
#include "../systems/SpriteRenderSystem.h"
#include "../editor/Editor.h"
#include "../audio/AudioEngine.h"

namespace Feather
{
	/*
	 * Methods
	 */
	void Engine::Init(EngineConfig* pConfig)
	{
		// Init engine systems/modules
		// TODO: Make a way to load modules in a easier way (without needing to to call init, update and shutdown every time)
		Logger::Init();
		Window::Init(pConfig);
		Mouse::Init();
		//AudioEngine::Init();
		Input::Init();
		VulkanEngine::Init();
		Editor::Init();
		ImGuiRenderer::Init();
		SpriteRenderSystem::Init();
		ECS::Init();

		miniAudio = MiniAudioBackend();
		AudioEngine::SetupBackend(&miniAudio);
		AudioEngine::Init();

		//SpriteAnimator::Init();
		//Steam::Init();
		//NetClient::Init();
	//    Font::LoadFont();
	//    Renderer::Init(GraphicsBackend::VULKAN);
	//    CGeforceNow::Init();
	}

	void Engine::Run()
	{
		while (!Window::ShouldCloseWindow())
		{
			ZoneScopedNC("Game::MainLoop", 0x34495e);

			// REVIEW: Probably won't need this because the renderer now has BeginFrame() and EndFrame() which could give us the FPS information
			//         Put this in the EngineRenderer class?
			double currentTime = Window::GetTime();
			double delta = currentTime - previousTime;
			Time::SetDelta(delta);
			previousTime = currentTime;

			// FPS Calculation
			frameCount++;
			int fps = int(frameCount / delta);
			Window::SetFrameInfo(fps, frames);
			frameCount = 0;

			// we only update sprite animations every two seconds
			//SpriteAnimator::UpdateEntities();

			//NetClient::Run();

			// Systems Update
			ECS::Update();

			// Updates
			//Tweener::Update();
			Window::Update();
			Mouse::Update();
			VulkanPipeline::Update();
			Camera::Update();
			Input::Update();
			AudioEngine::Update();

			Draw();

			// increment the frame number
			frames++; // REVIEW: Calculate frames in the Swapchain?
		}
	}

	void Engine::Draw()
	{
		ZoneScopedC(0x1abc9c);

		FrameMarkStart("Frame");
		VulkanEngine::BeginFrame();

		ImGuiRenderer::NewFrame();
		ImGuiRenderer::RenderGizmos(); // we render the gizmos before rendering the world entities (to be in the background)

		SpriteRenderSystem::RenderEntities(ECS::GetRegistry());

		ImGuiRenderer::Render();
		ImGuiRenderer::EndFrame();

		VulkanEngine::EndFrame();
		FrameMarkEnd("Frame");
	}

	void Engine::Cleanup()
	{
		// destroy engine systems
	//    CGeforceNow::Shutdown();
		//NetClient::Shutdown();
		//Steam::Shutdown();
		AudioEngine::Shutdown();
		SpriteRenderSystem::Shutdown();
		Editor::Shutdown();
		ImGuiRenderer::Shutdown();
		VulkanEngine::Shutdown();
		ECS::Shutdown();
		//    Renderer::Shutdown();
		Input::Shutdown();
		Mouse::Shutdown();
		Window::Shutdown();
	}
}

