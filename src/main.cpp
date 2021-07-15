#include <iostream>
#include <exception>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "../inc/glm/glm.hpp"
#include "../inc/glm/gtc/matrix_transform.hpp"
#include "../inc/glm/gtc/type_ptr.hpp"




constinit int windowWidth = 800;
constinit int windowHeight = 800;

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
void init() {
    glClearColor(0.0, 0.0, 1.0, 0.0);
    glViewport(0, 0, windowWidth, windowHeight);
    glm::ortho(0.0f, 100.0f, 0.0f, 100.0f, 0.0f, 0.0f);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColorP3ui

}

void loop(GLFWwindow *p_window) {
    init();
    while (!glfwWindowShouldClose(p_window)) {

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