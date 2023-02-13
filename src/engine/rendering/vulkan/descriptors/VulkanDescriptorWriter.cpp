//
// Created by Diego S. Seabra on 07/09/22.
//

#include "VulkanDescriptorWriter.h"
#include "../VulkanDevice.h"

//
// Constructor
//

VulkanDescriptorWriter::VulkanDescriptorWriter(VulkanDescriptorSetLayout &setLayout, VulkanDescriptorPool &pool)
    : setLayout{setLayout}, pool{pool} { }

//
// External
//

VulkanDescriptorWriter &VulkanDescriptorWriter::WriteBuffer(uint32_t binding, VkDescriptorBufferInfo *bufferInfo)
{
    assert(setLayout.bindings.count(binding) == 1 && "Layout does not contain specified binding");

    auto &bindingDescription = setLayout.bindings[binding];

    assert(bindingDescription.descriptorCount == 1 && "Binding single descriptor info, but binding expects multiple");

    VkWriteDescriptorSet write{};
    write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.descriptorType = bindingDescription.descriptorType;
    write.dstBinding = binding;
    write.pBufferInfo = bufferInfo;
    write.descriptorCount = 1;

    writes.push_back(write);
    return *this;
}

VulkanDescriptorWriter &VulkanDescriptorWriter::WriteImage(uint32_t binding, VkDescriptorImageInfo *imageInfo)
{
    assert(setLayout.bindings.count(binding) == 1 && "Layout does not contain specified binding");

    auto &bindingDescription = setLayout.bindings[binding];

    assert(bindingDescription.descriptorCount == 1 &&
            "Binding single descriptor info, but binding expects multiple");

    VkWriteDescriptorSet write{};
    write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.descriptorType = bindingDescription.descriptorType;
    write.dstBinding = binding;
    write.pImageInfo = imageInfo;
    write.descriptorCount = 1;

    writes.push_back(write);
    return *this;
}

VulkanDescriptorWriter& VulkanDescriptorWriter::WriteImageArray(uint32_t binding, std::vector<VkDescriptorImageInfo> &descriptors)
{ 
    assert(setLayout.bindings.count(binding) == 1 && "Layout does not contain specified binding");

    auto &bindingDescription = setLayout.bindings[binding];

    VkWriteDescriptorSet write{};
    write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.descriptorType = bindingDescription.descriptorType;
    write.descriptorCount = descriptors.size();
    write.dstBinding = binding;
    write.dstArrayElement = 0;
    write.pImageInfo = descriptors.data();
    //write.pBufferInfo = 0; // REVIEW: Do we need the buffer info?

    writes.push_back(write);
    return *this;
}

bool VulkanDescriptorWriter::Build(VkDescriptorSet &set)
{
    bool success = pool.AllocateDescriptor(setLayout.GetDescriptorSetLayout(), set);
    if (!success) {
        return false;
    }
    Overwrite(set);
    return true;
}

void VulkanDescriptorWriter::Overwrite(VkDescriptorSet &set)
{
    for (auto &write : writes) {
        write.dstSet = set;
    }
    vkUpdateDescriptorSets(VulkanDevice::GetDevice(), writes.size(), writes.data(), 0, nullptr);
}
