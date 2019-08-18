#pragma once

#include <vulkan\vulkan.h>
#include <vector>

//vulkan���� �����װ� �ٸ� ������� gpu���� command buffer�� ���� ���� ó���ȴ�.
//command buffer�� gpu���� ����Ǵ� ��ȭ�ǰ� ����� draw command���� ������ �ִ�.
//�� �����Ӹ��� Ŀ�ǵ尡 ��ȭ�Ǿ� Ŀ�ǵ���ۿ� �߰�������, �ӵ��״���ȭ�� ���ϱ� ����
// �ʼ������� pool����� �ؾ� �Ѵ�.
class DrawCommandBuffer
{
public:
	DrawCommandBuffer();
	~DrawCommandBuffer();

	VkCommandPool commandPool;
	std::vector<VkCommandBuffer> commandBuffers;

	void createCommandPoolAndBuffer(size_t imageCount);
	void beginCommandBuffer(VkCommandBuffer commandBuffer);
	void endCommandBuffer(VkCommandBuffer commandBuffer);

	void createCommandPool();
	void allocateCommandBuffers(size_t imageCount);

	void destroy();
};

