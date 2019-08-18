#include "VulkanContext.h"
#include "Device.h"
#include "VulkanInstance.h"
#include "SwapChain.h"
#include "DrawCommandBuffer.h"
#include "Renderpass.h"
#include "RenderTarget.h"

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
    device = new Device();
    device->pickPhysicalDevice(vInstance, surface);
    device->createLogicalDevice(surface, isValidationLayersEnabled, valLayersAndExt);

	//실제 이미지를 관리하는 스왑체인
	swapChain_ = new SwapChain();
	swapChain_->create(surface);

	//랜더패스 생성
	renderPass_ = new Renderpass();
	renderPass_->createRenderPass(swapChain_->swapChainImageFormat);

	//랜더타겟(이미지뷰)과 프레임버퍼 생성
	renderTarget_ = new RenderTarget();
	renderTarget_->createViewsAndFramebuffer(swapChain_->swapChainImages,
		swapChain_->swapChainImageFormat,
		swapChain_->swapChainImageExtent,
		renderPass_->renderPass);

	//드로우 커맨드 버퍼 생성
	drawCommandBuffer_ = new DrawCommandBuffer();
	drawCommandBuffer_->createCommandPoolAndBuffer(swapChain_->swapChainImages.size());

}

void VulkanContext::DrawBegin()
{
	//스왑체인에서 다음 이미지의 인덱스를 점유한다.
	vkAcquireNextImageKHR(device->logicalDevice,
		swapChain_->swapChain,
		std::numeric_limits<uint64_t>::max(),
		NULL, // is  signaled
		VK_NULL_HANDLE,
		&imageIndex_);

	currentCommandBuffer_ = drawCommandBuffer_->commandBuffers[imageIndex_];

	//커맨드 녹화시작
	drawCommandBuffer_->beginCommandBuffer(currentCommandBuffer_);

	VkClearValue clearcolor = { 1.0f, 0.0f, 0.0f, 1.0f };
	std::array<VkClearValue, 1> clearValue = { clearcolor };
	renderPass_->beginRenderPass(clearValue,
		currentCommandBuffer_,
		renderTarget_->swapChainFramebuffers[imageIndex_],
		renderTarget_->_swapChainImageExtent);

}

void VulkanContext::DrawEnd()
{
	//랜더패스 끝
	renderPass_->endRenderPass(currentCommandBuffer_);
	
	//커맨드 녹화 끝
	drawCommandBuffer_->endCommandBuffer(currentCommandBuffer_);

	//녹화한 커맨드를 커맨드 버퍼(그래픽스 큐)에 적용한다.
	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &currentCommandBuffer_;
	vkQueueSubmit(device->graphicsQueue, 1, &submitInfo, NULL);
	
	//프레임을 출력한다(Present Frame)		
	//프레임에 커맨드큐의 커맨드가 랜더되면, 표시(present) 큐를 통해, 뷰포트에 표시(제시)된다.
	//제시되는 동안, 이미지는 스왑체인의 백으로 이동한다.
	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = &swapChain_->swapChain;
	presentInfo.pImageIndices = &imageIndex_;	
	vkQueuePresentKHR(device->presentQueue, &presentInfo);
	
	//디바이스가 present를 완료할 동안 대기한다.	
	vkQueueWaitIdle(device->presentQueue);
}

Device* VulkanContext::GetDevice()
{
	return device;
}

void VulkanContext::CleanUp()
{
	//디바이스 사용이 끝날떄까지 대기
	vkDeviceWaitIdle(device->logicalDevice);
	
	drawCommandBuffer_->destroy();
	renderTarget_->destroy();
	renderPass_->destroy();	
	swapChain_->destroy();	
	device->destroy();	
	valLayersAndExt->destroy(vInstance->vkInstance, isValidationLayersEnabled);

	vkDestroySurfaceKHR(vInstance->vkInstance, surface, nullptr);
}

