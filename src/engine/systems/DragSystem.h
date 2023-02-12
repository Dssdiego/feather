//
// Created by Diego S. Seabra on 24/01/23
//

#ifndef FEATHER_DRAG_SYSTEM_H
#define FEATHER_DRAG_SYSTEM_H

#include "../input/Input.h"
#include "../systems/AreaSystem.h"

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
	class DragSystem
	{
	public:
		static void Update();
	};
}

#endif //FEATHER_DRAG_SYSTEM_H
