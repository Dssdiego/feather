//
// Created by Diego S. Seabra on 26/01/23
//

#include "ECS.h"

#include "../components/Components.h"
#include "../rendering/SpriteBatcher.h"

// systems
#include "../systems/AreaSystem.h"
#include "../systems/DragSystem.h"
#include "../systems/GravitySystem.h"

// game systems
#include "../../../game/src/systems/GameSystems.h"
#include "../systems/CollisionSystem.h"

namespace Feather
{
	void ECS::Init() 
	{
		// start with an empty entity
		ECS::Create();

		// initializing systems
		CollisionSystem::Init();
	}

	void ECS::Shutdown()
	{
		// shutting down systems
		CollisionSystem::Shutdown();

		// destroying all entities
		DestroyAll();
	}

	entt::entity ECS::Create()
	{
		TransformComponent transform =
		{
			glm::vec3(0.f, 0.f, 0.f),
			glm::vec3(1.f, 1.f, 1.f),
			glm::vec3(0.f, 0.f, 0.f),
			glm::vec3(0.f, 0.f, 0.f)
		};

		auto randId = rand();
		std::string entityName = "Empty Entity";

		IdentityComponent identity =
		{
			std::to_string(randId),
			entityName
		};

		SpriteRendererComponent renderer =
		{
			Color::White,
			"sprite",
			"engine",
			0,
			true
		};

		// TODO: When creating an entity, automatically add the id tag/component to it!
		auto newEntity = registry.create();
		AddComponent<IdentityComponent>(newEntity, identity);
		AddComponent<TransformComponent>(newEntity, transform);
		AddComponent<SpriteRendererComponent>(newEntity, renderer);
		return newEntity;
	}

	void ECS::Destroy(entt::entity e)
	{
		registry.destroy(e);
	}

	void ECS::DestroyAll()
	{
		for (auto& e : entities)
		{
			Destroy(e);
		}
	}

	void ECS::Update()
	{
		// updating engine systems
		AreaSystem::Update();
		DragSystem::Update();
		GravitySystem::Update();
		CollisionSystem::Update();

		// updating game systems
		GameSystems::Update();
	}
}
