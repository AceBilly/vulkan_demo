//
// Created by yhtse on 7/16/2021.
//

#include "Shader.h"

#include <string>
// readfile
// \param filePath 文件路径
// \returns {std::string} 返回包含文件内容的string
std::string readFile(const Ace::fs::path &filePath) {
    try {
        if(!Ace::fs::exists(filePath)) {
            throw std::runtime_error("file path not exists!");
        }
        std::string res;
        std::ifstream file(filePath);
        std::stringstream fileStream;
        fileStream << file.rdbuf();
        res = fileStream.str();
        return res;
    } catch(std::exception& e) {
        std::cout << "failed read file: " << e.what() << std::endl;
    }


}

Ace::Shader::Shader(fs::path vertexShader, fs::path fragShader) {
    const char* vertexShaderCode = readFile(vertexShader).c_str();
    const char* fragShaderCode = readFile(fragShader).c_str();

}