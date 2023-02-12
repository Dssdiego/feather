//
// Created by Diego S. Seabra on 15/10/22.
//

#include "SpriteAnimator.h"

// FIXME: all of this is required to be included before including the "Components.h" header
#include <glm/glm.hpp>
#include "../rendering/shapes/Shape.h"
#include "../common/structs.h"
#include "../common/Color.h"
#include "../entities/ECS.h"
// all... END
#include "../components/Components.h"

void SpriteAnimator::Init()
{
//    std::ifstream f("assets/yaml/anims.yaml");
//    std::string str((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
}

void SpriteAnimator::UpdateEntities()
{
//    auto view = World::GetRegistry()->view<Sprite>();
//
//    // for each entity
//    view.each([](auto &animSprite)
//      {
//          auto animFrame = animSprite.currentFrame;
//          auto beginTile = animSprite.beginTile;
//          auto endTile = animSprite.endTile;
//
//          animFrame++;
//
//          if (animFrame > endTile.y)
//              animFrame = 0;
//
//          animSprite.currentFrame = animFrame;
//      });

}
