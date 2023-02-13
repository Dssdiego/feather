//
// Created by Diego S. Seabra on 07/09/22.
//

#include "VulkanDescriptorSetLayout.h"
#include "../VulkanDevice.h"

//
// Constructor/Destructor
//

VulkanDescriptorSetLayout::VulkanDescriptorSetLayout(
        const std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> &bindings) : bindings(bindings)
{
    std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings{};
    for (auto& slb : bindings) {
        setLayoutBindings.push_back(slb.second);
    }

    VkDescriptorSetLayoutCreateInfo descriptorSetLayoutInfo{};
    descriptorSetLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    descriptorSetLayoutInfo.bindingCount = static_cast<uint32_t>(setLayoutBindings.size());
    descriptorSetLayoutInfo.pBindings = setLayoutBindings.data();

    VK_CHECK(vkCreateDescriptorSetLayout(VulkanDevice::GetDevice(), &descriptorSetLayoutInfo, nullptr, &descriptorSetLayout));

    Logger::Debug("Created descriptor set layout");
}

VulkanDescriptorSetLayout::~VulkanDescriptorSetLayout()
{
    vkDestroyDescriptorSetLayout(VulkanDevice::GetDevice(), descriptorSetLayout, nullptr);
}

//
// External
//

VulkanDescriptorSetLayout::Builder&
VulkanDescriptorSetLayout::Builder::AddBinding(uint32_t binding, VkDescriptorType descriptorType, VkShaderStageFlags stageFlags, uint32_t count)
{
    assert(bindings.count(binding) == 0 && "Binding already in use");

    VkDescriptorSetLayoutBinding layoutBinding{};
    layoutBinding.binding = binding;
    layoutBinding.descriptorType = descriptorType;
    layoutBinding.descriptorCount = count;
    layoutBinding.stageFlags = stageFlags;
    bindings[binding] = layoutBinding;
    return *this;
}

std::unique_ptr<VulkanDescriptorSetLayout> VulkanDescriptorSetLayout::Builder::Build() const
{
    return std::make_unique<VulkanDescriptorSetLayout>(bindings);
}


