//
// Created by Diego S. Seabra on 17/08/22.
//

#include "SpriteRenderSystem.h"
#include "../rendering/vulkan/VulkanPipeline.h"
#include "../rendering/Camera.h"
#include "../rendering/SpriteBatcher.h"
#include "../common/Assets.h"
#include <Tracy.hpp>
#include "../entities/ECS.h"

// FIXME: all of this is required to be included before including the "Components.h" header
#include <glm/glm.hpp>
#include "../rendering/shapes/Shape.h"
#include "../common/structs.h"
#include "../common/Color.h"
#include "../entities/ECS.h"
// all... END
#include "../components/Components.h"

namespace Feather
{
	// TODO: Refactor the code so that we don't use raw pointers. Instead we want to use smart pointers
	//       See more here: https://stackoverflow.com/questions/106508/what-is-a-smart-pointer-and-when-should-i-use-one
	SpriteRenderSystemImpl* mSpriteRenderSystemImpl = nullptr;

	//
	// Initialization/Destruction
	//

	void SpriteRenderSystem::Init()
	{
		mSpriteRenderSystemImpl = new SpriteRenderSystemImpl;
	}

	void SpriteRenderSystem::Shutdown()
	{
		delete mSpriteRenderSystemImpl;
	}

	void SpriteRenderSystem::RenderEntities(entt::registry* registry)
	{
		mSpriteRenderSystemImpl->RenderEntities(registry);
	}


	//
	// Implementation
	//

	SpriteRenderSystemImpl::SpriteRenderSystemImpl()
	{
		Logger::Debug("Initializing Sprite Render System");

		VulkanPipeline::Init();
	}

	SpriteRenderSystemImpl::~SpriteRenderSystemImpl()
	{
		Logger::Debug("Shutting down Sprite Render System");

		SpriteBatcher::Shutdown();
		VulkanPipeline::Shutdown();
	}

	void SpriteRenderSystemImpl::RenderEntities(entt::registry* registry)
	{
		ZoneScopedC(0x9b59b6);

		// for every frame, we clear the batch
		// REVIEW: There is a better way to do this?
		SpriteBatcher::Clear();

		// update uniform buffer with new data
		// REVIEW: Do we need to create the UBO every frame?
		UniformBufferObject ubo{};
		ubo.mvp = (Camera::GetProjection() * Camera::GetView());
		VulkanEngine::UpdateUniformBuffer(ubo);

		// binding to the pipeline to draw the sprites
	//    VulkanPipeline::SwitchToPipeline("sprite"); // always sprite pipeline for now
		VulkanPipeline::Bind();

		// for each tile entity
	//    auto view = registry->view<const TransformComponent, WorldInfo, IsometricTile>();
	//    view.each([this, &ubo](const auto &transform, auto &worldInfo, auto &isometricTile) {
	//        // NOTE: This changes the entire batch! Use carefully!
	////        ubo.mvp = (Camera::GetProjection() * Camera::GetView() * transform.mat4());
	////        VulkanEngine::UpdateUniformBuffer(ubo);
	//        auto texSet = Assets::GetTextureSet(isometricTile.textureName);
	//
	//        auto uv = SpriteAtlas::GetUVsCoords(texSet, isometricTile.atlasPosition - 1, isometricTile.flippedX);
	//        auto quad = SpriteBatcher::MakeQuad(transform.position, Color::ToVec4(worldInfo.color), uv, texSet->id);
	//        SpriteBatcher::AddQuad(quad);
	//    });

		// for each tile sprite entity
		auto solView = registry->view<const TransformComponent, SpriteRendererComponent>();
		solView.each([this, &ubo](const auto& transform, auto& renderer) {
			auto texSet = Assets::GetTextureSet(renderer.textureName);

		auto uv = SpriteAtlas::GetUVsCoords(texSet, renderer.tile, false);
		auto quad = SpriteBatcher::MakeQuad(
			transform.position,
			Color::ToVec4(renderer.color),
			uv,
			texSet->id,
			GetModelTransformation(transform)
		);

		if (renderer.visible)
			SpriteBatcher::AddQuad(quad);
			});

		// FIXME: This recreates the vertex and index buffers every frame. How can we reuse them?
		SpriteBatcher::Create();
		SpriteBatcher::Draw();
	}

	glm::mat4 SpriteRenderSystemImpl::GetModelTransformation(TransformComponent transform)
	{
		return glm::translate(glm::mat4(1.0f), transform.position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)) *
			glm::rotate(glm::mat4(1.0f), glm::radians(transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)) *
			glm::rotate(glm::mat4(1.0f), glm::radians(transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)) *
			glm::scale(glm::mat4(1.0f), transform.scale);
	}
}
