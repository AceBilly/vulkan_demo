#include <iostream>
#include <exception>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include "../inc/glm/glm.hpp"
#include "../inc/glm/gtc/matrix_transform.hpp"
#include "../inc/glm/gtc/type_ptr.hpp"




constinit int windowWidth = 800;
constinit int windowHeight = 800;

constinit float lineSegment[] = {1.0f, 1.0f,
                                 50.0f, 50.0f
                                 };

const char* vertexShaderSrc  = "#version 460 core\n"
                                  "layout (location = 0) in vec3 aPos\n"
                                  "void main()\n"
                                  "{\n"
                                  "gl_Position = vec4(aPos.x, aPos.y, 1.0, 1.0\n"
                                  "}\0";
const char* fragShaderSrc = "#version 460 core\n"
                                  "out vec4 FragColor;\n"
                                  "\n"
                                  "void main()\n"
                                  "{\n"
                                  "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
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
void init() {
    glClearColor(0.0, 0.0, 1.0, 0.0);
    glViewport(0, 0, windowWidth, windowHeight);
    glm::ortho(0.0f, 100.0f, 0.0f, 100.0f, 0.0f, 0.0f);



    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSrc, nullptr);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragShaderSrc, nullptr);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, vertexShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lineSegment), lineSegment, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(0));
    glEnableVertexAttribArray(0);
    glUseProgram(shaderProgram);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
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