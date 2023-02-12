//
// Created by Diego S. Seabra on 13/08/22.
//

#ifndef FEATHER_COLOR_H
#define FEATHER_COLOR_H

#include <cstdint>
#include <glm/vec4.hpp>
#include <string>
#include <vulkan/vulkan_core.h>
#include <iostream>
#include <sstream>
#include <imgui.h>

class Color
{
public:
    // Constructor
    Color(float r, float g, float b, float a);
    Color(const std::string &hex);

    std::string ToString() const;

    // Helpers
    static VkClearColorValue GetAsVulkanClear(Color color);
    static glm::vec4 ToVec4(Color color);
    static Color GetColorFromString(const std::string &value);
    static ImVec4 ToImGuiColor(Color color);

    // Definitions
    static const Color Transparent;
    static const Color Black;
    static const Color Gray;
    static const Color White;
    static const Color Red;
    static const Color Green;
    static const Color Blue;
    static const Color Yellow;
    static const Color Pink;
    static const Color Cyan;
    static const Color Background;
    static const Color EngineClear;

    // Specials
    static const Color Sky;

    float r, g, b, a;
};


#endif //FEATHER_COLOR_H
