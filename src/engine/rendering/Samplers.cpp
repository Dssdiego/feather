//
// Created by Diego Santos Seabra on 23/12/22.
//

#include "Samplers.h"

void Samplers::Init()
{
    ReadSamplersFromYAML();
}

VkSampler& Samplers::Get(std::string samplerName)
{
    for (auto& s : samplers)
    {
        if (s.name == samplerName)
            return s.sampler;
    }

    return samplers[0].sampler;
}

void Samplers::Shutdown()
{
    Logger::Debug("Destroying samplers");

    for (auto& s : samplers)
    {
        vkDestroySampler(VulkanDevice::GetDevice(), s.sampler, nullptr);
    }
}

void Samplers::ReadSamplersFromYAML()
{
    YAML::Node samplerYAML = YAML::LoadFile("configs/samplers.yaml");
    auto samps = samplerYAML["samplers"];

    for (const auto& s : samps)
    { 
		CreateSampler(
            s["name"].as<std::string>(),
            static_cast<VkFilter>(s["filter"].as<int>()),
            static_cast<VkSamplerAddressMode>(s["address_mode"].as<int>()),
            static_cast<VkSamplerMipmapMode>(s["mipmap_mode"].as<int>()),
            s["anisotropy"].as<bool>(),
            s["normalizedCoordinates"].as<bool>()
		);
    }
}

void Samplers::CreateSampler(const std::string &samplerName, VkFilter filter, VkSamplerAddressMode addressMode, VkSamplerMipmapMode mipmapMode, bool useAnisotropy, bool normalizedCoordinates)
{
    Logger::Debug("Creating sampler " + samplerName);

    SamplerSet samplerSet;
    samplerSet.name = samplerName;

    VkSamplerCreateInfo samplerInfo{};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = filter;
    samplerInfo.minFilter = filter;
    samplerInfo.addressModeU = addressMode;
    samplerInfo.addressModeV = addressMode;
    samplerInfo.addressModeW = addressMode;

    if (useAnisotropy)
    {
        samplerInfo.anisotropyEnable = VK_TRUE;
        samplerInfo.maxAnisotropy = 16;
    }
    else
    {
        samplerInfo.anisotropyEnable = VK_FALSE;
        samplerInfo.maxAnisotropy = 0;
    }

    samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    samplerInfo.unnormalizedCoordinates = !normalizedCoordinates;
    samplerInfo.compareEnable = VK_FALSE;
    samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
    samplerInfo.mipmapMode = mipmapMode;
    samplerInfo.mipLodBias = 0.0f;
    samplerInfo.minLod = 0.0f;
    samplerInfo.maxLod = 0.0f;

    VK_CHECK(vkCreateSampler(VulkanDevice::GetDevice(), &samplerInfo, nullptr, &samplerSet.sampler));

    samplers.push_back(samplerSet);
}
