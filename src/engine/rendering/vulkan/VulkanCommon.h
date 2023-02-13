//
// Created by Diego S. Seabra on 15/07/22.
//

#ifndef FEATHER_VULKANCOMMON_H
#define FEATHER_VULKANCOMMON_H

#define VK_CHECK(x)                                                 \
	do                                                              \
	{                                                               \
		VkResult err = x;                                           \
		if (err)                                                    \
		{                                                                 \
			std::cout << "Detected Vulkan error: " << err << std::endl; \
			abort();                                                \
		}                                                           \
	} while (0)

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

// Everything we need to build a pipeline (shader, pipeline object, layout, topology, polygon mode and blending)
struct PipelineSet
{
    std::string vertexShader;
    std::string fragmentShader;
    VkPipeline &pipeline;
    VkPipelineLayout &layout;
    VkPrimitiveTopology topology;
    VkPolygonMode polygonMode;
    VkBool32 enableBlending;
};

#endif //FEATHER_VULKANCOMMON_H
