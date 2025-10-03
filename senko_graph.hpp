#include "dependancies.hpp"
#include "window.hpp"










class SenkoGraph{
public:
    static void init(std::vector<const char*> &exts)
    {

        
        createInstance(exts);
        setupDebugMessenger();
        
       
  

        
    }
    static VkInstance vkInstance;
    enum Result{
        SenkoGraph_Result_Success,
        SenkoGraph_Result_No_VK_Support,
    };
    struct SwapChainDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    //struct GPU{
        
        //VkPhysicalDevice physicalDevice;
   // };

    struct GPUInfo{
        std::vector<VkQueueFamilyProperties> queueFamilies;
        std::vector<VkExtensionProperties> exts;
        VkPhysicalDeviceFeatures features;
    };

    static Result getGpus(std::vector<GPU> &gpus){
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(vkInstance, &deviceCount, nullptr);
        if (deviceCount == 0) {
            return SenkoGraph_Result_No_VK_Support;
        }
        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(vkInstance, &deviceCount, devices.data());

        uint32_t queueFamilyCount;
        uint32_t extensionCount;
        gpus.resize(deviceCount);

        std::vector<GPUInfo> gpusInfo(deviceCount);

        for (int i = 0; i < deviceCount; i++){
            GPU& gpu = gpus[i];
            gpu.physicalDevice = devices[i];
            vkGetPhysicalDeviceQueueFamilyProperties(gpu.physicalDevice, &queueFamilyCount, nullptr);

            gpu.queueFamilies.resize(queueFamilyCount);
            vkGetPhysicalDeviceQueueFamilyProperties(gpu.physicalDevice, &queueFamilyCount, gpu.queueFamilies.data());

            
            queueFamilyCount = 0;

            vkEnumerateDeviceExtensionProperties(gpu.physicalDevice, nullptr, &extensionCount, nullptr);

            gpusInfo[i].exts.resize(extensionCount);
            vkEnumerateDeviceExtensionProperties(gpu.physicalDevice, nullptr, &extensionCount, exts.data());

            extensionCount = 0;

            
            vkGetPhysicalDeviceFeatures(gpu.physicalDevice, &gpusInfo[i].features);


            
        }
        return SenkoGraph_Result_Success;
    }

    static bool getDisplaySupport(GPU& gpu, VkSurfaceKHR* surface, uint32_t& presentQueueFamilyIndex, SwapChainDetails* swapChainDetails){

        VkBool32 presentSupport = false;
        for (int i = 0; i < gpu.queueFamilies.size(); i++){
            vkGetPhysicalDeviceSurfaceSupportKHR(gpu.physicalDevice, i, *surface, &presentSupport);
            if (presentSupport){
                presentQueueFamilyIndex = i;
                break;
            }
            
        }

        if (presentSupport == false){
            return false;
        }

       
      ;

        bool hasExt;
        for (const auto& extension : gpu.exts) {

            if (!strcmp(extension.extensionName,VK_KHR_SWAPCHAIN_EXTENSION_NAME)){
                hasExt = true;
            }
        }

        if (!hasExt){
            return false;
        }

        gpu.useExts.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(gpu.physicalDevice, *surface, &swapChainDetails->capabilities);

        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(gpu.physicalDevice, *surface, &formatCount, nullptr);

        if (formatCount == 0){
            return false;
        }
        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(gpu.physicalDevice, *surface, &presentModeCount, nullptr);

        if (presentModeCount == 0){
            return false;
        }
        swapChainDetails->formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(gpu.physicalDevice, *surface, &formatCount, swapChainDetails->formats.data());

        swapChainDetails->presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(gpu.physicalDevice, *surface, &presentModeCount, swapChainDetails->presentModes.data());


        return true;

        
    }

private:
    
    static VkDebugUtilsMessengerEXT debugMessenger;


    
   
    
    
    
    static VkResult createInstance(std::vector<const char*> &exts) {

        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Hello Triangle";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;


        createInfo.enabledExtensionCount = static_cast<uint32_t>(exts.size());
        createInfo.ppEnabledExtensionNames = exts.data();

        createInfo.enabledLayerCount = 0;
        createInfo.pNext = nullptr;

        
       
        setupCreateInstanceDebug(createInfo,exts);
         

    
        return vkCreateInstance(&createInfo, nullptr, &vkInstance);

   

    }

    static VkDebugUtilsMessengerCreateInfoEXT setupCreateInstanceDebug(VkInstanceCreateInfo& createInfo, std::vector<const char *> &exts){

        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        exts.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        createInfo.enabledExtensionCount += 1;
        createInfo.ppEnabledExtensionNames = exts.data();

        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();

        populateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = &debugCreateInfo;
   
         
       return debugCreateInfo;
    }
    static VkResult setupDebugMessenger() {

        VkDebugUtilsMessengerCreateInfoEXT createInfo{};
        populateDebugMessengerCreateInfo(createInfo);

       
        return CreateDebugUtilsMessengerEXT(vkInstance, &createInfo, nullptr, &debugMessenger);

    }

    static VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
        if (func != nullptr) {
            return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
        } else {
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
    }

    static void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
        //createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = debugCallback;
    }

    

    static const std::vector<const char*> validationLayers;

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
        std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

        return VK_FALSE;
    }
};
const std::vector<const char*> SenkoGraph::validationLayers  = {
    "VK_LAYER_KHRONOS_validation"
};
VkDebugUtilsMessengerEXT SenkoGraph::debugMessenger;
VkInstance SenkoGraph::vkInstance;

