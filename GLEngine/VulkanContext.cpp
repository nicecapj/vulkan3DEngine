#include "pch.h"
#include "VulkanContext.h"
#include "Device.h"

VulkanContext* VulkanContext::instance = NULL;


VulkanContext* VulkanContext::getInstance()
{

    if (!instance)
    {
        instance = new VulkanContext();
    }
    return instance;
}

VulkanContext::~VulkanContext()
{

    if (instance)
    {
        delete instance;
        instance = nullptr;
    }
}

void VulkanContext::initVulkan(GLFWwindow* window)
{
    // Validation and Extension Layers
    // 에러 체크용
    valLayersAndExt = new AppValidationLayersAndExtensions();

    if (isValidationLayersEnabled && !valLayersAndExt->checkValidationLayerSupport())
    {
        throw std::runtime_error("validation Layers Not Available !");
    }

    // Create App And Vulkan Instance()
    vInstance = new VulkanInstance();
    vInstance->createAppAndVkInstance(isValidationLayersEnabled, valLayersAndExt);

    // Debug CallBack
    valLayersAndExt->setupDebugCallback(isValidationLayersEnabled, vInstance->vkInstance);

    // Create Surface
    // 현재 플랫폼에서 랜더한 이미지를 표시하기 위해 윈도우에 표면(서페이스)을 생성한다.
    if (glfwCreateWindowSurface(vInstance->vkInstance, window, nullptr, &surface) != VK_SUCCESS)
    {
        throw std::runtime_error(" failed to create window surface !");
    }

    // PickPhysicalDevice and CreateLogicalDevice();
    // 스왑체인을 하려면 물리디바이스가 필요하고, 물리디바이스에 실제 처리는 논리적으로, 논리디바이스로 접근한다.
    //
    device = new Device();
    device->pickPhysicalDevice(vInstance, surface);
    device->createLogicalDevice(surface, isValidationLayersEnabled, valLayersAndExt);

}


