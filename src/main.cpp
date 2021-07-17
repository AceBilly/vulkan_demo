#include <iostream>
#include <exception>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define __has_builtin(__builtin_source_location) true
#include <source_location>
#include <math.h>

#include "Shader.h"



constinit int windowWidth = 800;
constinit int windowHeight = 800;


constinit float lineSegment[] = {0.0f, 0.0f,
                                 0.5f, 0.5f,
                                 0.0f,  0.5f,
                                 0.5f, 0.0f
};
constinit uint indices[] = {
       2, 1, 0,
       0, 1, 3
};

const char *vertexShaderSrc = "#version 450 core\n"
                              "layout (location = 0) in vec2 aPos;\n"
                              "\n"
                              "void main()\n"
                              "{\n"
                              "    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);\n"
                              "}\0";
const char *fragShaderSrc = "#version 450 core\n"
                            "out vec4 FragColor;\n"
                            "uniform vec4 color;\n"
                            "\n"
                            "void main()\n"
                            "{\n"
                            "    FragColor = color;\n"
                            "}\0";

void initialize_glfw() {
    // 初始化glfw
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void initialize_glad() {
    // 初始化glad
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
        throw std::runtime_error("failed initialize glad");
}

GLFWwindow *createWindow(const int winWidth, const int winHeight) {
    GLFWwindow *p_window = glfwCreateWindow(winWidth, winHeight, "Computer Graphics", nullptr, nullptr);
    if (!p_window)
        throw std::runtime_error("failed create window!");
    glfwMakeContextCurrent(p_window);
    return p_window;
}

struct Data {
    unsigned int m_programId;
    unsigned int m_VAO;
    unsigned int m_EBO;
};

void handleError(unsigned int id, bool flag = true, const std::source_location location = std::source_location::current()) {
    int success;
    char infoLog[512];
    if (flag) {
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    } else {
        glGetProgramiv(id, GL_LINK_STATUS, &success);
    }
    if (!success) {
        std::cout << "file_name: " << location.file_name() << '\n'
                  << "function: " << location.function_name() << '\n'
                  << "line: " << location.line() << '\n';
        if(flag) {
            glGetShaderInfoLog(id, 512, nullptr, infoLog);
            std::cout << "Error:Shader vertex compilation_failed " << infoLog << std::endl;
        } else {
            glGetProgramInfoLog(id, 512, nullptr, infoLog);
            std::cout << "Error::program link failed " << infoLog << std::endl;
        }
    }
}

Data init() {
    // 设置背景色
    glClearColor(0.0, 1.0, 1.0, 0.0);
    glViewport(0, 0, windowWidth, windowHeight);
//    glm::ortho(0.0f, 100.0f, 0.0f, 100.0f, 0.0f, 0.0f);
// 编译顶点着色器
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSrc, nullptr);
    glCompileShader(vertexShader);
    handleError(vertexShader);
// 编译片段着色器源码
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragShaderSrc, nullptr);
    glCompileShader(fragmentShader);
    handleError(fragmentShader);
// 链接到着色器程序
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    handleError(shaderProgram, false);
// 释放着色器资源
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    // 顶点数组对象
    unsigned int VAO = 0;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    // 顶点缓冲对象
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lineSegment), lineSegment, GL_STATIC_DRAW);
    // 绑定属性指针
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *) (0));
    glEnableVertexAttribArray(0);
    // 创建索引缓冲对象
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glUseProgram(shaderProgram);
    glClear(GL_COLOR_BUFFER_BIT);
    return Data{.m_programId=shaderProgram, .m_VAO=VAO, .m_EBO=EBO};
}

void loop(GLFWwindow *p_window) {

    auto[shaderProgram, VAO, EBO] = init();
    while (!glfwWindowShouldClose(p_window)) {
        int uniformVarLocation = glGetUniformLocation(shaderProgram, "color");
        glUseProgram(shaderProgram);
        float runTime = glfwGetTime();
        float blue = (sin(runTime) / 2.0f) + 0.5f;
        glUniform4f(uniformVarLocation, 0.1f, 0.2, blue, 1.0f);

        glBindVertexArray(VAO);
        glLineWidth(8);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(p_window);
        glfwPollEvents();

    }
}

// 释放资源
void destructor() {
    glfwTerminate();
}

int main() {
    try {
        initialize_glfw();
        GLFWwindow *p_window = createWindow(windowWidth, windowHeight);
        initialize_glad();
        loop(p_window);
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    destructor();
    return 0;
}