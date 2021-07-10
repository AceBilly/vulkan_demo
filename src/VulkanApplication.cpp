//
// Created by yhtse on 7/10/2021.
//

#include "VulkanApplication.h"
#include <vector>

constinit int window_width = 800;
constinit int windows_height = 600;

bool VulkanApplication::isDeviceSuitable(VkPhysicalDevice device){
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
    return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
           deviceFeatures.geometryShader;
}

void VulkanApplication::setPhysicalDevice() {
    uint32_t  deviceCount = 0;
    vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);
    if (deviceCount == 0)
        throw std::runtime_error("failed to find GPUS with Vulkan Support");
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(m_instance, &deviceCount, devices.data());
    for (const auto& device : devices) {
        if (isDeviceSuitable(device)) {
            m_physicalDevice = device;
            break;
        }
    }
    if (m_physicalDevice == VK_NULL_HANDLE) {
        throw std::runtime_error("failed to find a suitable GPU");
    }
}

void VulkanApplication::initWindow() {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    m_window = glfwCreateWindow(window_width, windows_height, "VulkanDemo0", nullptr, nullptr);
    if (m_window == nullptr)
        throw std::runtime_error("failed create window");
}

void VulkanApplication::initVulkan() {
    createInstance();
    setPhysicalDevice();
}

void VulkanApplication::mainLoop() {
    while (!glfwWindowShouldClose(m_window)) {
        glfwPollEvents();
    }
}

void VulkanApplication::createInstance() {
    VkApplicationInfo appInfo = {
            .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .pNext = nullptr,
            .pApplicationName = "Hello Triangle",
            .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
            .pEngineName = "No engine",
            .engineVersion = VK_MAKE_VERSION(1, 0, 0),
            .apiVersion = VK_API_VERSION_1_0
    };
    uint glfwExtensionCount = 0;
    const char **glfwExtension = nullptr;
    glfwExtension = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    VkInstanceCreateInfo instanceCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            .pApplicationInfo = &appInfo, .enabledLayerCount = 0,
            .enabledExtensionCount = glfwExtensionCount,
            .ppEnabledExtensionNames = glfwExtension

    };
    if (vkCreateInstance(&instanceCreateInfo, nullptr, &m_instance) != VK_SUCCESS)
        throw std::runtime_error("failed to create instance");
    extensionInfo();
}

void VulkanApplication::extensionInfo() {
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> extension(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extension.data());
    std::cout << "avaliable extension: " << std::endl;
    for (auto const &entry : extension) {
        std::cout << "\t" << entry.extensionName << std::endl;
    }
}

void VulkanApplication::cleanUp() {
    vkDestroyInstance(m_instance, nullptr);

    glfwDestroyWindow(m_window);

    glfwTerminate();
}