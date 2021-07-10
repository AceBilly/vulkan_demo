//
// Created by yhtse on 7/10/2021.
//

#ifndef VULKAN_DEMO_VULKANAPPLICATION_H
#define VULKAN_DEMO_VULKANAPPLICATION_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>

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
    void extensionInfo();
    void cleanUp();
    void mainLoop();

    bool isDeviceSuitable(VkPhysicalDevice device);
    void setPhysicalDevice();
private:
    VkInstance m_instance;
    VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
    GLFWwindow *m_window = nullptr;
};


#endif //VULKAN_DEMO_VULKANAPPLICATION_H
