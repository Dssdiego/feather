//
// Created by Diego S. Seabra on 01/03/23
//

#ifndef FEATHER_QUAD_TREE_H
#define FEATHER_QUAD_TREE_H

#include <iostream>
#include <entt/entt.hpp>
#include <vector>
#include "../common/structs.h"
#include "../common/Rect.h"
#include "../components/Components.h"

//#define MAX_ENTITIES 10		// How many objects a node can hold before it splits
//#define MAX_LEVELS 5		// Deepest level subnode

namespace Feather
{
	struct Node
	{
		std::array<std::unique_ptr<Node>, 4> children;
		std::vector<entt::entity> entities;
	};

	class QuadTree
	{
	public:
		QuadTree();
		//QuadTree(uint32_t level, Rect bounds);

		void Update();

	private:
		static constexpr auto Threshold = std::size_t(16);  // Maximum number of values a node can contain before we try to split it
		static constexpr auto MaxDepth = std::size_t(8);	// Maximum depth of a node. We stop trying to split nodes which are at MaxDepth because it can hurt performance if we subdivide too much

		std::unique_ptr<Node> mRoot;
		Rect mRect;

		void Clear();

		void Insert(Node* node, std::size_t depth, Rect& rect, entt::entity entity);
		bool Remove(Node* node, Rect& rect, entt::entity entity);

		void Split(Node* node, Rect &rect);

		bool IsLeaf(const Node* node) const
		{
			return !static_cast<bool>(node->children[0]);
		}
	};
}

#endif //FEATHER_QUAD_TREE_H

