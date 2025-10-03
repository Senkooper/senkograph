#include "senko_graph.hpp"


GLFWwindow* window;
VkSurfaceKHR surface;
std::vector<SenkoGraph::GPU> gpus;
uint32_t presentQueueFamilyIndex;
uint32_t graphicsQueueFamilyIndex;
SenkoGraph::SwapChainDetails swapChainDetails;

int main(){


    try{
        glfwInit();
        window = createWindow(800,600,"SENKOOPERS ENGINE");
        std::vector<const char*> windowExts = getRequiredWindowExtensions();
        SenkoGraph::init(windowExts);
        if (glfwCreateWindowSurface(SenkoGraph::vkInstance, window, nullptr, &surface) != VK_SUCCESS) {
            throw std::runtime_error("failed to create window surface!");
        }
        //SenkoGraph::getGpus(gpus);
        if (SenkoGraph::getGpus(gpus)){
            throw std::runtime_error("failed to find gpus with vulkan support");
        }

        std::cerr << gpus.size();


        for (int i = 0; i < gpus.size(); i++){
            for (int qi = 0; qi < gpus[i].queueFamilies.size(); qi++){
                VkQueueFamilyProperties& queueFamily = gpus[i].queueFamilies[qi];
                if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                    graphicsQueueFamilyIndex = qi;
                    
                    if (gpus[i].features.samplerAnisotropy){
                        std::cout << "\nssa" <<  SenkoGraph::getDisplaySupport(gpus[i],&surface,presentQueueFamilyIndex,&swapChainDetails) << "kk" << presentQueueFamilyIndex << graphicsQueueFamilyIndex;
                        break;
                    }
                    
                }
            }
            
        }
       
    }catch(std::string msg){
        std::cerr << msg;
    }
    
  
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        //drawFrame();
    }
    return 0;
}