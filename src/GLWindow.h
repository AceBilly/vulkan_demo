//
// Created by yhtse on 7/21/2021.
//

#ifndef VULKAN_DEMO_GLWINDOW_H
#define VULKAN_DEMO_GLWINDOW_H


#include <memory>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.h"


namespace Ace {
    class GLWindow {
    public:
        using window_t = GLFWwindow;
    public:
        GLWindow(uint width, uint height, const std::string &winTitle);

    private:
        void createWindow(uint width, uint height, const std::string &winTitle);
        void loadShader(const Ace::fs::path& vertexShaderPath, const Ace::fs::path& fragShader, const Ace::fs::)
    private:
        window_t *m_window = nullptr;
        std::unique_ptr<Ace::Shader> *mp_shader =nullptr
    };
}  // namespace Ace;



#endif //VULKAN_DEMO_GLWINDOW_H
