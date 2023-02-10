//
// Created by Diego S. Seabra on 31/01/23
//

#ifndef FEATHER_EDITOR_DRAW_H
#define FEATHER_EDITOR_DRAW_H

#include <string>
#include <glm/glm.hpp>

class EditorDraw
{
public:
    // vector controls
    static void Vec2Control(const std::string& label, glm::vec2& values, float resetValue = 0.f, float columnWidth = 100.f);
    static void Vec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.f, float columnWidth = 100.f);
    static void Vec4Control(const std::string& label, glm::vec4& values, float resetValue = 0.f, float columnWidth = 100.f);

    // tooltips
    static void SetTooltip(std::string text);

    // buttons
    static bool AlignedButton(const char* label, float alignment = 0.5f);
};

#endif //FEATHER_EDITOR_DRAW_H
