#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW\glfw3.h>

#include <vulkan\vulkan.h>

#include "AppValidationLayersAndExtensions.h"
#include "VulkanInstance.h"
#include "Device.h"
#include "SwapChain.h"
#include "DrawCommandBuffer.h"


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
	void DrawBegin();
	void DrawEnd();
	
	Device* GetDevice();

	void CleanUp();

private:
    //surface
    VkSurfaceKHR surface;	//플랫폼에 종속, 랜더링된 이미지를 그릴수 있는 표면	

    //문제발생시 체크용
    AppValidationLayersAndExtensions *valLayersAndExt;

	//실제 불칸인스턴스와 물리장치 접근 클래스(디바이스)
    VulkanInstance* vInstance;
    Device* device;

	SwapChain* swapChain_;
	uint32_t imageIndex_ = 0;

	VkCommandBuffer currentCommandBuffer_;
	DrawCommandBuffer* drawCommandBuffer_;
};


