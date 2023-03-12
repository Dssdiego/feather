//
// Created by Diego S. Seabra on 01/03/23
//

#include "QuadTree.h"

namespace Feather
{
	//
	// Constructors
	//
	QuadTree::QuadTree() {}

	QuadTree::QuadTree(uint32_t level, Rect bounds)
	{
		mLevel = level;
		mBounds = bounds;
		mNodes.resize(4);
	}

	void QuadTree::Update()
	{
		Clear();

		auto view = ECS::GetRegistry()->view<const CollisionComponent>();
		view.each([](const auto& collision)
			{
			});
	}

	void QuadTree::Clear()
	{
		// TODO: Implement
	}
}

