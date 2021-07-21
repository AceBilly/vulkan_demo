#include <iostream>
#include <exception>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION

#include "../inc/stb_image.h"

//#define __has_builtin(__builtin_source_location) true

#include <source_location>
#include <cmath>

#include "Shader.h"


constinit int windowWidth = 800;
constinit int windowHeight = 800;


constinit float lineSegment[] = {0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,// 左下
                                 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,// 右上
                                 0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,// 左上
                                 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f// 右下
};
constinit uint indices[] = {
        2, 1, 0,
        0, 1, 3
};


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
    Ace::Shader m_shader;
    unsigned int m_VAO;
    unsigned int m_EBO;
    unsigned int m_texId;
};

Data init() {
    // 设置背景色
    glClearColor(0.0, 1.0, 1.0, 0.0);
    glViewport(0, 0, windowWidth, windowHeight);
    Ace::Shader shader("shader/vertexShader.glsl", "shader/fragmentShader.glsl");
    shader.getVariables("config/variablesLocation.json");

    // 纹理对象
    int width, height, colorChannel;
    unsigned char *imageData = stbi_load("Images/brick.png", &width, &height, &colorChannel, 0);
    if (!imageData) {
        throw std::runtime_error("failed load image");
    }
    unsigned int textureId;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(imageData);

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
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *) (0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *) (2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*) (5 * sizeof(float)));
    glEnableVertexAttribArray(4);

    // 创建索引缓冲对象
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    shader.use();
    glClear(GL_COLOR_BUFFER_BIT);
    return Data{.m_shader=shader, .m_VAO=VAO, .m_EBO=EBO, .m_texId = textureId};
}

void loop(GLFWwindow *p_window) {
//    std::string var("color");
    auto[shader, VAO, EBO, textureId] = init();
    while (!glfwWindowShouldClose(p_window)) {
        float runTime = glfwGetTime();
        float blue = (sin(runTime) / 2.0f) + 0.5f;

        shader.use();
        glBindTexture(GL_TEXTURE_2D, textureId);
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