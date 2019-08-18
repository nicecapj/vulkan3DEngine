#include "Tools.h"
#include "Device.h"
#include "VulkanContext.h"

namespace vkTools {


	VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags) {

		VkImageViewCreateInfo viewInfo = {};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.image = image;
		viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;	//2D 텍스쳐
		viewInfo.format = format;

		//// subrange what is the purpose of the image and
		//// what part of the image should be accessed

		viewInfo.subresourceRange.aspectMask = aspectFlags;
		viewInfo.subresourceRange.baseMipLevel = 0;	//기본 밉맵지정(언리얼에도 시작점 지정이 있는데 그 역활)
		viewInfo.subresourceRange.levelCount = 1;

		//// multiple layers is for stereoscopic 3D applications
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = 1;

		VkImageView imageView;
		if (vkCreateImageView(VulkanContext::getInstance()->GetDevice()->logicalDevice, &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
			throw std::runtime_error("failed to create texture image view !");
		}

		return imageView;
	}

}