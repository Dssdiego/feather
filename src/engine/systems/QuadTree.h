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

#define MAX_ENTITIES 10		// How many objects a node can hold before it splits
#define MAX_LEVELS 5		// Deepest level subnode

namespace Feather
{
	class QuadTree
	{
	public:
		QuadTree();
		QuadTree(uint32_t level, Rect bounds);

		void Update();

		//uint32_t GetIndex(Rect rect);

		//void Split();
		//void Insert();

		//std::vector<entt::entity> Retrieve();

	private:
		uint32_t mLevel; // Current node level (0 being the topmost node)

		std::vector<entt::entity> mEntities{};
		std::vector<QuadTree> mNodes{};

		Rect mBounds;

		void Clear();
	};
}

#endif //FEATHER_QUAD_TREE_H

