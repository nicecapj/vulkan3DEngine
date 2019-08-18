#include "pch.h"
#include "SwapChain.h"
#include "VulkanContext.h"

SwapChain::SwapChain(){}

SwapChain::~SwapChain(){}

void SwapChain::create(VkSurfaceKHR surface) {

	//디바이스가 지원하는 스왑체인정보 가져온다.
	SwapChainSupportDetails swapChainSupportDetails = VulkanContext::getInstance()->GetDevice()->swapchainSupport;

	VkSurfaceFormatKHR surfaceFormat = chooseSwapChainSurfaceFormat(swapChainSupportDetails.surfaceFormats);
	VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupportDetails.presentModes);
	VkExtent2D extent = chooseSwapExtent(swapChainSupportDetails.surfaceCapabilities);

	//set number of swap chain images
	uint32_t imageCount = swapChainSupportDetails.surfaceCapabilities.minImageCount;

	if (swapChainSupportDetails.surfaceCapabilities.maxImageCount > 0 && imageCount > swapChainSupportDetails.surfaceCapabilities.maxImageCount) {
		imageCount = swapChainSupportDetails.surfaceCapabilities.maxImageCount;
	}

	// creation of swapchain info struct
	VkSwapchainCreateInfoKHR createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = surface;

	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1; // this is 1 unless you are making a stereoscopic 3D application
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	QueueFamilyIndices indices = VulkanContext::getInstance()->GetDevice()->getQueueFamiliesIndicesOfCurrentDevice();

	uint32_t queueFamilyIndices[] = { (uint32_t)indices.graphicsFamily, (uint32_t)indices.presentFamily };

	if (indices.graphicsFamily != indices.presentFamily) {

		// image can be used across multiple queue families without ownership transfer
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;

	}
	else {

		// image is owned by one family at a time and ownership has to be transferred
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0;
		createInfo.pQueueFamilyIndices = nullptr;
	}

	// transform can be applied to the images if required otherwise choose current transform
	createInfo.preTransform = swapChainSupportDetails.surfaceCapabilities.currentTransform;

	// ignore blending with other windows in the window system
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

	createInfo.presentMode = presentMode;

	// do you want pixels to be clipped if there is a window in front
	createInfo.clipped = VK_TRUE;

	createInfo.oldSwapchain = VK_NULL_HANDLE;

	if (vkCreateSwapchainKHR(VulkanContext::getInstance()->GetDevice()->logicalDevice, &createInfo, nullptr, &swapChain) != VK_SUCCESS) {
		throw std::runtime_error("failed to create swap chain !");
	}


	// Retrieve swapchain images
	// Color images are created automatically

	vkGetSwapchainImagesKHR(VulkanContext::getInstance()->GetDevice()->logicalDevice, swapChain, &imageCount, nullptr);
	swapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(VulkanContext::getInstance()->GetDevice()->logicalDevice, swapChain, &imageCount, swapChainImages.data());

	swapChainImageFormat = surfaceFormat.format;
	swapChainImageExtent = extent;

}// createSwapChain


void SwapChain::destroy(){

	// Swapchain
	vkDestroySwapchainKHR(VulkanContext::getInstance()->GetDevice()->logicalDevice, swapChain, nullptr);		
}


// ++++++++++++++++++
// Swap chain helpers
// ++++++++++++++++++

//choosing the right setting for the surfaceformat, presentation mode and swap extent

VkSurfaceFormatKHR SwapChain::chooseSwapChainSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {

	//서페이스포맷이 1개만 지원하는데, 형식 모르겠으면 SRGB(B8G8R8A8)로 리턴한다.
	if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED) {
		return{ VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
	}

	for (const auto& availableFormat : availableFormats) {
		//여러개 포맷 지원중, UINT정규화이고, SRGB지워하면 바로 그것으로 리턴(선형컬러 아님)
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			return availableFormat;
		}
	}
	return availableFormats[0];
}

VkPresentModeKHR SwapChain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes) {

	//티어링 발생 안함. VSYNC활성화하고, FIFO큐, 이전그림 다 그려질때까지 큐에서 대기. 
	//스왑채인 크기-1 의 큐를 가짐. 항상 지원되는 모드
	VkPresentModeKHR bestMode = VK_PRESENT_MODE_FIFO_KHR;	

	for (const auto & availablePresentMode : availablePresentModes) {

		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
			//큐에 넣되, 큐안의 요소는 FIFO_KHR과 달리 1개뿐이다. 그려질때가지 대기함으로 티어링 발생안함.
			return availablePresentMode;
		}
		else if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
			//정 지원 안하면 즉시모드, 티어링 발생할수 있다.
			//랜더링이 모니터 주사율보다 빠르면 괜찮고, 느릴 경우, 찢어짐 발생한다.
			bestMode = availablePresentMode;
		}

		return bestMode;
	}
}


VkExtent2D SwapChain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {

	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
		return capabilities.currentExtent;
	}
	else {

		VkExtent2D actualExtent = { 800, 600 };

		actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
		actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

		return actualExtent;
	}
}