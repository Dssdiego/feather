//
// Created by Diego S. Seabra on 07/09/22.
//

#include "VulkanBuffer.h"
#include "VulkanDevice.h"

//
// Constructor/Destructor
//

VulkanBuffer::VulkanBuffer(VkDeviceSize instanceSize, uint32_t instanceCount, VkBufferUsageFlags usageFlags,
                           VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize minOffsetAlignment)
                           : instanceSize{instanceSize}, instanceCount{instanceCount}, usageFlags{usageFlags}, memoryPropertyFlags{memoryPropertyFlags}
{
    alignmentSize = GetAlignment(instanceSize, minOffsetAlignment);
    bufferSize = alignmentSize * instanceCount;
    VulkanDevice::CreateBuffer(bufferSize, usageFlags, memoryPropertyFlags, buffer, memory);
}

VulkanBuffer::~VulkanBuffer()
{
   Destroy();
}

//
// Internal
//

VkDeviceSize VulkanBuffer::GetAlignment(VkDeviceSize instanceSize, VkDeviceSize minOffsetAligment)
{
    if (minOffsetAligment > 0)
    {
        return (instanceSize + minOffsetAligment - 1) & ~(minOffsetAligment - 1);
    }
    return instanceSize;
}

VkResult VulkanBuffer::Map(VkDeviceSize size, VkDeviceSize offset)
{
    assert(buffer && memory && "Called map on buffer before its creation");
    return vkMapMemory(VulkanDevice::GetDevice(), memory, offset, size, 0 , &mapped);
}

void VulkanBuffer::UnMap()
{
    if (mapped)
    {
        vkUnmapMemory(VulkanDevice::GetDevice(), memory);
        mapped = nullptr;
    }
}

void VulkanBuffer::WriteToBuffer(void *data, VkDeviceSize size, VkDeviceSize offset)
{
    assert(mapped && "Cannot copy to unmapped buffer");

    if (size == VK_WHOLE_SIZE) {
        memcpy(mapped, data, bufferSize);
    } else {
        char *memOffset = (char *)mapped;
        memOffset += offset;
        memcpy(memOffset, data, size);
    }
}

void VulkanBuffer::Destroy()
{
    //Logger::Debug("Destroying Vulkan Buffer");

    UnMap();
    vkDestroyBuffer(VulkanDevice::GetDevice(), buffer, nullptr);
    vkFreeMemory(VulkanDevice::GetDevice(), memory, nullptr);
}

VkResult VulkanBuffer::FlushIndex(int index)
{
    return Flush(alignmentSize, index * alignmentSize);
}

VkResult VulkanBuffer::Flush(VkDeviceSize size, VkDeviceSize offset)
{
    VkMappedMemoryRange mappedRange = {};
    mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
    mappedRange.memory = memory;
    mappedRange.offset = offset;
    mappedRange.size = size;
    return vkFlushMappedMemoryRanges(VulkanDevice::GetDevice(), 1, &mappedRange);
}

void VulkanBuffer::WriteToIndex(void *data, int index)
{
    WriteToBuffer(data, instanceSize, index * alignmentSize);
}

VkDescriptorBufferInfo VulkanBuffer::DescriptorInfo(VkDeviceSize size, VkDeviceSize offset)
{
    return VkDescriptorBufferInfo{buffer, offset, size};
}
