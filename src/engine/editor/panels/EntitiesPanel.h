//
// Created by Diego S. Seabra on 31/01/23
//

#ifndef FEATHER_ENTITIES_PANEL_H
#define FEATHER_ENTITIES_PANEL_H

#include <vector>
#include <entt/entt.hpp>

namespace Feather
{
	class EntitiesPanel
	{
	public:
		void Draw();

		inline entt::entity GetSelectedEntity() { return selected; }

	private:
		entt::entity selected{};
	};
}

#endif //FEATHER_ENTITIES_PANEL_H
