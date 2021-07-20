//
// Created by yhtse on 7/16/2021.
//
#ifndef SHADER_H_
#define SHADER_H_

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <map>
//#include <boost/filesystem.hpp>
#include <glad/glad.h>
#include <initializer_list>


namespace Ace {
    namespace fs = std::filesystem;
    struct StringCompare {
        using result_type = bool;
        using first_argument_type = std::string;
        using second_argument_type = std::string;

        bool operator()(const std::string& lhs, const std::string& rhs)const {
            if (lhs.size() == rhs.size()) {
                for (unsigned int i = 0; i < lhs.size(); ++i) {
                    if (lhs[i] != rhs[i]) {
                        return false;
                    }
                }
                return true;
            }
            return false;
        }
    };
    class Shader {
    public:
        Shader(const fs::path &vertexShader, const fs::path &fragShader);

        ~Shader() {}

    public:
        // 出现不能查询到变量地址的问题
        // TODO(AceBilly) bug
        void setValue(const std::string &var, float value1);

        void setValue(const std::string& var, float (&value)[4]);

        void setValue(const std::string &varName, std::initializer_list<float> vec4);

        // 需要指定变量位置的文件
        void getVariables(const fs::path &shaderPath);

    public:
        void use() const {
            glUseProgram(m_id);
        }

        [[nodiscard]] const uint getId() const { return m_id; }

    private:
        int m_id;  // shader program id;
        std::map<std::string, int, StringCompare> m_variables;
    };
}

#endif  // shader_h_