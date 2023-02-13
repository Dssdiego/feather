//
// Created by Diego S. Seabra on 09/07/22.
//

#include "VulkanDevice.h"
#include "../../common/Config.h"

// callback functions
static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
        void *pUserData) {
//    std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

    std::stringstream ss;
    ss << "[VULKAN] "  << pCallbackData->pMessage << " | Objects :: " << pCallbackData->pObjects;

    if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)
        Logger::Info(ss.str());

//    if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT)
//        Logger::Debug(ss.str());

    if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
        Logger::Warn(ss.str());

    if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
        Logger::Error(ss.str(), "");

    return VK_FALSE;
}

// TODO: Refactor the code so that we don't use raw pointers. Instead we want to use smart pointers
//       See more here: https://stackoverflow.com/questions/106508/what-is-a-smart-pointer-and-when-should-i-use-one
VulkanDeviceImpl* mVulkanDeviceImpl = nullptr;

//
// Initialization/Destruction
//

void VulkanDevice::Init()
{
    mVulkanDeviceImpl = new VulkanDeviceImpl;
}

void VulkanDevice::Shutdown()
{
    delete mVulkanDeviceImpl;
}

//
// External
//

VkCommandBuffer VulkanDevice::BeginSingleTimeCommands()
{
    return mVulkanDeviceImpl->BeginSingleTimeCommands();
}

void VulkanDevice::EndSingleTimeCommands(VkCommandBuffer commandBuffer)
{
    mVulkanDeviceImpl->EndSingleTimeCommands(commandBuffer);
}

VkInstance VulkanDevice::GetInstance()
{
    return mVulkanDeviceImpl->instance;
}

VkCommandPool VulkanDevice::GetCommandPool()
{
    return mVulkanDeviceImpl->commandPool;
}

VkDevice VulkanDevice::GetDevice()
{
    return mVulkanDeviceImpl->device;
}

VkPhysicalDevice VulkanDevice::GetPhysicalDevice()
{
    return mVulkanDeviceImpl->physicalDevice;
}

VkSurfaceKHR VulkanDevice::GetSurface()
{
    return mVulkanDeviceImpl->surface;
}

VkQueue VulkanDevice::GetGraphicsQueue()
{
    return mVulkanDeviceImpl->graphicsQueue;
}

VkQueue VulkanDevice::GetPresentQueue()
{
    return mVulkanDeviceImpl->presentQueue;
}

uint32_t VulkanDevice::GetGraphicsQueueFamilyIdx()
{
    return mVulkanDeviceImpl->FindPhysicalQueueFamilies().graphicsFamily;
}

uint32_t VulkanDevice::GetPresentQueueFamilyIdx()
{
    return mVulkanDeviceImpl->FindPhysicalQueueFamilies().presentFamily;
}

SwapChainSupportDetails VulkanDevice::GetSwapChainSupport()
{
    return mVulkanDeviceImpl->QuerySwapChainSupport(mVulkanDeviceImpl->physicalDevice);
}

VkPhysicalDeviceProperties VulkanDevice::GetProperties()
{
    return mVulkanDeviceImpl->GetProperties();
}

void VulkanDevice::CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
                                VkBuffer &buffer, VkDeviceMemory &bufferMemory)
{
    mVulkanDeviceImpl->CreateBuffer(size, usage, properties, buffer, bufferMemory);
}

uint32_t VulkanDevice::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
    return mVulkanDeviceImpl->FindMemoryType(typeFilter, properties);
}

QueueFamilyIndices VulkanDevice::FindPhysicalQueueFamilies()
{
    return mVulkanDeviceImpl->FindPhysicalQueueFamilies();
}

//
// Implementation
//

VulkanDeviceImpl::VulkanDeviceImpl()
{
    CreateInstance();
    SetupDebugMessenger();
    CreateSurface();
    PickPhysicalDevice();
    CreateLogicalDeviceAndQueues();
    CreateCommandPool();
}

VulkanDeviceImpl::~VulkanDeviceImpl()
{
    Logger::Debug("Destroying command pool");
    vkDestroyCommandPool(device, commandPool, nullptr);

    Logger::Debug("Destroying device");
    vkDestroyDevice(device, nullptr);

    if (enableValidationLayers) {
        Logger::Debug("Destroying debug messenger");
        DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
    }

    Logger::Debug("Destroying surface");
    vkDestroySurfaceKHR(instance, surface, nullptr);

    Logger::Debug("Destroying instance");
    vkDestroyInstance(instance, nullptr);
}

