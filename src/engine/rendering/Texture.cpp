//
// Created by Diego Santos Seabra on 21/08/22.
//

#include "Texture.h"
#include "vulkan/VulkanBuffer.h"
#include "Samplers.h"

//
// Constructor/Destructor
//

Texture::Texture()
{}

void Texture::Create(const std::string& imagePath, const std::string& samplerName)
{
    this->samplerName = samplerName;

    CreateTextureImage(imagePath);
    VulkanImage::CreateImageView(image, imageView, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_ASPECT_COLOR_BIT);
}

void Texture::Destroy()
{
    vkDestroyImageView(VulkanDevice::GetDevice(), imageView, nullptr);

    vkDestroyImage(VulkanDevice::GetDevice(), image, nullptr);
    vkFreeMemory(VulkanDevice::GetDevice(), imageMemory, nullptr);
}

//
// Implementation
//

void Texture::CreateTextureImage(const std::string &imagePath)
{
    // reading the image with the stbi library
    pixels = stbi_load(imagePath.c_str(), &width, &height, &channels, STBI_rgb_alpha);

    // calculating the size of the image (in vulkan terms)
    VkDeviceSize imageSize = width * height * 4;
    uint32_t imageCount = sizeof(pixels[0]);

    // checking if the image data was filled (e.g. we have pixel data)
    if (!pixels)
    {
        Logger::Error("Failed to load texture image!", imagePath.c_str());
        throw std::runtime_error("failed to load texture image"); // REVIEW: Here we'll probably just want to handle the error instead of stopping the engine!
    }

    // creating a buffer to upload the texure to the GPU
    VulkanBuffer stagingBuffer(
            imageSize,
            imageCount,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
    );
    stagingBuffer.Map();
    stagingBuffer.WriteToBuffer((void *) pixels);

    // texture image is now mapped on the GPU so we don't need it's data representation anymore
    stbi_image_free(pixels);

    // creating and uploading the image to the GPU
    VulkanImage::CreateImage(width, height, VK_FORMAT_R8G8B8A8_UNORM,
                VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT |
                                         VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                image, imageMemory);

    VulkanImage::TransitionImageLayout(image, VK_FORMAT_R8G8B8A8_UNORM,
                          VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

    VulkanImage::CopyBufferToImage(stagingBuffer.GetBuffer(), image,
                      static_cast<uint32_t>(width),
                      static_cast<uint32_t>(height));

    VulkanImage::TransitionImageLayout(image, VK_FORMAT_R8G8B8A8_UNORM,
                          VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                          VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
}

VkDescriptorImageInfo Texture::DescriptorInfo()
{
    VkDescriptorImageInfo imageInfo{};
    imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    imageInfo.imageView = imageView;
    imageInfo.sampler = Samplers::Get(samplerName);
    return imageInfo;
}

glm::vec2 Texture::Size()
{
    return {width, height};
}


