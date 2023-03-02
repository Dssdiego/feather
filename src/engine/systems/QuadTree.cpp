//
// Created by Diego S. Seabra on 01/03/23
//

#include "QuadTree.h"

namespace Feather
{
	// Constructors
	QuadTree::QuadTree() { }
	QuadTree::QuadTree(uint32_t level, Rect bounds)
	{
		mLevel = level;
		mEntities = {};
		mBounds = bounds;
		mNodes = std::vector<QuadTree>(4);
	}

	uint32_t QuadTree::GetIndex(Rect rect)
	{
		return 0;
	}

	// Clears the quadtree
	void QuadTree::Clear()
	{
		mEntities.clear();

		for (auto& n : mNodes)
		{
			// REVIEW: Check "n != nullptr"?
			n.Clear();
		}
	}

	// Splits the node into 4 subnodes
	void QuadTree::Split()
	{
		uint32_t subWidth = (int)(mBounds.GetWidth() / 2);
		uint32_t subHeight = (int)(mBounds.GetHeight() / 2);

		uint32_t x = mBounds.GetX();
		uint32_t y = mBounds.GetY();

		mNodes.push_back(QuadTree(mLevel + 1, Rect(x + subWidth, y, subWidth, subHeight)));
		mNodes.push_back(QuadTree(mLevel + 1, Rect(x, y, subWidth, subHeight)));
		mNodes.push_back(QuadTree(mLevel + 1, Rect(x, y + subHeight, subWidth, subHeight)));
		mNodes.push_back(QuadTree(mLevel + 1, Rect(x + subWidth, y + subHeight, subWidth, subHeight)));
	}

	// Insert the entity into the quadtree. 
	// If the node exceeds the capacity, it will split and add all objects
	// to their corresponding nodes
	void QuadTree::Insert()
	{
	}

	// Returns all entities that could collide with the given object
	std::vector<entt::entity> QuadTree::Retrieve()
	{
		return std::vector<entt::entity>();
	}

}

