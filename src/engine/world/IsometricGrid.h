//
// Created by Diego S. Seabra on 06/11/22.
//

#ifndef FEATHER_ISOMETRICGRID_H
#define FEATHER_ISOMETRICGRID_H

#include <iostream>
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"

class IsometricGrid
{
public:
    static glm::vec3 GridToWorldPosition(glm::vec2 gridPosition);

private:
    inline static const glm::vec2 rowOffset = { 1.f, 0.5f };
    inline static const glm::vec2 colOffset = {-1.f, 0.5f };
};


#endif //FEATHER_ISOMETRICGRID_H
