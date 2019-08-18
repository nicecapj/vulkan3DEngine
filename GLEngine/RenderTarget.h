#pragma once

#include <vulkan/vulkan.h>
#include<vector>
#include <array>

//랜더타겟은 말그대로, 랜더링할 대상이다.
//한글로 대충 3D개념을 배우고 프로그램을 하다보면, 랜더타겟은 화면에 그려지는 3D화면외에, 특수효과나
//3D캐릭터를 UI에 캡쳐해넣거나 할때 사용하는 개념으로 알게 된다.
//3D 오브젝트 1개를 그려도 내부적으로는 랜더타겟이 1개는 필요하다.

//랜더타겟 클래스는 이미지뷰(텍스처라고 보면 편하다)를 만들고, 그 해상도로 프레임버퍼를 만든다.
//그림을 사용하기 위해서는 ImageView를 생성해야 한다. 그림은 다른 정보(밉맵이라던지)는 없기 때문에 
//접근할 수 있는 정보를 위해서 필요하다.
//랜더패스에서 프레임마다 지정된 어태치먼트를 명시해야 한다. 프레임버퍼를 만들고, 사진뷰를 어태치먼트처럼 전달한다.
class RenderTarget
{
public:
	RenderTarget();
	~RenderTarget();
	
	std::vector<VkImage> _swapChainImages;
	VkExtent2D _swapChainImageExtent;

	std::vector<VkImageView> swapChainImageViews;
	std::vector<VkFramebuffer> swapChainFramebuffers;

	VkImage depthImage;
	VkDeviceMemory depthImageMemory;
	VkImageView depthImageView;

	VkFormat depthFormat;

	void createViewsAndFramebuffer(std::vector<VkImage> swapChainImages, VkFormat swapChainImageFormat, VkExtent2D swapChainImageExtent, VkRenderPass renderPass);

	void createImageViews(VkFormat swapChainImageFormat);
	void createFrameBuffer(VkExtent2D swapChainImageExtent, VkRenderPass renderPass);


	void destroy();


};



