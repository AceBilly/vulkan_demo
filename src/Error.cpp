#include "Error.h"


void Ace::Error::shaderError(unsigned int id, bool flag, const std::source_location location) {
    int success;
    char infoLog[512];
    if (flag) {
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    } else {
        glGetProgramiv(id, GL_LINK_STATUS, &success);
    }
    if (!success) {
        std::cout << "file_name: " << location.file_name() << ":\n"
                  << "line: " << location.line() << "\n \t"
                  << "function: " << location.function_name() << '\n';
        if (flag) {
            glGetShaderInfoLog(id, 512, nullptr, infoLog);
            std::cout << "Error:Shader vertex compilation_failed " << infoLog << std::endl;
        } else {
            glGetProgramInfoLog(id, 512, nullptr, infoLog);
            std::cout << "Error::program link failed " << infoLog << std::endl;
        }
    }
}