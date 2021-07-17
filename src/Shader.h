//
// Created by yhtse on 7/16/2021.
//
#ifndef SHADER_H_
#define SHADER_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

#include <glad/glad.h>

namespace Ace {
    namespace fs = std::filesystem;

    class Shader {
    public:
        Shader(fs::path vertexShader, fs::path fragShader);

        ~Shader() {}

    public:
        void use() const {
            glUseProgram(m_id);
        }

        const uint getId() const { return m_id; }

    private:
        uint m_id;  // shader program id;

    };
}

#endif  // shader_h_