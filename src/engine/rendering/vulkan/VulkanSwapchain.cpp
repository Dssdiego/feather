//
// Created by Diego S. Seabra on 09/07/22.
//

#include "VulkanSwapchain.h"
#include "VulkanEngine.h"

namespace Feather
{
	// TODO: Refactor the code so that we don't use raw pointers. Instead we want to use smart pointers
	//       See more here: https://stackoverflow.com/questions/106508/what-is-a-smart-pointer-and-when-should-i-use-one
	VulkanSwapChainImpl* mVulkanSwapChainImpl = nullptr;

	//
	// Initialization/Destruction
	//

	void VulkanSwapchain::Init()
	{
		mVulkanSwapChainImpl = new VulkanSwapChainImpl;
	}

	void VulkanSwapchain::Shutdown()
	{
		delete mVulkanSwapChainImpl;
	}

	uint32_t VulkanSwapchain::GetImageCount()
	{
		return mVulkanSwapChainImpl->GetImageCount();
	}

	// FIXME: This method could be misleading!
	//  It isn't the current number of frames in flight but the MAX number!
	uint32_t VulkanSwapchain::GetNumberOfFramesInFlight()
	{
		return mVulkanSwapChainImpl->MAX_FRAMES_IN_FLIGHT;
	}

	VkImageView VulkanSwapchain::GetCurrentImageView()
	{
		return mVulkanSwapChainImpl->GetCurrentImageView();
	}

	void VulkanSwapchain::Recreate()
	{
		// handling window minimization
		int width, height = 0;
		glfwGetFramebufferSize(Window::GetWindow(), &width, &height);
		while (width == 0 || height == 0) // window is minimized, wait for it to be on the foreground again
		{
			glfwGetFramebufferSize(Window::GetWindow(), &width, &height);
			glfwWaitEvents();
		}

		// don't touch resources that may still be in use
		vkDeviceWaitIdle(VulkanDevice::GetDevice());

		// when trying to recreate the swapchain, check if it already exists. if exists, we shut it down and we create it again
		if (mVulkanSwapChainImpl->swapChain != nullptr)
		{
			Logger::Debug("Recreating swap chain...");

			Shutdown();
			Init();
		}
		else {
			// if it doesn't exists, we create it from scratch
			Init();
		}
	}

	VkResult VulkanSwapchain::AcquireNextImage(uint32_t* imageIndex)
	{
		return mVulkanSwapChainImpl->AcquireNextImage(imageIndex);
	}

	VkResult VulkanSwapchain::SubmitCommandBuffers(uint32_t* imageIndex)
	{
		return mVulkanSwapChainImpl->SubmitCommandBuffers(imageIndex);
	}

	VkRenderPass VulkanSwapchain::GetRenderPass()
	{
		return mVulkanSwapChainImpl->GetRenderPass();
	}

	VkFramebuffer VulkanSwapchain::GetFrameBuffer(int index)
	{
		return mVulkanSwapChainImpl->GetFrameBuffer(index);
	}

	VkExtent2D VulkanSwapchain::GetSwapChainExtent()
	{
		return mVulkanSwapChainImpl->GetSwapChainExtent();
	}

