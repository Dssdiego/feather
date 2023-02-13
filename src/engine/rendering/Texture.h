//
// Created by Diego Santos Seabra on 21/08/22.
//

#pragma once

#include "stb/stb_image.h"

#include "vulkan/VulkanDevice.h"
#include "vulkan/VulkanImage.h"

#include <string>
#include <vulkan/vulkan.h>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

class Texture
{
public:
    Texture();

    void Create(const std::string& imagePath, const std::string& samplerName);
    VkDescriptorImageInfo DescriptorInfo();
    glm::vec2 Size();
    void Destroy();

private:
    int width, height, channels;
    stbi_uc* pixels;

    VkImage image;
    VkImageView imageView;
    VkImageLayout imageLayout;

    VkDeviceMemory imageMemory;

    std::string samplerName;

    // Vulkan image creation/definition
    void CreateTextureImage(const std::string &imagePath);
};
