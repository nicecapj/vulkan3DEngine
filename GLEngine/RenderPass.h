#pragma once

#include <vulkan\vulkan.h>
#include <array>

//each FrameBuffer
//    - Color attachment(뷰포트로 표시될 컬러정보)
//    - Depth attachment (뷰포트로 표시되진 않지만, 내부적으로 사용될 픽셀당 깊이 정보)
//    - samples
//프레임버퍼는 타겟 어태치먼트의 컬렉션이다.

// swapChain생성후에 renderPass로 넘긴다.
// 컬러와 뎁스가 얼마나 많이 표시(present)질지 필요한지 명시하고, 
// 각 프레임에서 얼마나 많은 sample이 사용되는지 명시한다.

class Renderpass
{
public:
	Renderpass();
	~Renderpass();

	VkRenderPass renderPass;

	void createRenderPass(VkFormat swapChainImageFormat);

	//opengl의 clear는 beginRenderPass에서 함께 해준다.
	void beginRenderPass(std::array<VkClearValue, 1> clearValues, VkCommandBuffer commandBuffer, VkFramebuffer swapChainFrameBuffer, VkExtent2D swapChainImageExtent);
	void endRenderPass(VkCommandBuffer commandBuffer);


	void destroy();
};

