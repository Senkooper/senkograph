#include <glfw-3.4.bin.WIN64\include\GLFW\glfw3.h>
#include <vector>

GLFWwindow* createWindow(uint32_t initialWidth, uint32_t initialHeight, const char * title){
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    return glfwCreateWindow(initialWidth, initialHeight, title, nullptr, nullptr);
};

std::vector<const char*> getRequiredWindowExtensions() {
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    return extensions;
}