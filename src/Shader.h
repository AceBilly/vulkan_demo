//
// Created by yhtse on 7/16/2021.
//
#ifndef SHADER_H_
#define SHADER_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <map>
//#include <boost/filesystem.hpp>
#include <glad/glad.h>

namespace Ace {
    namespace fs = std::filesystem;

    class Shader {
    public:
        Shader(const fs::path& vertexShader, const fs::path& fragShader);
        ~Shader() {}
    public:
        void setValue(const std::string& var, float value1);
        void setValue(const std::string& var, float (&value)[4]);
    public:
        void use() const {
            glUseProgram(m_id);
        }

        [[nodiscard]] const uint getId() const { return m_id; }

    private:
        int m_id;  // shader program id;
    };
}

#endif  // shader_h_