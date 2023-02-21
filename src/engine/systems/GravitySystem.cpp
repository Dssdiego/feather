//
// Created by Diego S. Seabra on 21/02/23
//

#include "GravitySystem.h"

#include "../components/Components.h"

void GravitySystem::Update()
{
	auto view = ECS::GetRegistry()->view<const GravityComponent, TransformComponent>();
	view.each([](const auto& gravity, auto& transform)
		{
			transform.position.y += (gravity.value * Time::Delta);
		});
}