void VulkanDeviceImpl::CreateInstance()
{
    if (enableValidationLayers && !CheckValidationLayerSupport())
    {
        Logger::Error("validation layers requested, but not available", "");
        throw std::runtime_error("validation layers requested, but not available");
    }

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = Config::GetEngineName().c_str(); // TODO: Game name should go here!
    appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
    appInfo.pEngineName = Config::GetEngineVersion().c_str();
    appInfo.engineVersion = VK_MAKE_VERSION(0,0,0);
    appInfo.apiVersion = VK_MAKE_VERSION(1, 0, VK_HEADER_VERSION);

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    auto glfwExtensions = Window::GetRequiredExtensions();

    // enabling debug extensions if we have validation layers enabled
    if (enableValidationLayers) {
        glfwExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    createInfo.enabledExtensionCount = static_cast<uint32_t>(glfwExtensions.size());
    createInfo.ppEnabledExtensionNames = glfwExtensions.data();

    if (enableValidationLayers)
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    } else {
        createInfo.enabledLayerCount = 0;
        createInfo.pNext = nullptr;
    }

    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> extensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
    std::cout << "Available extensions:\n";

    for (const auto& extension: extensions)
    {
        std::cout << "\t" << extension.extensionName << std::endl;
    }

    VK_CHECK(vkCreateInstance(&createInfo, nullptr, &instance));

    Logger::Debug("Instance created");
}

void VulkanDeviceImpl::CreateSurface()
{
    VK_CHECK(glfwCreateWindowSurface(instance, Window::GetWindow(), nullptr, &surface));

    Logger::Debug("Surface created");
}

void VulkanDeviceImpl::SetupDebugMessenger()
{
    if (!enableValidationLayers) return;

    VkDebugUtilsMessengerCreateInfoEXT createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = DebugCallback;
    createInfo.pUserData = nullptr; // Optional

    VK_CHECK(CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger));

    Logger::Debug("Setup debug messenger");
}

void VulkanDeviceImpl::PickPhysicalDevice()
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if (deviceCount == 0)
    {
        Logger::Error("failed to find GPU's with Vulkan support", "");
        throw std::runtime_error("failed to find GPU's with Vulkan support");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    for (const auto& device: devices)
    {
        if (IsDeviceSuitable(device))
        {
            physicalDevice = device;
            break;
        }
    }

    if (physicalDevice == VK_NULL_HANDLE)
    {
        Logger::Error("failed to find a suitable GPU", "");
        throw std::runtime_error("failed to find a suitable GPU");
    }

    vkGetPhysicalDeviceProperties(physicalDevice, &properties);

    // getting vulkan version
    uint32_t vulkanVersion = properties.apiVersion;
    uint32_t vulkanVMajor = VK_API_VERSION_MAJOR(vulkanVersion);
    uint32_t vulkanVMinor = VK_API_VERSION_MINOR(vulkanVersion);
    uint32_t vulkanVPatch = VK_API_VERSION_PATCH(vulkanVersion);

    std::stringstream ssVulkanVersion;
    ssVulkanVersion << "Vulkan Version: " << vulkanVMajor << "." << vulkanVMinor << "." << vulkanVPatch;

    Logger::Info("GPU: " + std::string(properties.deviceName));
    Logger::Info(ssVulkanVersion.str());

    Logger::Debug("Picked physical device");
}

void VulkanDeviceImpl::CreateLogicalDeviceAndQueues()
{
    QueueFamilyIndices indices = FindQueueFamilies(physicalDevice);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily, indices.presentFamily };

    // assigning a priority queue (0.0f -> 1.0f)
    float queuePriority = 1.0f;
    for (uint32_t queueFamily: uniqueQueueFamilies)
    {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures{};
    deviceFeatures.samplerAnisotropy = VK_TRUE;

    // creating the logical device
    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();

    createInfo.pEnabledFeatures = &deviceFeatures;

    createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();

    // retro compatibility with older implementations
    if (enableValidationLayers)
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    } else
    {
        createInfo.enabledLayerCount = 0;
    }

    // we're now ready to instantiate the logical device
    VK_CHECK(vkCreateDevice(physicalDevice, &createInfo, nullptr, &device));

    // getting the device graphics/present queues
    vkGetDeviceQueue(device, indices.graphicsFamily, 0, &graphicsQueue);
    vkGetDeviceQueue(device, indices.presentFamily, 0, &presentQueue);

    Logger::Debug("Created logical device and queues");
}

