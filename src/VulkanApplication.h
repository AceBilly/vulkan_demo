//
// Created by yhtse on 7/10/2021.
//

#ifndef VULKAN_DEMO_VULKANAPPLICATION_H
#define VULKAN_DEMO_VULKANAPPLICATION_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>


// debug 模式中启用 vulkan 验证层
// 并没有完全开启
const std::vector<const char*> validationLayers = {
        "VK_LAYER_LUNARG_standard_validation"
};

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

class VulkanApplication {
public:
    VulkanApplication()=default;
    ~VulkanApplication()=default;
public:
    void run() {
        initWindow();
        initVulkan();
        mainLoop();
        cleanUp();
    }
private:
    void initVulkan();

    void initWindow();

    void createInstance();
    static void extensionInfo();
    void cleanUp();
    void mainLoop();

    // 备用wsl 无法检测到可用显卡驱动
    // 只返回true
    static bool isDeviceSuitable(VkPhysicalDevice device);
    void setPhysicalDevice();
    // 检查是否支持开启验证层
    static bool checkValidationLayersSupport();
private:
    VkInstance m_instance;
    VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
    GLFWwindow *m_window = nullptr;
    VkDebugReportCallbackEXT m_callback;
};


#endif //VULKAN_DEMO_VULKANAPPLICATION_H
