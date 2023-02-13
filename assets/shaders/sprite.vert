#version 450

// inputs from code/pipeline
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec4 v_color;
layout(location = 2) in vec2 v_uv;
layout(location = 3) in uint v_texId;
layout(location = 4) in mat4 v_mvp; // occupying locations 4,5,6,7 (4 x vec4, to represent a mat4 in code)

// send to fragment shader
layout(location = 0) out vec2 uv;
layout(location = 1) out vec4 color;
layout(location = 2) out uint texId;

// uniform buffer
layout(set = 0, binding = 0) uniform UniformBufferObject {
    mat4 mvp;
} ubo;

// shader code
void main() {
    gl_Position = ubo.mvp * v_mvp * vec4(v_position, 1.0f);

    // send to fragment shader
    uv = v_uv;
    color = v_color;
    texId = v_texId;
}
