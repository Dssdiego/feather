//
// Created by Diego S. Seabra on 24/01/23
//

#include "AreaSystem.h"
#include "../entities/ECS.h"

namespace Feather
{
	// REVIEW: This should be in a EntitySystem Update()?
	void AreaSystem::Update()
	{
		// TODO: Check if we have the transform component in the first place!
		//	     If we remove it, the area system should continue to execute/process
		auto view = ECS::GetRegistry()->view<const TransformComponent, AreaComponent>();
		view.each([](const auto& transform, auto& area)
			{
				area.min = transform.position - (0.5f * transform.scale);
				area.max = transform.position + (0.5f * transform.scale);
			});

		// FIXME: Area System doesn't work on entities that are rotated!
	}

	bool AreaSystem::IsInside(AreaComponent area, glm::vec3 coords)
	{
		if (coords.x >= area.min.x && coords.x <= area.max.x &&
			coords.y >= area.min.y && coords.y <= area.max.y)
		{
			return true;
		}

		return false;
	}
}
