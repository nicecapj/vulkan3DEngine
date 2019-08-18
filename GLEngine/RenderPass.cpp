#include "Renderpass.h"
#include "VulkanContext.h"
#include "Device.h"
#include "Tools.h"

Renderpass::Renderpass(){}


Renderpass::~Renderpass(){}


void Renderpass::createRenderPass(VkFormat swapChainImageFormat) {

	// Tell vulkan the swapchain framebuffer attachments we will be using
	// How many color buffers and depth buffers and 
	// How many samples to use for each of them;

	VkAttachmentDescription colorAttachment = {};
	colorAttachment.format = swapChainImageFormat;
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR; //랜더링 시작 이후 컬러값 버린다.
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE; //랜더된 이후에, 컬러값 저장한다. 포스트이펙트나 다양한 용도로 차후에 사용할 수 있다.

	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;	//랜더링 시작 이후, 스텐실값 버린다.
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;	//랜더된 이후, 메모리에 저장하지 않고, 신경쓰지 않는다.

	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR; // 이미지는 스왑체인으로 보내진다(src)

	// Subpasses - used for post processing
	// previous attachment is sent as reference to subpass to be worked on

	VkAttachmentReference colorAttachRef = {};
	colorAttachRef.attachment = 0; // the first attachment at 0th index
	colorAttachRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL; // images used as color attachment

	// Subpass
	VkSubpassDescription subpass = {};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;//this is a graphics subpass could be compute as well
	subpass.colorAttachmentCount = 1; // this references the - layout(location = 0) out vec4 outColor - of the fragment shader
	subpass.pColorAttachments = &colorAttachRef;


	// Create render pass

	std::array<VkAttachmentDescription, 1> attachments = { colorAttachment };

	VkRenderPassCreateInfo rpCreateInfo = {};
	rpCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	rpCreateInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
	rpCreateInfo.pAttachments = attachments.data();
	rpCreateInfo.subpassCount = 1;
	rpCreateInfo.pSubpasses = &subpass;


	if (vkCreateRenderPass(VulkanContext::getInstance()->GetDevice()->logicalDevice, &rpCreateInfo, nullptr, &renderPass) != VK_SUCCESS) {
		throw std::runtime_error(" failed to create renderpass !!");
	}
}

void Renderpass::beginRenderPass(std::array<VkClearValue, 1> clearValues,
	VkCommandBuffer commandBuffer,
	VkFramebuffer swapChainFrameBuffer,
	VkExtent2D swapChainImageExtent) {

	VkRenderPassBeginInfo rpBeginInfo = {};
	rpBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	rpBeginInfo.renderPass = renderPass;
	rpBeginInfo.framebuffer = swapChainFrameBuffer;//sChain->swapChainFramebuffers[i];
	rpBeginInfo.renderArea.offset = { 0,0 };
	rpBeginInfo.renderArea.extent = swapChainImageExtent;//sChain->swapChainImageExtent;


	rpBeginInfo.pClearValues = clearValues.data();
	rpBeginInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());

	// ------ Begin the render pass
	vkCmdBeginRenderPass(commandBuffer,// commandBuffers[i]
		&rpBeginInfo,
		VK_SUBPASS_CONTENTS_INLINE);
}

void Renderpass::endRenderPass(VkCommandBuffer commandBuffer) {

	vkCmdEndRenderPass(commandBuffer);
}

void Renderpass::destroy(){
	
	vkDestroyRenderPass(VulkanContext::getInstance()->GetDevice()->logicalDevice, renderPass, nullptr);

}
