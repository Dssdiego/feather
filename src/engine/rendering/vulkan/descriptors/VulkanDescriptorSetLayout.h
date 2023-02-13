//
// Created by Diego S. Seabra on 07/09/22.
//

#ifndef FEATHER_VULKANDESCRIPTORSETLAYOUT_H
#define FEATHER_VULKANDESCRIPTORSETLAYOUT_H

#include <vulkan/vulkan.h>
#include <unordered_map>
#include <iostream>
#include <cassert>
#include <memory>

class VulkanDescriptorSetLayout
{
public:
    class Builder
    {
    public:
        Builder &AddBinding(
                uint32_t binding,
                VkDescriptorType descriptorType,
                VkShaderStageFlags stageFlags,
                uint32_t count = 1
        );

        std::unique_ptr<VulkanDescriptorSetLayout> Build() const;

    private:
        std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings{};
    };

    VulkanDescriptorSetLayout(const std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> &bindings);
    ~VulkanDescriptorSetLayout();

//    VulkanDescriptorSetLayout(const VulkanDescriptorSetLayout &) = delete;
//    VulkanDescriptorSetLayout &operator=(const VulkanDescriptorSetLayout &) = delete;

    VkDescriptorSetLayout GetDescriptorSetLayout() const { return descriptorSetLayout; }

private:
    VkDescriptorSetLayout descriptorSetLayout;
    std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings{};

    friend class VulkanDescriptorWriter;
};


#endif //FEATHER_VULKANDESCRIPTORSETLAYOUT_H
