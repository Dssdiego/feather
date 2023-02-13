#version 450

// inputs from vertex shader
layout(location = 0) in vec2 uv;

// output fragment shader color
layout(location = 0) out vec4 outColor;

// push constants
layout(push_constant) uniform Push {
    mat4 transform;
    vec4 color;
} push;

// shader code
void main() {
    vec4 finalColor = push.color;
    if (finalColor.a == 0)
        discard;

    outColor = finalColor;
}
