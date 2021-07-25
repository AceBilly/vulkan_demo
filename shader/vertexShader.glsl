#version 450 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 inVertexColor;
layout (location = 2) in vec2 inTextureCoords;

layout (location = 5) uniform mat4 transform;
//out vec4 vertexColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);

out vec4 vertexColor;
out vec2 textureCoords;
void main() {
    gl_Position = transform * vec4(aPos.x, aPos.y, 0.0f, 1.0f);
    vertexColor = vec4(inVertexColor, 1.0f);
    textureCoords = inTextureCoords;
}
