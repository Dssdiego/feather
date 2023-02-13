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

vec3 grayscale(vec3 color, float str) {
    float g = dot(color, vec3(0.299, 0.587, 0.114));
    return mix(color, vec3(g), str);
}

// shader code
void main() {
    vec4 texColor = texture(sampler2D(textures[v_texId], texSampler), v_uv);
    if (texColor.a == 0)
        discard;

    vec4 multColor = v_color * texColor;
    vec3 finalColor = grayscale(multColor.rgb, 1.0f);

    outColor = vec4(finalColor, 1.0f);
}
