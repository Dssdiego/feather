//
// Created by Diego S. Seabra on 24/01/23
//

#ifndef FEATHER_AREASYSTEM_H
#define FEATHER_AREASYSTEM_H

#include "../world/World.h"

// FIXME: all of this is required to be included before including the "Components.h" header
#include <glm/glm.hpp>
#include "../rendering/shapes/Shape.h"
#include "../common/structs.h"
#include "../common/Color.h"
#include "../entities/ECS.h"
// all... END
#include "../components/Components.h"

namespace Feather
{
	class AreaSystem
	{
	public:
		static void Update();

		static bool IsInside(AreaComponent area, glm::vec3 coords);
	};
}

#endif //FEATHER_AREASYSTEM_H
