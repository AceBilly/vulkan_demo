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


Ace::GLWindow::GLWindow(uint width, uint height, const std::string &winTitle) : m_windowWidth(width),
                                                                                m_windowHeight(height) {
    using namespace std::placeholders;
    initialize_glfw();
    createWindow(width, height, winTitle);
    initialize_glad();
    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
    glfwSetFramebufferSizeCallback(m_window, &frameBuffer_size_callback);
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
    // 另外的十个正方体
    glm::vec3 cubePositions[] = {
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(2.0f, 5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f),
            glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3(2.4f, -0.4f, -3.5f),
            glm::vec3(-1.7f, 3.0f, -7.5f),
            glm::vec3(1.3f, -2.0f, -2.5f),
            glm::vec3(1.5f, 2.0f, -2.5f),
            glm::vec3(1.5f, 0.2f, -1.5f),
            glm::vec3(-1.3f, 1.0f, -1.5f)
    };
    // 摄像机所需变量
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);  // 摄像机位置
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);  // 摄像机前面
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);  // 摄像机的上轴

    auto process = [&]() {
        float deltaTime = 0.0f; // 当前帧与上一帧的时间差
        float lastFrame = 0.0f; // 上一帧的时间
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        float cameraSpeed = 0.05; // adjust accordingly
        if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
            cameraPos += cameraSpeed * cameraFront;
        if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
            cameraPos -= cameraSpeed * cameraFront;
        if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    };

    while (!glfwWindowShouldClose(m_window)) {
        reFlashBuffer();
        if (m_textures.size()) {
            int count = 0;
            for (auto &texture : m_textures) {
                texture->bindTextures(count);
                ++count;
            }
        }
        if (mp_shader) {
            mp_shader->use();
            getWindowSize();
            // TODO 在这里开一个服务端线程，接受其他线程或进程更改数值的请求 （性能代价大）
            glm::mat4 view = glm::mat4(1.0f);
            glm::mat4 projection = glm::mat4(1.0f);

            view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
            projection = glm::perspective(45.0f, (float) m_windowWidth / (float) m_windowHeight, 0.1f, 100.0f);
            mp_shader->setValue("view", view);
            mp_shader->setValue("projection", projection);
        }

        glBindVertexArray(m_VAO);
        for (auto &cubePosition : cubePositions) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePosition);
            model = glm::rotate(model, (float) (glfwGetTime()), glm::vec3(1.0f, 0.3f, 0.5f));
            mp_shader->setValue("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
//        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//        glDrawElements(GL_TRIANGLES, m_vertexCount, GL_UNSIGNED_INT, 0);

        process();
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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Ace::GLWindow::load2DTexture(const Ace::fs::path &texturePath, const std::string &textureUniformName) {
    thread_local int textureCount = 0;
    load2DTexture(texturePath);
    mp_shader->setValue(textureUniformName, textureCount);
    ++textureCount;
}

void Ace::GLWindow::reFlashBuffer() {
    auto[r, g, b, a] = m_background;
    setBackgroundColor(r, g, b, a);
}

void Ace::GLWindow::frameBuffer_size_callback(Ace::GLWindow::window_t *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void Ace::GLWindow::getWindowSize() {
    glfwGetWindowSize(m_window, &m_windowWidth, &m_windowHeight);
}

void Ace::GLWindow::processInput() const {

}

