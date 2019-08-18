#pragma once

#include <vulkan\vulkan.h>
#include <vector>
#include <set>
#include <algorithm>


class SwapChain
{
public:
	SwapChain();
	~SwapChain();


	VkSwapchainKHR swapChain;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainImageExtent;
	
	std::vector<VkImage> swapChainImages;

	//SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);

	VkSurfaceFormatKHR chooseSwapChainSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	//스왑체인이 
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);
	//스왑체인이 지원하는 이미지 범위(크기) 선택
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	
	void create(VkSurfaceKHR surface);

	void destroy();

};