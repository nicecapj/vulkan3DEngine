#pragma once

#include <vulkan\vulkan.h>
#include <vector>

//vulkan에서 도루잉과 다른 연산들은 gpu에서 command buffer를 통해 직접 처리된다.
//command buffer는 gpu에서 실행되는 녹화되고 실행될 draw command들을 가지고 있다.
//매 프레임마다 커맨드가 녹화되어 커맨드버퍼에 추가됨으로, 속도및단편화를 피하기 위해
// 필수적으로 pool사용을 해야 한다.
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

