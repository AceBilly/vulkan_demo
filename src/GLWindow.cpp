//
// Created by yhtse on 7/21/2021.
//

#include "GLWindow.h"
#include "Error.h"



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
    glViewport(0, 0, width, height);
}
// 从文件中加载着色器 编译， 并且获得其中着色器变量的位置信息
void Ace::GLWindow::loadShader(const Ace::fs::path &vertexShaderPath, const Ace::fs::path &fragShaderPath,
                               const Ace::fs::path &variablesLocationConfig) {
    mp_shader = std::make_unique<Ace::Shader>(vertexShaderPath, fragShaderPath);
    mp_shader->getVariables(variablesLocationConfig);
}

void Ace::GLWindow::load2DTexture(const Ace::fs::path &texturePath) {
    m_textures.push_back(std::make_shared<Ace::Texture>(texturePath));
}


void Ace::GLWindow::generateVao() {

    glGenVertexArrays(1, &m_VAO);


    glBindVertexArray(m_VAO);
}




void Ace::GLWindow::render() {
    while (!glfwWindowShouldClose(m_window)) {
        reFlashBuffer();
        if (m_textures.size()) {
            int count = 0;
            for (auto& texture : m_textures) {
                texture->bindTextures(count);
                ++count;
            }
        }
        if (mp_shader) {
            mp_shader->use();
            // TODO 在这里开一个服务端线程，接受其他线程或进程更改数值的请求 （性能代价未知）
            glm::mat4 trans = glm::mat4(1.0f);
            float scale = sin(glfwGetTime());
            trans = glm::translate(trans, glm::vec3(0.0f, 0.2f, 0.0f));
            trans = glm::scale(trans, glm::vec3(scale, scale, scale));
            trans = glm::rotate(trans, ((float)glfwGetTime()), glm::vec3(0.0, 0.0, 1.0f));

            mp_shader->setValue("transform", trans);

        }

        glBindVertexArray(m_VAO);
//        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_TRIANGLES, m_vertexCount, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(m_window);
        glfwPollEvents();
//        Ace::Error::getOpenglError();  // debug

    }
}

void Ace::GLWindow::setBackgroundColor(float r, float g, float b, float a) {
    m_background.r = r;
    m_background.g = g;
    m_background.b = b;
    m_background.a = a;
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Ace::GLWindow::load2DTexture(const Ace::fs::path &texturePath, const std::string &textureUniformName) {
    thread_local int textureCount = 0;
    load2DTexture(texturePath);
    mp_shader->setValue(textureUniformName, textureCount);
    ++textureCount;
}

void Ace::GLWindow::reFlashBuffer() {
    auto [r,g,b,a] = m_background;
    setBackgroundColor(r, g, b, a);
}

