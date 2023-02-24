//
// Created by Diego S. Seabra on 24/02/23
//

#include "CollisionSystem.h"

void CollisionSystem::Update()
{
	/*auto view = ECS::GetRegistry()->view<const AreaComponent>();
	view.each([](const auto& area)
		{
		});*/
}

bool CollisionSystem::CheckCollision(AreaComponent a1, AreaComponent a2)
{
    return a1.max.x > a2.min.x && a1.min.x < a2.max.x &&
		   a1.max.y > a2.min.y && a1.min.y < a2.max.y;
}

