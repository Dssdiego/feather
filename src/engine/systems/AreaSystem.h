//
// Created by Diego S. Seabra on 24/01/23
//

#ifndef FEATHER_AREA_SYSTEM_H
#define FEATHER_AREA_SYSTEM_H

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

#endif //FEATHER_AREA_SYSTEM_H
