#version 450
//#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec2 fragCoord;
layout(location = 0) out vec4 outColor;

struct Pixel{
  vec4 value;
};

layout(std140, binding = 0) buffer buf
{
   Pixel imageData[];
};

void main() {
  // r g b a 800*gl_fragCoord.y+gl_fragCoord.x
  vec4 color = vec4(1,1,1,1);
  outColor = imageData[int(fragCoord.x)+int(fragCoord.y*800)].value;
  //outColor = vec4(fragCoord, 0, 1);
}