//
// Created by Diego S. Seabra on 01/03/23
//

#include "QuadTree.h"

namespace Feather
{
	//
	// Constructors
	//
	QuadTree::QuadTree() 
	{
		mRoot = std::make_unique<Node>();
	}

	//QuadTree::QuadTree(uint32_t level, Rect bounds)
	//{
		//mLevel = level;
		//mBounds = bounds;
		//mNodes.resize(4);
	//}

	void QuadTree::Update()
	{
		//auto view = ECS::GetRegistry()->view<const CollisionComponent>();
		//view.each([](const auto& collision)
		//	{
		//	});
	}

	void QuadTree::Clear()
	{
		// TODO: Implement
	}

	void QuadTree::Insert(Node* node, std::size_t depth, Rect& rect, entt::entity entity)
	{
		assert(node != nullptr);
	}

	bool QuadTree::Remove(Node* node, Rect& rect, entt::entity entity)
	{
		assert(node != nullptr);

		return false;
	}

	void QuadTree::Split(Node* node, Rect& rect)
	{
		assert(node != nullptr);
		assert(IsLeaf(node) && "Only leaves can be split!");
	}
}

