//
// Created by Diego S. Seabra on 07/09/22.
//

#ifndef FEATHER_VULKANDESCRIPTORWRITER_H
#define FEATHER_VULKANDESCRIPTORWRITER_H

#include "VulkanDescriptorSetLayout.h"
#include "VulkanDescriptorPool.h"

class VulkanDescriptorWriter
{
public:
    VulkanDescriptorWriter(VulkanDescriptorSetLayout &setLayout, VulkanDescriptorPool &pool);

    VulkanDescriptorWriter &WriteBuffer(uint32_t binding, VkDescriptorBufferInfo *bufferInfo);
    VulkanDescriptorWriter &WriteImage(uint32_t binding, VkDescriptorImageInfo *imageInfo);
    VulkanDescriptorWriter &WriteImageArray(uint32_t binding, std::vector<VkDescriptorImageInfo> &descriptors);

    bool Build(VkDescriptorSet &set);
    void Overwrite(VkDescriptorSet &set);

private:
    VulkanDescriptorSetLayout &setLayout;
    VulkanDescriptorPool &pool;
    std::vector<VkWriteDescriptorSet> writes;
};


#endif //FEATHER_VULKANDESCRIPTORWRITER_H
