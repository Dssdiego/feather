#version 450

layout(binding = 1) uniform sampler2D texSampler;

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
    outColor = push.color;
}
