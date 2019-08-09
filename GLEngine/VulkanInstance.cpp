#include "pch.h"
#include "VulkanInstance.h"

VulkanInstance::VulkanInstance() {}


VulkanInstance::~VulkanInstance() {}

void VulkanInstance::createAppAndVkInstance(bool enableValidationLayers, AppValidationLayersAndExtensions *valLayersAndExtentions)
{
    //불칸 인스턴스를 생성하려면 어플리케이션 정보와 인스턴스 생성정보가 필요하다.
    // links the application to the Vulkan library

    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Simple Vulkan";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "VulkanEngine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;


    VkInstanceCreateInfo vkInstanceInfo = {};
    vkInstanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    vkInstanceInfo.pApplicationInfo = &appInfo;

    // specify extensions and validation layers
    // these are global meaning they are applicable to whole program not just the device

    auto extensions = valLayersAndExtentions->getRequiredExtensions(enableValidationLayers);
    vkInstanceInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());;
    vkInstanceInfo.ppEnabledExtensionNames = extensions.data();

    if (enableValidationLayers)
    {
        vkInstanceInfo.enabledLayerCount = static_cast<uint32_t>(valLayersAndExtentions->requiredValidationLayers.size());
        vkInstanceInfo.ppEnabledLayerNames = valLayersAndExtentions->requiredValidationLayers.data();
    }
    else
    {
        vkInstanceInfo.enabledLayerCount = 0;
    }

    //불칸 인스턴스 생성
    if (vkCreateInstance(&vkInstanceInfo, nullptr, &vkInstance))
    {
        throw std::runtime_error("failed to create vkInstance ");
    }

} // create instance
