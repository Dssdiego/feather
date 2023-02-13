//
// Created by Diego S. Seabra on 18/05/22.
//

#ifndef FEATHER_VERTEX_H
#define FEATHER_VERTEX_H

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/glm.hpp>
#include <vulkan/vulkan.h>
#include <array>

struct Vertex {
    glm::vec3 pos;
    glm::vec4 color;
    glm::vec2 uv;
    uint32_t texId;
    glm::vec4 mat0;
    glm::vec4 mat1;
    glm::vec4 mat2;
    glm::vec4 mat3;

    // vertex binding describes at which rate to load data from memory throughout the vertices
    //   it specifies the number of bytes between data entries and whether
    //   to move to the next data entry after each vertex or after each instance
    static VkVertexInputBindingDescription GetBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0; // index of the binding in the array of bindings
        bindingDescription.stride = sizeof(Vertex); // number of bytes from one entry to the next
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX; // move to the next data entry after each vertex

        return bindingDescription;
    }

    // an attribute description struct describes how to extract a vertex attribute from a chunk of vertex data
    //  originating from a binding description
    static std::array<VkVertexInputAttributeDescription, 8> GetAttributeDescriptions()
    {
        std::array<VkVertexInputAttributeDescription, 8> attributeDescriptions{};
        attributeDescriptions[0].binding = 0; // which binding the per-vertex data comes
        attributeDescriptions[0].location = 0; // location directive of the input in the vertex shader
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT; // position is a vec3
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        attributeDescriptions[1].binding = 0; // which binding the per-vertex data comes
        attributeDescriptions[1].location = 1; // location directive of the input in the vertex shader
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32A32_SFLOAT; // color is a vec4
        attributeDescriptions[1].offset = offsetof(Vertex, color);

        attributeDescriptions[2].binding = 0; // which binding the per-vertex data comes
        attributeDescriptions[2].location = 2; // location directive of the input in the vertex shader
        attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT; // uv is a vec2
        attributeDescriptions[2].offset = offsetof(Vertex, uv);

        attributeDescriptions[3].binding = 0; // which binding the per-vertex data comes
        attributeDescriptions[3].location = 3; // location directive of the input in the vertex shader
        attributeDescriptions[3].format = VK_FORMAT_R32_UINT; // texture id is a "uint32_t"
        attributeDescriptions[3].offset = offsetof(Vertex, texId);

        attributeDescriptions[4].binding = 0; // which binding the per-vertex data comes
        attributeDescriptions[4].location = 4; // location directive of the input in the vertex shader
        attributeDescriptions[4].format = VK_FORMAT_R32G32B32A32_SFLOAT;
        attributeDescriptions[4].offset = offsetof(Vertex, mat0);

        attributeDescriptions[5].binding = 0; // which binding the per-vertex data comes
        attributeDescriptions[5].location = 5; // location directive of the input in the vertex shader
        attributeDescriptions[5].format = VK_FORMAT_R32G32B32A32_SFLOAT;
        attributeDescriptions[5].offset = offsetof(Vertex, mat1);

        attributeDescriptions[6].binding = 0; // which binding the per-vertex data comes
        attributeDescriptions[6].location = 6; // location directive of the input in the vertex shader
        attributeDescriptions[6].format = VK_FORMAT_R32G32B32A32_SFLOAT;
        attributeDescriptions[6].offset = offsetof(Vertex, mat2);

        attributeDescriptions[7].binding = 0; // which binding the per-vertex data comes
        attributeDescriptions[7].location = 7; // location directive of the input in the vertex shader
        attributeDescriptions[7].format = VK_FORMAT_R32G32B32A32_SFLOAT;
        attributeDescriptions[7].offset = offsetof(Vertex, mat3);

        return attributeDescriptions;
    }
};


#endif //FEATHER_VERTEX_H
