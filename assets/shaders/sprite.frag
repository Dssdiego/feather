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
//    outColor = vec4(uv, 0.f, 1.f);
    // vec4 texColor = texture(texSampler, v_uv);
    vec4 texColor = texture(sampler2D(textures[v_texId], texSampler), v_uv);
    if (texColor.a == 0)
        discard;

    outColor = v_color * texColor;
}
