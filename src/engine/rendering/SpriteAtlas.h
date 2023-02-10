//
// Created by Diego S. Seabra on 22/09/22.
//

#ifndef FEATHER_SPRITEATLAS_H
#define FEATHER_SPRITEATLAS_H

#include <iostream>
#include <imgui.h>
#include <imgui_impl_vulkan.h>

#include "Texture.h"
#include "../common/Assets.h"

// NOTE: We are only using a single texture for all engine/game sprites because pixel art is very small
//       and a 4k texture is enough to hold a lot of images ;)
class SpriteAtlas
{
public:
    static glm::vec4 GetUVCoordinate(TextureSet* texSet, uint32_t line, uint32_t column);
    static std::vector<glm::vec2> GetUVsCoords(TextureSet* texSet, uint32_t tileId, bool flippedX);
};


#endif //FEATHER_SPRITEATLAS_H
