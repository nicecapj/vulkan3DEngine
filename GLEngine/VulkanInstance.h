#pragma once
#include <vulkan\vulkan.h>
//#include <stdexcept>

#include"AppValidationLayersAndExtensions.h"


//불칸 인스턴스 생성
class VulkanInstance
{
public:
    VulkanInstance();
    ~VulkanInstance();

    VkInstance vkInstance;

    void createAppAndVkInstance(bool enableValidationLayers,
                                AppValidationLayersAndExtensions *valLayersAndExtentions);


};

