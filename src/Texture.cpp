//
// Created by yhtse on 7/22/2021.
//

#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../inc/stb_image.h"

Ace::Texture::Texture() {
    glGenTextures(1, &m_textureId);
    glBindTexture(GL_TEXTURE_2D, m_textureId);
}

void Ace::Texture::setTextureParameter() {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Ace::Texture::load2DTexture(const Ace::fs::path &texturePath) {
    unsigned char *imageData = stbi_load(texturePath.c_str(), &m_textureInfo.width, &m_textureInfo.height,
                                         &m_textureInfo.colorChannel, 0);
    if (!imageData)
        throw std::runtime_error("failed load texture");
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_textureInfo.width, m_textureInfo.height, 0, GL_RGB, GL_UNSIGNED_BYTE,
                 imageData);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(imageData);
}

Ace::Texture::Texture(const Ace::fs::path &texturePath) : Texture() {
    setTextureParameter();
    load2DTexture(texturePath);
}