void VulkanDeviceImpl::CreateCommandPool()
{
	QueueFamilyIndices queueFamilyIndices = FindPhysicalQueueFamilies();

	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT; // this allows the command buffer to be implicitly reset when 'vkBeginCommandBuffer' is called
	poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily; // since we want to record the commands for drawing, we must use the graphics queue family

	VK_CHECK(vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool));

	Logger::Debug("Command pool created");
}

VkCommandBuffer VulkanDeviceImpl::BeginSingleTimeCommands()
{
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = commandPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    return commandBuffer;
}

void VulkanDeviceImpl::EndSingleTimeCommands(VkCommandBuffer commandBuffer)
{
    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(graphicsQueue);
    vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
}

// TODO: Create VulkanBuffer class for buffer methods?
void VulkanDevice::CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
{
    VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

    VkBufferCopy copyRegion{};
    copyRegion.srcOffset = 0; // optional
    copyRegion.dstOffset = 0; // optional
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    EndSingleTimeCommands(commandBuffer);
}

//
// Helpers
//

QueueFamilyIndices VulkanDeviceImpl::FindQueueFamilies(VkPhysicalDevice device)
{
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto &queueFamily : queueFamilies) {
        if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;
            indices.graphicsFamilyHasValue = true;
        }
        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
        if (queueFamily.queueCount > 0 && presentSupport) {
            indices.presentFamily = i;
            indices.presentFamilyHasValue = true;
        }
        if (indices.isComplete()) {
            break;
        }

        i++;
    }

    return indices;
}

QueueFamilyIndices VulkanDeviceImpl::FindPhysicalQueueFamilies()
{
    return FindQueueFamilies(physicalDevice);
}

bool VulkanDeviceImpl::CheckValidationLayerSupport()
{
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char* layerName: validationLayers)
    {
        bool layerFound = false;

        for (const auto& layerProperties: availableLayers)
        {
            if (strcmp(layerName, layerProperties.layerName) == 0)
            {
                layerFound = true;
                break;
            }
        }

        if (!layerFound)
        {
            return false;
        }
    }

    return true;
}

VkResult VulkanDeviceImpl::CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void VulkanDeviceImpl::DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
                                                     const VkAllocationCallbacks *pAllocator)
{
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}

SwapChainSupportDetails VulkanDeviceImpl::QuerySwapChainSupport(VkPhysicalDevice device)
{
    // querying basic surface capabilities
    SwapChainSupportDetails details;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

    // querying the supported surface formats
    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

    if (formatCount != 0)
    {
        // resize to hold all the available formats
        std::cout << "# of available formats: " << formatCount << std::endl;
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
    }

    // querying the supported presentation modes
    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

    if (presentModeCount != 0)
    {
        // resize to hold all the available present modes
        std::cout << "# of present modes: " << presentModeCount << std::endl;
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());

        for (auto p : details.presentModes)
        {
            std::cout << "Present Mode Available: " << p << std::endl;
        }
    }

    return details;
}

bool VulkanDeviceImpl::IsDeviceSuitable(VkPhysicalDevice device)
{
    QueueFamilyIndices indices = FindQueueFamilies(device);

    bool extensionsSupported = CheckDeviceExtensionSupport(device);
    bool swapChainAdequate = false;

    // we only check for swap chain support AFTER checking that the extension is available
    if (extensionsSupported)
    {
        SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(device);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    VkPhysicalDeviceFeatures supportedFeatures;
    vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

    return indices.isComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
}

bool VulkanDeviceImpl::CheckDeviceExtensionSupport(VkPhysicalDevice device)
{
    uint32_t extensionsCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionsCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionsCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionsCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

    for (const auto& extension : availableExtensions)
    {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

VkPhysicalDeviceProperties VulkanDeviceImpl::GetProperties()
{
    return properties;
}

void VulkanDeviceImpl::CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
                                    VkBuffer &buffer, VkDeviceMemory &bufferMemory)
{
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size; // size of the buffer in bytes
    bufferInfo.usage = usage; // which purposes the data in the buffer is going to be used
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE; // only used by the graphics queue, so it can have exclusive access

    VK_CHECK(vkCreateBuffer(device, &bufferInfo, nullptr, &buffer));

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, properties);
    // TODO: Implement "FindMemoryTipe" method

    VK_CHECK(vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory));

    vkBindBufferMemory(device, buffer, bufferMemory, 0);
}

uint32_t VulkanDeviceImpl::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
    // querying info about the available types of memory
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
    {
        if (typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
        {
            return i;
        }
    }

    throw std::runtime_error("failed to find a suitable memory type");
}
