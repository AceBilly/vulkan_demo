//
// Created by yhtse on 7/21/2021.
//

#include "GLWindow.h"

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

void Ace::GLWindow::createWindow(uint width, uint height, const std::string &winTitle) {
    m_window = glfwCreateWindow(width, height, winTitle.c_str(), nullptr, nullptr);
    if (!m_window)
        throw std::runtime_error("failed create window!");
    glfwMakeContextCurrent(m_window);

}


Ace::GLWindow::GLWindow(uint width, uint height, const std::string &winTitle) {
    initialize_glfw();
    createWindow(width, height, winTitle);
    initialize_glad();
}

