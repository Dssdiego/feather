//
// Created by Diego S. Seabra on 07/09/22.
//

#ifndef FEATHER_VULKANDESCRIPTORPOOL_H
#define FEATHER_VULKANDESCRIPTORPOOL_H

#include <vulkan/vulkan.h>
#include <memory>
#include <vector>

class VulkanDescriptorPool
{
public:
    class Builder
    {
    public:
//    VulkanDescriptorPool(const VulkanDescriptorPool &) = delete;
//    VulkanDescriptorPool &operator=(const VulkanDescriptorPool &) = delete;

        Builder &AddPoolSize(VkDescriptorType descriptorType, uint32_t count);
        Builder &SetPoolFlags(VkDescriptorPoolCreateFlags flags);
        Builder &SetMaxSets(uint32_t count);
        std::unique_ptr<VulkanDescriptorPool> Build() const;


    private:
        std::vector<VkDescriptorPoolSize> poolSizes{};
        uint32_t maxSets = 1000;
        VkDescriptorPoolCreateFlags poolFlags = 0;
    };

    VulkanDescriptorPool(uint32_t maxSets, VkDescriptorPoolCreateFlags poolFlags, const std::vector<VkDescriptorPoolSize> &poolSizes);
    ~VulkanDescriptorPool();

    bool AllocateDescriptor(const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet &descriptor) const;
    void FreeDescriptors(std::vector<VkDescriptorSet> &descriptors) const;

    void ResetPool();

private:
    VkDescriptorPool descriptorPool;

    friend class VulkanDescriptorWriter;
};


#endif //FEATHER_VULKANDESCRIPTORPOOL_H
