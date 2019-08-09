#pragma once

#include <vulkan\vulkan.h>
#include <stdexcept>

#include <iostream>
#include <vector>
#include <set>

#include "VulkanInstance.h"
#include "AppValidationLayersAndExtensions.h"

struct SwapChainSupportDetails
{

    VkSurfaceCapabilitiesKHR surfaceCapabilities; // size and images in swapchain
    std::vector<VkSurfaceFormatKHR> surfaceFormats;
    std::vector<VkPresentModeKHR> presentModes;
};

//QUEUE는 graphics -> compute -> present 3개의 큐가 있다.
//GPU로 향하는 커맨드(COMMANDS)들은 큐를 사용하여 실행된다.
//랜더링은 랜더링 큐로, 계산은 CPOMPUTE큐로, 표시할것은 PRESENT큐로 보낸다
struct QueueFamilyIndices
{
    int graphicsFamily = -1;	//GPU 큐종류에서, 그래픽스 큐를 핸들링, 메모리 전송하는 COMPUTE큐랑 분리한다.
    int presentFamily = -1;		//프레임버퍼 표시 큐

    bool arePresent()
    {
        return graphicsFamily >= 0 && presentFamily >= 0;
    }
};

//랜더링 하기 위해 사용하는 실제 물리 장치(불칸 호환이 되야 한다)
//GPU Vendor사이트 확인할것
//geforce 600이상이나 Radeon HD2000이상이면 보통 지원
//더블버퍼링 : 랜더링된 현재 프레임은 화면(현재 서페이스)으로 보내지고, 출력되게 되고
//다음 프레임은 RENDER가 끝나면 SWAP을 통해, 현재 서페이스로 보내지고, 화면에 그려진다.
//이렇게 안하고, 바로 서페이스로 보내서 그리게 되면, 화면이 반만 그려져 있거나 찢어지는 스크린 티어링(SCREEN TEARING)이 발생한다.
//물리장치는 그래픽 카드이고, 실제 사용은 논리장치를 통해 물리장치(GPU)에 접근해서 하게된다.
class Device
{

public:

    Device();
    ~Device();

    // +++++++++++++++
    // Physical device
    // +++++++++++++++

    VkPhysicalDevice physicalDevice;
    SwapChainSupportDetails swapchainSupport;
    QueueFamilyIndices queueFamiliyIndices;

    // Set the required device extentions we need
    std::vector<const char*>deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

    void pickPhysicalDevice (VulkanInstance* vInstance, VkSurfaceKHR surface);

    bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface);

    bool checkDeviceExtensionSupported(VkPhysicalDevice device) ;
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);

    QueueFamilyIndices getQueueFamiliesIndicesOfCurrentDevice();

    // ++++++++++++++
    // Logical device
    // ++++++++++++++

    void createLogicalDevice(VkSurfaceKHR surface, bool isValidationLayersEnabled, AppValidationLayersAndExtensions *appValLayersAndExtentions);

    VkDevice logicalDevice;

    // handle to the graphics queue from the queue families of the gpu
    VkQueue graphicsQueue; // we can also have seperate queue for compute, memory transfer, etc.
    VkQueue presentQueue; // queue for displaying the framebuffer


    void destroy();

};

