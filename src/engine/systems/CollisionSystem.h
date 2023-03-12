//
// Created by Diego S. Seabra on 24/02/23
//

#ifndef FEATHER_COLLISION_SYSTEM_H
#define FEATHER_COLLISION_SYSTEM_H

#include "../components/Components.h"
#include <entt/entt.hpp>
#include "QuadTree.h"

class CollisionSystem
{
public:
	static void Init();
	static void Update();
	static void Shutdown();

private:
	static bool CheckCollision(AreaComponent a1, AreaComponent a2);

	inline static QuadTree quadtree = QuadTree(0, Rect(0,0,0,0));
};

#endif FEATHER_COLLISION_SYSTEM_H
