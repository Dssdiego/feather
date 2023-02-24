//
// Created by Diego S. Seabra on 24/02/23
//

#ifndef FEATHER_COLLISION_SYSTEM_H
#define FEATHER_COLLISION_SYSTEM_H

#include "../components/Components.h"
#include <entt/entt.hpp>

class CollisionSystem
{
public:
	static void Update();

private:
	static bool CheckCollision(AreaComponent a1, AreaComponent a2);
};

#endif FEATHER_COLLISION_SYSTEM_H
