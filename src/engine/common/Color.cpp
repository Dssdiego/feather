//
// Created by Diego S. Seabra on 13/08/22.
//

#include "Color.h"

// Constructors
Color::Color(float r, float g, float b, float a): r(r), g(g), b(b), a(a) {}

Color::Color(const std::string &hex)
{
    auto rHex = std::stoul(hex.substr(0, 2), nullptr, 16);
    auto gHex = std::stoul(hex.substr(2, 2), nullptr, 16);
    auto bHex = std::stoul(hex.substr(4, 2), nullptr, 16);

    r = (float) rHex / 255.f;
    g = (float) gHex / 255.f;
    b = (float) bHex / 255.f;
    a = 1.f;
}

// Definitions
const Color Color::Transparent  = {0.0f,0.0f,0.0f,0.0f};
const Color Color::Black        = {0.0f,0.0f,0.0f,1.0f};
const Color Color::Gray         = {0.54f,0.54f,0.54f,1.0f};
const Color Color::White        = {1.0f,1.0f,1.0f,1.0f};
const Color Color::Red          = {1.0f,0.0f,0.0f,1.0f};
const Color Color::Green        = {0.0f,1.0f,0.0f,1.0f};
const Color Color::Blue         = {0.0f,0.0f,1.0f,1.0f};
const Color Color::Yellow       = {1.0f,1.0f,0.0f,1.0f};
const Color Color::Pink         = {1.0f,0.0f,1.0f,1.0f};
const Color Color::Cyan         = {0.0f,1.0f,1.0f,1.0f};
const Color Color::Background   = {0.47f, 0.42f, 0.96f, 1.0f};
const Color Color::Sky   = {0.57f, 0.82f, 1.f, 1.0f};

VkClearColorValue Color::GetAsVulkanClear(Color color)
{
    VkClearColorValue result;
    result.float32[0] = color.r;
    result.float32[1] = color.g;
    result.float32[2] = color.b;
    result.float32[3] = color.a;
    return result;
}

glm::vec4 Color::ToVec4(Color color)
{
    return glm::vec4(color.r,color.g,color.b,color.a);
}

// FIXME: This can be done automatically!
Color Color::GetColorFromString(const std::string &value)
{
    auto color = Color::Black;

    if (value == "black")
        color = Color::Black;

    if (value == "blue")
        color = Color::Blue;

    if (value == "red")
        color = Color::Red;

    if (value == "white")
        color = Color::White;

    if (value == "green")
        color = Color::Green;

    if (value == "pink")
        color = Color::Pink;

    if (value == "yellow")
        color = Color::Yellow;

    if (value == "cyan")
        color = Color::Cyan;

    return color;
}

ImVec4 Color::ToImGuiColor(Color color)
{
    return ImVec4(color.r, color.g, color.b, color.a);
}

std::string Color::ToString() const
{
    std::stringstream ss;
    ss << "{ r: " << r << ", g: " << g << ", b: " << b << ", a: " << a << " }";
    return ss.str();
}
