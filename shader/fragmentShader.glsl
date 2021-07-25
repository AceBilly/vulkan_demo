#version 450 core

out vec4 FragColor;
layout (location = 7) uniform sampler2D cusTexture;
layout (location = 8) uniform sampler2D cusTexture1;
in vec4 vertexColor;
in vec2 textureCoords;
vec2 emojCoords = vec2(1-textureCoords.x, textureCoords.y);
void main() {
        FragColor = mix(texture(cusTexture, textureCoords), texture(cusTexture1, emojCoords), 0.3) * vertexColor;
//        FragColor.r = redColor;
//        FragColor = texture(cusTexture, textureCoords);
}
