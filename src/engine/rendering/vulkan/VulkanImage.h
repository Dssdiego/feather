//
// Created by Diego S. Seabra on 25/08/22.
//

#ifndef FEATHER_VULKANIMAGE_H
#define FEATHER_VULKANIMAGE_H

#include <iostream>
#include <vulkan/vulkan.h>

class VulkanImage
{
public:
    static void CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
                              VkMemoryPropertyFlags properties, VkImage &image, VkDeviceMemory &imageMemory);
    static void CreateImageView(VkImage image, VkImageView &imageView, VkFormat format, VkImageAspectFlags aspectFlags);
    static void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
    static void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
};


#endif //FEATHER_VULKANIMAGE_H
