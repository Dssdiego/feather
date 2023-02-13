//
// Created by Diego S. Seabra on 07/09/22.
//

#ifndef FEATHER_VULKANBUFFER_H
#define FEATHER_VULKANBUFFER_H

#include <cassert>
#include <vulkan/vulkan.h>

class VulkanBuffer
{
public:
    VulkanBuffer(VkDeviceSize instanceSize,
                 uint32_t instanceCount,
                 VkBufferUsageFlags usageFlags,
                 VkMemoryPropertyFlags memoryPropertyFlags,
                 VkDeviceSize minOffsetAlignment = 1);
    ~VulkanBuffer();

//    VulkanBuffer(const VulkanBuffer&) = delete;
//    VulkanBuffer &operator=(const VulkanBuffer&) = delete;

    void WriteToBuffer(void* data, VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
    void WriteToIndex(void* data, int index);
    VkResult Flush(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
    VkResult FlushIndex(int index);

    VkResult Map(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
    void UnMap();

    VkDescriptorBufferInfo DescriptorInfo(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);

    VkBufferUsageFlags GetUsageFlags() const { return usageFlags; }
    VkDeviceSize Size() const { return bufferSize; }
    VkBuffer GetBuffer() const { return buffer; }
    void Destroy();

private:
    void* mapped = nullptr;
    VkBuffer buffer = VK_NULL_HANDLE;
    VkDeviceMemory memory = VK_NULL_HANDLE;

    VkDeviceSize bufferSize;
    uint32_t instanceCount;
    VkDeviceSize instanceSize;
    VkDeviceSize alignmentSize;
    VkBufferUsageFlags usageFlags;
    VkMemoryPropertyFlags memoryPropertyFlags;

    static VkDeviceSize GetAlignment(VkDeviceSize instanceSize, VkDeviceSize minOffsetAligment);
};


#endif //FEATHER_VULKANBUFFER_H
