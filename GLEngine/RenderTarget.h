#pragma once

#include <vulkan/vulkan.h>
#include<vector>
#include <array>

//����Ÿ���� ���״��, �������� ����̴�.
//�ѱ۷� ���� 3D������ ���� ���α׷��� �ϴٺ���, ����Ÿ���� ȭ�鿡 �׷����� 3Dȭ��ܿ�, Ư��ȿ����
//3Dĳ���͸� UI�� ĸ���سְų� �Ҷ� ����ϴ� �������� �˰� �ȴ�.
//3D ������Ʈ 1���� �׷��� ���������δ� ����Ÿ���� 1���� �ʿ��ϴ�.

//����Ÿ�� Ŭ������ �̹�����(�ؽ�ó��� ���� ���ϴ�)�� �����, �� �ػ󵵷� �����ӹ��۸� �����.
//�׸��� ����ϱ� ���ؼ��� ImageView�� �����ؾ� �Ѵ�. �׸��� �ٸ� ����(�Ӹ��̶����)�� ���� ������ 
//������ �� �ִ� ������ ���ؼ� �ʿ��ϴ�.
//�����н����� �����Ӹ��� ������ ����ġ��Ʈ�� ����ؾ� �Ѵ�. �����ӹ��۸� �����, �����並 ����ġ��Ʈó�� �����Ѵ�.
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



