//
// Created by Diego S. Seabra on 26/01/23
//

#include "ECS.h"

// FIXME: all of this is required to be included before including the "Components.h" header
#include <glm/glm.hpp>
#include "../rendering/shapes/Shape.h"
#include "../common/structs.h"
#include "../common/Color.h"
#include "../entities/ECS.h"
// all... END
#include "../components/Components.h"
#include "../rendering/SpriteBatcher.h"

// systems
#include "../systems/AreaSystem.h"
#include "../systems/DragSystem.h"

namespace Feather
{
	void ECS::Init() 
	{
		// start with an empty entity
		ECS::Create();
	}

	void ECS::Shutdown()
	{
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

		Identity identity =
		{
			std::to_string(randId),
			entityName
		};

		AtlasSpriteRenderer renderer =
		{
			Color::White,
			"sprite",
			"engine",
			0,
			true
		};

		// TODO: When creating an entity, automatically add the id tag/component to it!
		auto newEntity = registry.create();
		AddComponent<Identity>(newEntity, identity);
		AddComponent<TransformComponent>(newEntity, transform);
		AddComponent<AtlasSpriteRenderer>(newEntity, renderer);
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
		AreaSystem::Update();
		DragSystem::Update();
	}
}
