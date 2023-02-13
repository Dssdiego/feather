//
// Created by Diego S. Seabra on 12/09/22.
//

#ifndef FEATHER_UTILS_H
#define FEATHER_UTILS_H

class Utils
{
public:

    /**
     * Pass a value between @inStart and @inStop and this method will return a number between
     * @outStart and @outStop
     **/
    inline static float MapToRange(float value, float inStart, float inStop, float outStart, float outStop)
    {
        return outStart + (outStop - outStart) * ((value - inStart) / (inStop - inStart));
    }

};

#endif //FEATHER_UTILS_H
