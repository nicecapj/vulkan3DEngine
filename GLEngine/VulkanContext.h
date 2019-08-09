#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW\glfw3.h>

#include <vulkan\vulkan.h>

#include "AppValidationLayersAndExtensions.h"
#include "VulkanInstance.h"
#include "Device.h"


#ifdef _DEBUG
const bool isValidationLayersEnabled = true;
#else
const bool isValidationLayersEnabled = false;
#endif

//불칸 렌더러를 만드는 기능을 가진 클래스
//VK인스턴스와 윈도우서페이스, 물리장치를 접근하도록 설정
class VulkanContext
{

public:

    static VulkanContext* getInstance();
    static VulkanContext* instance;

    ~VulkanContext();

    void initVulkan(GLFWwindow* window);


private:
    //surface
    VkSurfaceKHR surface;

    // My Classes
    AppValidationLayersAndExtensions *valLayersAndExt;
    VulkanInstance* vInstance;
    Device* device;


};


