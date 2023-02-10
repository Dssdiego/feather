//
// Created by Diego S. Seabra on 15/10/22.
//

#ifndef FEATHER_SPRITE_ANIMATOR_H
#define FEATHER_SPRITE_ANIMATOR_H

#include <iostream>

class SpriteAnimator
{
public:
    static void Init();
    static void UpdateEntities();

private:
    inline static uint32_t fps = 24;
    inline static uint32_t frameCount = 0; // FIXME: We already have the frame counter in the "Game.cpp" class
};


#endif //FEATHER_SPRITE_ANIMATOR_H