	VkImageView VulkanSwapchain::CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags)
	{
		return mVulkanSwapChainImpl->CreateImageView(image, format, aspectFlags);
	}

	float VulkanSwapchain::GetAspectRatio()
	{
		return mVulkanSwapChainImpl->GetAspectRatio();
	}

	//
	// Implementation
	//

	VulkanSwapChainImpl::VulkanSwapChainImpl()
	{
		CreateSwapChain();
		CreateImageViews();
		CreateRenderPass();
		CreateDepthResources();
		CreateFrameBuffers();
		CreateSyncObjects();

		// initilizing camera stuff after we have our swapchain ready
		Camera::Init(GetAspectRatio());
	}

	VulkanSwapChainImpl::~VulkanSwapChainImpl()
	{
		Logger::Debug("Destroying swapchain image views");
		for (auto imageView : swapChainImageViews)
		{
			vkDestroyImageView(VulkanDevice::GetDevice(), imageView, nullptr);
		}
		swapChainImageViews.clear();

		Logger::Debug("Destroying swapchain");
		if (swapChain != nullptr)
		{
			vkDestroySwapchainKHR(VulkanDevice::GetDevice(), swapChain, nullptr);
			swapChain = nullptr;
		}

		Logger::Debug("Destroying depth image");
		vkDestroyImageView(VulkanDevice::GetDevice(), depthImageView, nullptr);
		vkDestroyImage(VulkanDevice::GetDevice(), depthImage, nullptr);
		vkFreeMemory(VulkanDevice::GetDevice(), depthImageMemory, nullptr);

		Logger::Debug("Destroying framebuffers");
		for (auto frameBuffer : swapChainFrameBuffers)
		{
			vkDestroyFramebuffer(VulkanDevice::GetDevice(), frameBuffer, nullptr);
		}

		Logger::Debug("Destroying renderpass");
		vkDestroyRenderPass(VulkanDevice::GetDevice(), renderPass, nullptr);

		// TODO: Cleanup synchonization objects (semaphores and fences)
		Logger::Debug("Destroying sync objects");
		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			vkDestroySemaphore(VulkanDevice::GetDevice(), renderFinishedSemaphores[i], nullptr);
			vkDestroySemaphore(VulkanDevice::GetDevice(), imageAvailableSemaphores[i], nullptr);
			vkDestroyFence(VulkanDevice::GetDevice(), inFlightFences[i], nullptr);
		}
	}

	void VulkanSwapChainImpl::CreateSwapChain()
	{
		SwapChainSupportDetails swapChainSupport = VulkanDevice::GetSwapChainSupport();

		VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
		VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);
		VkExtent2D extent = ChooseSwapExtent(swapChainSupport.capabilities);

		uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1; // one more image to have "room" for more processing
		if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
		{
			// making sure we don't exceed the maximum number of images in the swap chain
			imageCount = swapChainSupport.capabilities.maxImageCount;
		}
		std::cout << "Minimum image count in the swap chain: " << imageCount << std::endl;

		VkSwapchainCreateInfoKHR createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = VulkanDevice::GetSurface(); // tying our surface to the swap chain
		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = extent;
		createInfo.imageArrayLayers = 1; // the amount of layers each image consists of (always 1 unless we are developing a stereoscopic 3D app)

		// VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT - This is a color image we're rendering into
		// VK_IMAGE_USAGE_TRANSFER_SRC_BIT - We'll be copying the image somewhere (screenshot, postProcess)
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;

		uint32_t queueFamilyIndices[] = { VulkanDevice::GetGraphicsQueueFamilyIdx(), VulkanDevice::GetPresentQueueFamilyIdx() };

		// specifying how to handle swap chain images that will be used across multiple queue families (graphics and presentation)
		// in our case, we'll be drawing on the images in the swap chain from the graphics queue and then
		//    submitting them on the presentation queue
		// REVIEW: Is this correct?
		if (VulkanDevice::GetGraphicsQueueFamilyIdx() != VulkanDevice::GetPresentQueueFamilyIdx())
		{
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = queueFamilyIndices;
		}
		else {
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			createInfo.queueFamilyIndexCount = 0; // optional
			createInfo.pQueueFamilyIndices = nullptr; // optional
		}

		// we don't want any pre-transformation on the images for now
		createInfo.preTransform = swapChainSupport.capabilities.currentTransform;

		// compositeAlpha = blending with other windows in the window system. So we will ignore the alpha channel (aka opaque)
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE; // we don't care for pixels that are obscured (ex.: another window in front of our own)
		createInfo.oldSwapchain = VK_NULL_HANDLE; // for now we assume that we'll be creating only one swap chain.
		// NOTE: Only one swap chain is not the best option because we'd like the screen to be resized, so in the future we'll probably recreate the swap chain ;)

		// effectively creating the swap chain
		VK_CHECK(vkCreateSwapchainKHR(VulkanDevice::GetDevice(), &createInfo, nullptr, &swapChain));

		// retrieve swap chain images from the logical device
		vkGetSwapchainImagesKHR(VulkanDevice::GetDevice(), swapChain, &imageCount, nullptr);
		swapChainImages.resize(imageCount);
		vkGetSwapchainImagesKHR(VulkanDevice::GetDevice(), swapChain, &imageCount, swapChainImages.data());

		swapChainImageFormat = surfaceFormat.format;
		swapChainExtent = extent;

		Logger::Debug("Swap chain created");
	}

	void VulkanSwapChainImpl::CreateImageViews()
	{
		// resizing the list to fit all of the image views
		swapChainImageViews.resize(swapChainImages.size());

		for (size_t i = 0; i < swapChainImages.size(); i++)
		{
			swapChainImageViews[i] = CreateImageView(swapChainImages[i], swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
		}

		std::cout << "# of image views created: " << swapChainImageViews.size() << std::endl;
	}

	void VulkanSwapChainImpl::CreateRenderPass()
	{
		// for now, just a single color buffer attachment represented by one of the images from the swap chain
		VkAttachmentDescription colorAttachment{};
		colorAttachment.format = swapChainImageFormat; // matching the format of the swap chain images
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT; // matching the format of the swap chain images
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR; // clear the framebuffer before drawing a new frame
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE; // rendered contents will be stored in memory
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE; // doing nothing with stencil buffers for now, so we don't care what is in the buffer
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE; // doing nothing with stencil buffers for now, so we don't care what is in the buffer
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED; // layout of the image before the render pass begins
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR; // layout to automatically transition the image to when the render pass finishes

		// we'll stick to a single subpass for now (so index 0)
		VkAttachmentReference colorAttachmentRef{};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		// depth
		VkAttachmentDescription depthAttachment{};
		depthAttachment.format = FindDepthFormat();
		depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		VkAttachmentReference depthAttachmentRef{};
		depthAttachmentRef.attachment = 1;
		depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		// subpass
		VkSubpassDescription subpass{};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentRef;
		subpass.pDepthStencilAttachment = &depthAttachmentRef;

		VkSubpassDependency dependency{};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT; // wait for the swap chain to finish reading before we can access it
		dependency.srcAccessMask = 0;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		std::array<VkAttachmentDescription, 2> attachments = { colorAttachment, depthAttachment };

		VkRenderPassCreateInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		renderPassInfo.pAttachments = attachments.data();
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;
		renderPassInfo.dependencyCount = 1;
		renderPassInfo.pDependencies = &dependency;

		// effectively creating the render pass
		VK_CHECK(vkCreateRenderPass(VulkanDevice::GetDevice(), &renderPassInfo, nullptr, &renderPass));

		Logger::Debug("Render pass created");
	}

	void VulkanSwapChainImpl::CreateDepthResources()
	{
		// REVIEW: Use multiple depth buffers/images? Ex.: One depth buffer per framebuffer?
		//         How it would change on our render pass?
		VkFormat depthFormat = FindDepthFormat();

		VulkanImage::CreateImage(swapChainExtent.width, swapChainExtent.height,
			depthFormat, VK_IMAGE_TILING_OPTIMAL,
			VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImage,
			depthImageMemory);

		depthImageView = CreateImageView(depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
	}

	void VulkanSwapChainImpl::CreateFrameBuffers()
	{
		// resizing the container to hold all the framebuffers
		swapChainFrameBuffers.resize(swapChainImageViews.size());

		// iterate through the image views and create framebuffers from them
		for (size_t i = 0; i < swapChainImageViews.size(); i++)
		{
			std::array<VkImageView, 2> attachments = {
					swapChainImageViews[i],
					depthImageView // REVIEW: Pass multiple depth buffers here if we are using them
			};

			VkFramebufferCreateInfo frameBufferInfo{};
			frameBufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			frameBufferInfo.renderPass = renderPass;
			frameBufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
			frameBufferInfo.pAttachments = attachments.data();
			frameBufferInfo.width = swapChainExtent.width;
			frameBufferInfo.height = swapChainExtent.height;
			frameBufferInfo.layers = 1; // out swap chain contains single images, so only 1 layer

			VK_CHECK(vkCreateFramebuffer(VulkanDevice::GetDevice(), &frameBufferInfo, nullptr, &swapChainFrameBuffers[i]));
		}

		Logger::Debug("Framebuffers created");
	}

	void VulkanSwapChainImpl::CreateSyncObjects()
	{
		imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);

		inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
		imagesInFlight.resize(GetImageCount(), VK_NULL_HANDLE);

		VkSemaphoreCreateInfo semaphoreInfo{};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceInfo = {};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT; // REVIEW: Why use the signaled bit?

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			VK_CHECK(vkCreateSemaphore(VulkanDevice::GetDevice(), &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]));
			VK_CHECK(vkCreateSemaphore(VulkanDevice::GetDevice(), &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]));
			VK_CHECK(vkCreateFence(VulkanDevice::GetDevice(), &fenceInfo, nullptr, &inFlightFences[i]));
		}

		Logger::Debug("Created sync objects (semaphores and fences)");
	}

	VkResult VulkanSwapChainImpl::AcquireNextImage(uint32_t* imageIndex)
	{
		vkWaitForFences(
			VulkanDevice::GetDevice(),
			1,
			&inFlightFences[currentFrame],
			VK_TRUE,
			std::numeric_limits<uint64_t>::max());

		VkResult result = vkAcquireNextImageKHR(
			VulkanDevice::GetDevice(),
			swapChain,
			std::numeric_limits<uint64_t>::max(),
			imageAvailableSemaphores[currentFrame],  // must be a not signaled semaphore
			VK_NULL_HANDLE,
			imageIndex);

		return result;
	}

	VkResult VulkanSwapChainImpl::SubmitCommandBuffers(uint32_t* imageIndex)
	{
		auto submitCommandBuffers = VulkanEngine::GetSubmitCommandBuffers();

		if (imagesInFlight[*imageIndex] != VK_NULL_HANDLE)
		{
			vkWaitForFences(VulkanDevice::GetDevice(), 1, &imagesInFlight[*imageIndex], VK_TRUE, UINT64_MAX);
		}
		imagesInFlight[*imageIndex] = inFlightFences[currentFrame];

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame] };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = static_cast<uint32_t>(submitCommandBuffers.size());
		submitInfo.pCommandBuffers = submitCommandBuffers.data(); // which command buffers to actually submit for execution

		VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame] };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores; // which semaphore to signal once the command buffer(s) have finished execution

		vkResetFences(VulkanDevice::GetDevice(), 1, &inFlightFences[currentFrame]);
		if (vkQueueSubmit(VulkanDevice::GetGraphicsQueue(), 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS)
		{
			std::string err = "Failed to submit draw command buffer";
			Logger::Error(err, "");
			throw std::runtime_error(err);
		}

		// submitting the result back to the swap chain to have it eventually show up on the screen
		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores; // which semaphores to wait on before presentation can happen

		VkSwapchainKHR swapChains[] = { swapChain };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = imageIndex;

		// submitting the request to present an image to the swap chain
		auto result = vkQueuePresentKHR(VulkanDevice::GetPresentQueue(), &presentInfo);

		currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;

		return result;
	}

	//
	// Helpers
	//

	VkImageView VulkanSwapChainImpl::CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags)
	{
		VkImageViewCreateInfo viewInfo{};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.image = image;
		viewInfo.format = format;
		viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		viewInfo.subresourceRange.aspectMask = aspectFlags;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = 1;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = 1;

		VkImageView imageView;
		VK_CHECK(vkCreateImageView(VulkanDevice::GetDevice(), &viewInfo, nullptr, &imageView));

		return imageView;
	}

	VkSurfaceFormatKHR VulkanSwapChainImpl::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
	{
		VkSurfaceFormatKHR result;

		// if vulkan returned an unknown format, then we just force what we want.
		if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED)
		{
			result.format = VK_FORMAT_B8G8R8A8_UNORM;
			result.colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
			return result;
		}

		// favor 32 bit rgba and srgb nonlinear colorspace
		for (const auto& availableFormat : availableFormats)
		{
			if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM &&
				availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
				return availableFormat;
			}
		}

		// if all else fails, just return what's available
		return availableFormats[0];
	}

	VkPresentModeKHR VulkanSwapChainImpl::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
	{
		// uncapped frame rate, use MAILBOX presentation mode (if available)
		if (!Window::IsVSyncEnabled())
		{
			// checking if triple buffering is available
			for (const auto& availablePresentMode : availablePresentModes)
			{
				if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
				{
					Logger::Debug("Present Mode: MAILBOX");
					return availablePresentMode;
				}
			}
		}
		//else // VSync enabled
		//    return VK_PRESENT_MODE_FIFO_KHR;

		// if triple buffering is not available, use Vulkan's default swap chain queue (aka VSync)
		Logger::Debug("Present Mode: V-SYNC");
		return VK_PRESENT_MODE_FIFO_KHR;
	}

	VkExtent2D VulkanSwapChainImpl::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
	{
		if (capabilities.currentExtent.width != UINT32_MAX)
		{
			return capabilities.currentExtent;
		}
		else {
			int width, height;
			glfwGetFramebufferSize(Window::GetWindow(), &width, &height);

			VkExtent2D actualExtent = {
					static_cast<uint32_t>(width),
					static_cast<uint32_t>(height)
			};

			actualExtent.width =
				std::max(capabilities.minImageExtent.width,
					std::min(capabilities.maxImageExtent.width,
						actualExtent.width));

			actualExtent.height =
				std::max(capabilities.minImageExtent.height,
					std::min(capabilities.maxImageExtent.height,
						actualExtent.height));

			return actualExtent;
		}
	}

	VkFormat VulkanSwapChainImpl::FindDepthFormat()
	{
		return FindSupportedFormat(
			{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
			VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
		);
	}

	VkFormat VulkanSwapChainImpl::FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling,
		VkFormatFeatureFlags features)
	{
		for (VkFormat format : candidates)
		{
			VkFormatProperties props;
			vkGetPhysicalDeviceFormatProperties(VulkanDevice::GetPhysicalDevice(), format, &props);

			if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features)
			{
				return format;
			}
			else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features)
			{
				return format;
			}
		}

		Logger::Error("failed to find supported format", "");
		throw std::runtime_error("failed to find supported format");
	}

	uint32_t VulkanSwapChainImpl::GetImageCount()
	{
		return swapChainImages.size();
	}

	VkRenderPass VulkanSwapChainImpl::GetRenderPass()
	{
		return renderPass;
	}

	VkFramebuffer VulkanSwapChainImpl::GetFrameBuffer(int index)
	{
		return swapChainFrameBuffers[index];
	}

	VkExtent2D VulkanSwapChainImpl::GetSwapChainExtent()
	{
		return swapChainExtent;
	}

	VkImageView VulkanSwapChainImpl::GetCurrentImageView()
	{
		return swapChainImageViews[currentFrame];
	}

	float VulkanSwapChainImpl::GetAspectRatio()
	{
		return (float)swapChainExtent.width / (float)swapChainExtent.height;
	}
}
