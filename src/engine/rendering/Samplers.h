//
// Created by Diego Santos Seabra on 23/12/22.
//

#ifndef FEATHER_SAMPLERS_H
#define FEATHER_SAMPLERS_H

#include <iostream>
#include <vulkan/vulkan.hpp>
#include <yaml.h>

#include "../profiling/Logger.h"
#include "vulkan/VulkanDevice.h"
#include "vulkan/VulkanCommon.h"

struct SamplerSet
{
	std::string name;
	VkSampler sampler;
};

class Samplers
{
public:
	static void Init();
	static VkSampler& Get(std::string samplerName);
	static void Shutdown();

private:
	static void ReadSamplersFromYAML();
	static void CreateSampler(const std::string& samplerName, VkFilter filter, VkSamplerAddressMode addressMode, VkSamplerMipmapMode mipmapMode, bool useAnisotropy, bool normalizedCoordinates);

	inline static std::vector<SamplerSet> samplers{};
};

#endif //FEATHER_SAMPLERS_H
