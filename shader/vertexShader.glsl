#version 450 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 inVertexColor;
layout (location = 2) in vec2 inTexCoords;

out vec3 vertexColor;

void main() {
    gl_Position = vec4(aPos.x, aPos.y, 0.0f, 1.0f);
    vertexColor = inVertexColor;
}
