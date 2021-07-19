//
// Created by yhtse on 7/16/2021.
//

#include "Shader.h"
#include "Error.h"

#include <string>

// readfile
// \param filePath 文件路径
// \returns {std::string} 返回包含文件内容的string
std::string readFile(const Ace::fs::path &filePath) {
    try {
        if (!Ace::fs::exists(filePath)) {
            throw std::runtime_error("file path not exists!");
        }
        std::string res;
        std::ifstream file;
        file.open(filePath);
        std::stringstream fileStream;
        fileStream << file.rdbuf();
        res = fileStream.str();
        return res;
    } catch (std::exception &e) {
        std::cout << "failed read file: " << e.what() << std::endl;
    }


}

// 从文件中读取着色器源码 并编译 最后释放着色器资源
// \param vertexShader 顶点着色器文件路径
// \param fragShader   片段着色器文件路径
Ace::Shader::Shader(const fs::path &vertexShader, const fs::path &fragShader) {
    // 读取文件 待优化
    std::string fragShaderCodeStr = readFile(fragShader);
    std::string vertexShaderCodeStr = readFile(vertexShader);
    const char *vertexShaderCode = vertexShaderCodeStr.c_str();
    const char *fragShaderCode = fragShaderCodeStr.c_str();

    uint vertexId = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexId, 1, &vertexShaderCode, nullptr);
    glCompileShader(vertexId);
    Ace::Error::shaderError(vertexId, true);

    uint fragmentId = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentId, 1, &fragShaderCode, nullptr);
    glCompileShader(fragmentId);
    Ace::Error::shaderError(fragmentId, true);

    m_id = glCreateProgram();
    glAttachShader(m_id, vertexId);
    glAttachShader(m_id, fragmentId);
    glLinkProgram(m_id);
    Ace::Error::shaderError(m_id, false);
    use();
    glDeleteShader(vertexId);
    glDeleteShader(fragmentId);
}


void Ace::Shader::setValue(const std::string &var, float value1) {
//    glUniform1f(glGetUniformLocation(m_id, var.c_str()), value1);
    int location = glGetUniformLocation(m_id, var.c_str());

    glUniform1f(location, value1);
}

void Ace::Shader::setValue(const std::string &var, float (&values)[4]) {
    int location = glGetUniformLocation(m_id, var.c_str());
    auto [v1, v2, v3, v4] = values;
//    int location = 2;
    glUniform4f(location, v1, v2, v3, v4);
}