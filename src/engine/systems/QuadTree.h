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

namespace Feather
{
	class QuadTree
	{
	public:
		QuadTree();
		QuadTree(uint32_t level, Rect bounds);

		uint32_t GetIndex(Rect rect);

		void Clear();
		void Split();
		void Insert();

		std::vector<entt::entity> Retrieve();

	private:
		uint32_t mMaxObjects = 10; // How many objects a node can hold before it splits
		uint32_t mMaxLevels = 5;   // Deepest level subnode

		uint32_t mLevel = 0;	   // Current node level (0 being the topmost node)

		std::vector<entt::entity> mEntities{};
		std::vector<QuadTree> mNodes{4};

		Rect mBounds;
	};
}

#endif //FEATHER_QUAD_TREE_H

