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

float scale = 2;
float resolution = 2;

float grid(vec2 st, float res){
    vec2 grid = fract(st*res);
    return 1.f - (step(res, grid.x) * step(res, grid.y));
}

// shader code
void main() {
    vec3 color = vec3(0.035f, 0.035f, 0.035f);

    // Grid
    vec2 grid_st = uv * 300.f;
//    color += vec3(0.5f, 0.f, 0.f) * grid(grid_st, 0.01f);
//    color += vec3(0.2f, 0.f, 0.f) * grid(grid_st, 0.02f);
    color += vec3(0.2f) * grid(grid_st, 0.1f);

    outColor = vec4(color, 1.0f);
}
