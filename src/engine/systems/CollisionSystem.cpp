//
// Created by Diego S. Seabra on 24/02/23
//

#include "CollisionSystem.h"

void CollisionSystem::Init()
{
	//auto windowSize = Window::GetSize();
	quadtree = QuadTree();
}

void CollisionSystem::Update()
{
	quadtree.Update();
}

void CollisionSystem::Shutdown()
{
	// TODO: Implemen
}

bool CollisionSystem::CheckCollision(AreaComponent a1, AreaComponent a2)
{
    return a1.max.x > a2.min.x && a1.min.x < a2.max.x &&
		   a1.max.y > a2.min.y && a1.min.y < a2.max.y;
}

