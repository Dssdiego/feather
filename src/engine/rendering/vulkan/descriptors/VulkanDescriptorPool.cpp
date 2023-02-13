//
// Created by Diego S. Seabra on 07/09/22.
//

#include "VulkanDescriptorPool.h"
#include "../VulkanDevice.h"

//
// Constructor/Destructor
//

VulkanDescriptorPool::VulkanDescriptorPool(uint32_t maxSets, VkDescriptorPoolCreateFlags poolFlags,
                                           const std::vector<VkDescriptorPoolSize> &poolSizes)
{
    VkDescriptorPoolCreateInfo descriptorPoolInfo{};
    descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    descriptorPoolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    descriptorPoolInfo.pPoolSizes = poolSizes.data();
    descriptorPoolInfo.maxSets = maxSets;
    descriptorPoolInfo.flags = poolFlags;

    VK_CHECK(vkCreateDescriptorPool(VulkanDevice::GetDevice(), &descriptorPoolInfo, nullptr, &descriptorPool));

    Logger::Debug("Created descriptor pool");
}

VulkanDescriptorPool::~VulkanDescriptorPool()
{
    vkDestroyDescriptorPool(VulkanDevice::GetDevice(), descriptorPool, nullptr);
}

//
// External
//

VulkanDescriptorPool::Builder &VulkanDescriptorPool::Builder::AddPoolSize(VkDescriptorType descriptorType, uint32_t count)
{
    poolSizes.push_back({descriptorType, count});
    return *this;
}

VulkanDescriptorPool::Builder &VulkanDescriptorPool::Builder::SetPoolFlags(VkDescriptorPoolCreateFlags flags)
{
    poolFlags = flags;
    return *this;
}

VulkanDescriptorPool::Builder &VulkanDescriptorPool::Builder::SetMaxSets(uint32_t count)
{
    maxSets = count;
    return *this;
}

std::unique_ptr<VulkanDescriptorPool> VulkanDescriptorPool::Builder::Build() const
{
    return std::make_unique<VulkanDescriptorPool>(maxSets, poolFlags, poolSizes);
}

bool VulkanDescriptorPool::AllocateDescriptor(const VkDescriptorSetLayout descriptorSetLayout,
                                              VkDescriptorSet &descriptor) const
{
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool;
    allocInfo.pSetLayouts = &descriptorSetLayout;
    allocInfo.descriptorSetCount = 1;

    // REVIEW: We might want to create a "DescriptorPoolManager" class that handles this case, and builds
    //         a new pool whenever an old pool fills up. But this is beyond our current scope
    if (vkAllocateDescriptorSets(VulkanDevice::GetDevice(), &allocInfo, &descriptor) != VK_SUCCESS) {
        return false;
    }
    return true;
}

void VulkanDescriptorPool::FreeDescriptors(std::vector<VkDescriptorSet> &descriptors) const
{
    vkFreeDescriptorSets(VulkanDevice::GetDevice(), descriptorPool, static_cast<uint32_t>(descriptors.size()), descriptors.data());
}

void VulkanDescriptorPool::ResetPool()
{
    vkResetDescriptorPool(VulkanDevice::GetDevice(), descriptorPool, 0);
}
