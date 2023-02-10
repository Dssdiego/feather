//
// Created by Diego S. Seabra on 06/11/22.
//

#include "IsometricGrid.h"

glm::vec3 IsometricGrid::GridToWorldPosition(glm::vec2 gridPosition)
{
    return {
        gridPosition.x * rowOffset.x * 0.5f + gridPosition.y * colOffset.x * 0.5f,
        gridPosition.x * rowOffset.y * 0.5f + gridPosition.y * colOffset.y * 0.5f,
        0.f
    };
}
