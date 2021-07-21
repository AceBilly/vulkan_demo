#version 450 core

in vec2 texCoords;
in vec3 vertexColor;

out vec4 FragColor;

layout (location = 2) uniform sampler2D customTexture0;
uniform sampler2D customTexture1;

void main() {
//        FragColor = vec4(vertexColor, 1.0f);
        FragColor = texture(customTexture0, texCoords) * vec4(vertexColor, 1.0f);
}
