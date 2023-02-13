#version 450

// binding 0 is occupied with the buffer in the vertex stage!
layout(set = 0, binding = 1) uniform texture2D textures[16];
layout(set = 0, binding = 2) uniform sampler texSampler;

// inputs from vertex shader
layout(location = 0) in vec2 v_uv;
layout(location = 1) in vec4 v_color;
layout(location = 2) flat in uint v_texId;

// output fragment shader color
layout(location = 0) out vec4 outColor;

// shader code
void main() {
    // vec2 uvSinglePixel = vec2(0.01f, 0.01f);
    // vec4 texColor = texture(sampler2D(textures[v_texId], texSampler), uvSinglePixel);
    // outColor = texColor;
    outColor = vec4(0.f, 1.f, 0.f, 1.f);
}
