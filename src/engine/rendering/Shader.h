//
// Created by Diego S. Seabra on 18/05/22.
//

#ifndef FEATHER_SHADER_H
#define FEATHER_SHADER_H

#include <string>
#include <vector>
#include <fstream>
#include <vulkan/vulkan.h>

class Shader
{
public:
    static std::vector<char> ReadFile(const std::string& filename);
};


#endif //FEATHER_SHADER_H
