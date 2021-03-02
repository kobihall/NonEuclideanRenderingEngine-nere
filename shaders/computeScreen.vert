#version 450

layout(location = 0) in vec2 position;
layout(location = 0) out vec2 fragCoord;

vec2 positions[6] = vec2[](
    vec2(1, -1),
    vec2(1, 1),
    vec2(-1, 1),
    vec2(-1, 1),
    vec2(-1, -1),
    vec2(1, -1)
);

void main() {
    gl_Position = vec4(positions[gl_VertexIndex], 0.0, 1.0);
    fragCoord.x = 800*(positions[gl_VertexIndex].x+1)/2;
    fragCoord.y = 600*(positions[gl_VertexIndex].y+1)/2;
}