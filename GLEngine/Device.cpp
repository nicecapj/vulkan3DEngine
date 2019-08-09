#include "pch.h"
#include "Device.h"




Device::Device() {}

Device::~Device() {}

void Device::pickPhysicalDevice(VulkanInstance* vInstance, VkSurfaceKHR surface)
{

    uint32_t deviceCount = 0;

    vkEnumeratePhysicalDevices(vInstance->vkInstance, &deviceCount, nullptr);

    if (deviceCount == 0)
    {
        throw std::runtime_error("failed to find GPUs with vulkan support !");
    }

    std::cout << "Device Count: " << deviceCount << std::endl;

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(vInstance->vkInstance, &deviceCount, devices.data());


    std::cout << std::endl;
    std::cout << "DEVICE PROPERTIES" << std::endl;
    std::cout << "=================" << std::endl;

    for (const auto& device : devices)
    {

        VkPhysicalDeviceProperties  deviceProperties;

        vkGetPhysicalDeviceProperties(device, &deviceProperties);

        std::cout << std::endl;
        std::cout << "Device name: " << deviceProperties.deviceName << std::endl;

        if (isDeviceSuitable(device, surface))
        {
            physicalDevice = device;
        }

        break;

    }

    if (physicalDevice == VK_NULL_HANDLE)
    {
        throw std::runtime_error("failed to find suitable GPU !");
    }

}

bool Device::isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface)
{

    // find queue families the device supports

    QueueFamilyIndices qFamilyIndices = findQueueFamilies(device, surface);

    // Check device extentions supported
    // 1. 먼저 해당 디바이스가 불칸지원(확장)을 지원하는지 체크한다.
    bool extensionSupported = checkDeviceExtensionSupported(device);

    bool swapChainAdequate = false;

    // If swapchain extension is present
    // Check surface formats and presentation modes are supported
    //2. 스왑체인(더블버퍼링)을 지원하는지 체크하기 위해, 서페이스포맷과 표시(프리젠테이션) 모드를 지원하는지 체크한다.
    if (extensionSupported)
    {
        swapchainSupport = querySwapChainSupport(device, surface);
        swapChainAdequate = !swapchainSupport.surfaceFormats.empty() && !swapchainSupport.presentModes.empty();

    }

    VkPhysicalDeviceFeatures supportedFeatures;
    vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

    //samplerAnisotropy는 이방성 필터링인데, 지원하게 되면, 계단화(pixelated)되거나 흐려지는(blurry)것을 개선해준다.
    return qFamilyIndices.arePresent() && extensionSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;

}


// queue families
// select the family that supports graphics.
// 그래픽큐와 프레젠스 큐의 인덱스를 찾아서 리턴한다.
QueueFamilyIndices Device::findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface)
{
    uint32_t queueFamilyCount = 0;

    //디바이스가 지원하는 큐 종류 갯수를 얻기 위해 한번 호출
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);

    //큐 종류의 프로퍼티를 얻기 위해 다시 갯수와, VkQueueFamilyProperties data로 넘겨서 호출(큐수, graphics큐(VK_QUEUE_GRAPHICS_BIT)인지)
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    //큐 종류들 중에서, bit플래그로 graphics큐 찾는다.
    //GEFORCE2080TI의 경우 3개의 큐패밀리
    int i = 0;
    for (const auto& queueFamily : queueFamilies)
    {
        //queueCount 지원하는 큐수
        //queueFlags 큐의 종류(VK_QUEUE_GRAPHICS_BIT , VK_QUEUE_COMPUTE_BIT, VK_QUEUE_TRANSFER_BIT, VK_QUEUE_SPARSE_BINDING_BIT...)
        //보드 통합 그래픽카드의 경우는 VK_QUEUE_GRAPHICS_BIT와 프리젠테이션 큐(vkGetPhysicalDeviceSurfaceSupportKHR())를  동시에 지원하기도 한다.
        if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            queueFamiliyIndices.graphicsFamily = i;
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

        if (queueFamily.queueCount > 0 && presentSupport)
        {
            queueFamiliyIndices.presentFamily = i;
        }


        if (queueFamiliyIndices.arePresent())
        {
            break;
        }

        i++;
    }


    return queueFamiliyIndices;
}

bool Device::checkDeviceExtensionSupported(VkPhysicalDevice device)
{

    uint32_t extensionCount;

    // Get available device extentions count
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    // Get available device extentions
    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    // Populate with required device exentions we need
    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

    // Check if the required extention is present
    for (const auto& extension : availableExtensions)
    {
        requiredExtensions.erase(extension.extensionName);
    }

    // If device has the required device extention then return
    return requiredExtensions.empty();
}

SwapChainSupportDetails Device::querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface)
{

    SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.surfaceCapabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

    if (formatCount != 0)
    {
        details.surfaceFormats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.surfaceFormats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

    if (presentModeCount != 0)
    {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
    }

    return details;
}

QueueFamilyIndices Device::getQueueFamiliesIndicesOfCurrentDevice()
{

    return queueFamiliyIndices;
}


// ++++++++++++++
// Logical device
// ++++++++++++++

void Device::createLogicalDevice(VkSurfaceKHR surface, bool isValidationLayersEnabled, AppValidationLayersAndExtensions *appValLayersAndExtentions)
{

    // find queue families like graphics and presentation
    QueueFamilyIndices indices = findQueueFamilies(physicalDevice, surface);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<int> uniqueQueueFamilies = { indices.graphicsFamily, indices.presentFamily };

    float queuePriority = 1.0f;

    for (int queueFamily : uniqueQueueFamilies)
    {
        VkDeviceQueueCreateInfo queueCreateInfo = {};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1; // we only require 1 queue
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    //specify device features
    VkPhysicalDeviceFeatures deviceFeatures = {}; // we dont specify anything now

    deviceFeatures.samplerAnisotropy = VK_TRUE;	//이방성 필터링 킨다. 이게 불칸에 그리 중요한가 책에서 강조함


    VkDeviceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = queueCreateInfos.data();;
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());

    createInfo.pEnabledFeatures = &deviceFeatures;


    // enable device extentension and validation layers
    //printf("deviceExtensionSize: %d \n", deviceExtensions.size());

    createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();

    if (isValidationLayersEnabled)
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(appValLayersAndExtentions->requiredValidationLayers.size());
        createInfo.ppEnabledLayerNames = appValLayersAndExtentions->requiredValidationLayers.data();
    }
    else
    {
        createInfo.enabledLayerCount = 0;
    }


    //create logical device
    //물리장치 접근을 위한 논리 장치 생성
    if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &logicalDevice) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create logical device !");
    }

    //get handle to the graphics queue of the gpu
    //논리장치에서 gpu의 그래픽 큐 얻는다.
    vkGetDeviceQueue(logicalDevice, indices.graphicsFamily, 0, &graphicsQueue);

    //get handle to the presentation queue of the gpu
    //논리장치에서 gpu의 프레젠트 큐 얻는다.
    vkGetDeviceQueue(logicalDevice, indices.presentFamily, 0, &presentQueue);

}

void Device::destroy()
{
    vkDestroyDevice(logicalDevice, nullptr);
}


