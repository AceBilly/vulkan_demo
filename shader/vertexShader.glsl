#version 450 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 inVertexColor;

layout (location = 5) uniform float offset;

//out vec4 vertexColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);

out vec4 vertexColor;
void main() {
    gl_Position = vec4(aPos.x + offset, aPos.y + offset, 0.0f, 1.0f);
//    gl_Position = Vec4(inVertexColor.r, inVertexColor.g, 0.0f, 1.0f);
    vertexColor = vec4(inVertexColor, 1.0f);

}
