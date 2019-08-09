#pragma once

#include <vulkan\vulkan.h>
#include <vector>
#include <iostream>

#define GLFW_INCLUDE_VULKAN
#include <GLFW\glfw3.h>

//어플리케이션 오류확인과, 확장기능 체크용 클래스
class AppValidationLayersAndExtensions
{

public:
    AppValidationLayersAndExtensions();
    ~AppValidationLayersAndExtensions();

    const std::vector<const char*> requiredValidationLayers =
    {
        "VK_LAYER_LUNARG_standard_validation"
    };

    bool checkValidationLayerSupport();
    std::vector<const char*> getRequiredExtensions(bool isValidationLayersEnabled);


    // Debug Callback
    VkDebugReportCallbackEXT callback;

    void setupDebugCallback(bool isValidationLayersEnabled, VkInstance vkInstance);
    void destroy(VkInstance instance, bool isValidationLayersEnabled);


    // Callback

    VkResult createDebugReportCallbackEXT(
        VkInstance instance,
        const VkDebugReportCallbackCreateInfoEXT* pCreateInfo,
        const VkAllocationCallbacks* pAllocator,
        VkDebugReportCallbackEXT* pCallback)
    {

        auto func = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");

        if (func != nullptr)
        {
            return func(instance, pCreateInfo, pAllocator, pCallback);
        }
        else
        {
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }

    }

    void DestroyDebugReportCallbackEXT(
        VkInstance instance,
        VkDebugReportCallbackEXT callback,
        const VkAllocationCallbacks* pAllocator)
    {

        auto func = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
        if (func != nullptr)
        {
            func(instance, callback, pAllocator);
        }
    }

};

