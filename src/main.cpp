#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <iostream>

int main() {
    if (!glfwInit())
        throw std::runtime_error("Failed to initialize GLFW");

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
    glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Vulkan Smoke Test", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwShowWindow(window);

    // Vulkan instance (minimal)
    VkInstance instance;
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "SmokeTest";
    appInfo.apiVersion = VK_API_VERSION_1_3;

    uint32_t extCount = 0;
    const char** exts = glfwGetRequiredInstanceExtensions(&extCount);

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = extCount;
    createInfo.ppEnabledExtensionNames = exts;

    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
        throw std::runtime_error("Failed to create Vulkan instance");

    std::cout << "Vulkan instance created!\n";

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();  // process window events
    }

    vkDestroyInstance(instance, nullptr);
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}