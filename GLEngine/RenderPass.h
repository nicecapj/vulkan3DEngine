#pragma once

#include <vulkan\vulkan.h>
#include <array>

//each FrameBuffer
//    - Color attachment(����Ʈ�� ǥ�õ� �÷�����)
//    - Depth attachment (����Ʈ�� ǥ�õ��� ������, ���������� ���� �ȼ��� ���� ����)
//    - samples
//�����ӹ��۴� Ÿ�� ����ġ��Ʈ�� �÷����̴�.

// swapChain�����Ŀ� renderPass�� �ѱ��.
// �÷��� ������ �󸶳� ���� ǥ��(present)���� �ʿ����� ����ϰ�, 
// �� �����ӿ��� �󸶳� ���� sample�� ���Ǵ��� ����Ѵ�.

class Renderpass
{
public:
	Renderpass();
	~Renderpass();

	VkRenderPass renderPass;

	void createRenderPass(VkFormat swapChainImageFormat);

	//opengl�� clear�� beginRenderPass���� �Բ� ���ش�.
	void beginRenderPass(std::array<VkClearValue, 1> clearValues, VkCommandBuffer commandBuffer, VkFramebuffer swapChainFrameBuffer, VkExtent2D swapChainImageExtent);
	void endRenderPass(VkCommandBuffer commandBuffer);


	void destroy();
};

