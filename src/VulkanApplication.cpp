//
// Created by yhtse on 7/10/2021.
//

#include "VulkanApplication.h"
#include <string>

// 窗口大小
constinit int window_width = 800;
constinit int windows_height = 600;

// 验证GPU设备是否可用，
// @returns true 开发环境目前显卡不可检测
bool VulkanApplication::isDeviceSuitable(VkPhysicalDevice device) {
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
    return true;
}

bool VulkanApplication::checkValidationLayersSupport() {
    uint32_t layerCount = 0;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
    std::vector<VkLayerProperties> layersProperties(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, layersProperties.data());
    for (const char *layerName : validationLayers) {
        for (const auto &layerProperties : layersProperties) {
            if (std::string(layerName) == std::string(layerProperties.layerName)) {
                return true;
            }
        }
    }
    return false;
}

void VulkanApplication::setPhysicalDevice() {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);
    if (deviceCount == 0)
        throw std::runtime_error("failed to find GPUS with Vulkan Support");
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(m_instance, &deviceCount, devices.data());
    for (const auto &device : devices) {
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

std::vector<const char *> getRequireExtension() {
    std::vector<const char *> extension;
    uint glfwExtensionCount = 0;
    const char **glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    for(uint i = 0;i < glfwExtensionCount; ++i) {
        extension.push_back(glfwExtensions[i]);
    }
    if(enableValidationLayers) {
        extension.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
    }
    return extension;
}
// validation layer callback function
static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugReportFlagsEXT flags,
        VkDebugReportObjectTypeEXT objType,
        uint64_t obj,
        size_t location,
        int32_t code,
        const char* layerPrefix,
        const char* msg,
        void* userData) {

    std::cerr << "validation layer: " << msg << std::endl;

    return VK_FALSE;
}
void VulkanApplication::createInstance() {
    if (enableValidationLayers && !checkValidationLayersSupport()) {
        throw std::runtime_error("validation layers requested, but not avaliable");
    }
    VkApplicationInfo appInfo = {
            .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .pNext = nullptr,
            .pApplicationName = "Hello Triangle",
            .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
            .pEngineName = "No engine",
            .engineVersion = VK_MAKE_VERSION(1, 0, 0),
            .apiVersion = VK_API_VERSION_1_0
    };
    auto extensions = getRequireExtension();
    VkInstanceCreateInfo instanceCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            .pApplicationInfo = &appInfo, .enabledLayerCount = enableValidationLayers
                                                               ? static_cast<uint32_t>(validationLayers.size()) : 0,
            .ppEnabledLayerNames = validationLayers.data(),
            .enabledExtensionCount = static_cast<uint32_t>(extensions.size()),
            .ppEnabledExtensionNames = extensions.data()

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