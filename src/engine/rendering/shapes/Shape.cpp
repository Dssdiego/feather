//
// Created by Diego S. Seabra on 20/08/22.
//

#include "Shape.h"

namespace Feather
{
	//
	// Public/External
	//

	Shape::Shape(ShapeType type)
	{
		switch (type)
		{
		case ShapeType::Triangle:
			DefineTriangle();
			break;

		case ShapeType::Quad:
			DefineQuad();
			break;

		case ShapeType::Circle:
			DefineCircle();
			break;
		}

		MakeShape();
	}

	void Shape::MakeShape()
	{
		CreateVertexBuffer();
		CreateIndexBuffer();
	}

	void Shape::Draw()
	{
		Bind();
		vkCmdDrawIndexed(VulkanEngine::GetCurrentCommandBuffer(), static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
	}

	void Shape::Destroy()
	{
		// we don't need to manually destroy the buffers because of the use of the unique ptr
	 //   vertexBuffer.Destroy();
	}

	//
	// Private
	//

	void Shape::CreateVertexBuffer()
	{
		vertexCount = static_cast<uint32_t>(vertices.size());
		assert(vertexCount >= 3 && "Vertex count must be at least 3 so that we can draw triangles ;)");

		VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;
		uint32_t vertexSize = sizeof(vertices[0]);

		VulkanBuffer stagingBuffer(
			vertexSize,
			vertexCount,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		);
		stagingBuffer.Map();
		stagingBuffer.WriteToBuffer((void*)vertices.data());

		vertexBuffer = std::make_shared<VulkanBuffer>(
			vertexSize,
			vertexCount,
			VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
			);

		VulkanDevice::CopyBuffer(stagingBuffer.GetBuffer(), vertexBuffer->GetBuffer(), bufferSize);
	}

	void Shape::CreateIndexBuffer()
	{
		// REVIEW: Do we really need a staging buffer?
		indexCount = static_cast<uint32_t>(indices.size());
		assert(vertexCount > 0 && "Index count must be at least 1 so that we can draw triangles with indices ;)");

		VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();
		uint32_t indexSize = sizeof(indices[0]);

		VulkanBuffer stagingBuffer(
			indexSize,
			indexCount,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		);
		stagingBuffer.Map();
		stagingBuffer.WriteToBuffer((void*)indices.data());

		indexBuffer = std::make_shared<VulkanBuffer>(
			indexSize,
			indexCount,
			VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
			);

		VulkanDevice::CopyBuffer(stagingBuffer.GetBuffer(), indexBuffer->GetBuffer(), bufferSize);
	}

	void Shape::Bind()
	{
		// bind the vertex buffer
		VkBuffer buffers[] = { vertexBuffer->GetBuffer() };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(VulkanEngine::GetCurrentCommandBuffer(), 0, 1, buffers, offsets);

		// bind the index buffer
		vkCmdBindIndexBuffer(VulkanEngine::GetCurrentCommandBuffer(), indexBuffer->GetBuffer(), 0, VK_INDEX_TYPE_UINT32);
	}

	//
	// Shape Definition
	//

	void Shape::DefineTriangle()
	{
		vertices = {
				{{0.0f, -0.5f, 0.f}, {1.0f, 0.0f, 0.0f, 0.f}, {0.f, 0.f}},
				{{0.5f, 0.5f, 0.f}, {0.0f, 1.0f, 0.0f, 0.f}, {0.f, 0.f}},
				{{-0.5f, 0.5f, 0.f}, {0.0f, 0.0f, 1.0f, 0.f}, {0.f, 0.f}}
		};

		indices = {
				0, 1, 2
		};
	}

	void Shape::DefineQuad()
	{
		// position (3) | color (4) | uv (2)
		vertices = {
				{{-0.5f, -0.5f, 0.f}, {1.f, 1.f, 1.f, 0.f}, {0.f, 0.f}},
				{{0.5f, -0.5f, 0.f}, {1.f, 1.f, 1.f, 0.f}, {1.f, 0.f}},
				{{0.5f, 0.5f, 0.f}, {1.f, 1.f, 1.f, 0.f}, {1.f, 1.f}},
				{{-0.5f, 0.5f, 0.f}, {1.f, 1.f, 1.f, 0.f}, {0.f, 1.f}}
		};

		indices = {
				0, 1, 2, 2, 3, 0
		};
	}

	// TODO: Reimplement circle now that we have to consider UV coordinates
	void Shape::DefineCircle()
	{
		//    int numSides = 64;
		//
		//    std::vector<Vertex> uniqueVertices{};
		//
		//    for (int i = 0; i < numSides; i++) {
		//        float angle = i * glm::two_pi<float>() / numSides;
		//        uniqueVertices.push_back({{glm::cos(angle), glm::sin(angle), 0.f}});
		//    }
		//
		//    uniqueVertices.push_back({}); // adding center vertex at (0,0)
		//
		//    for (int i = 0; i < numSides; i++) {
		//        vertices.push_back(uniqueVertices[i]);
		//        vertices.push_back(uniqueVertices[(i + 1) % numSides]);
		//        vertices.push_back(uniqueVertices[numSides]);
		//    }
	}

	void Shape::ReplaceUVs(float startX, float startY, float endX, float endY)
	{
		vertices[0].uv = { startX, startY };
		vertices[1].uv = { endX, startY };
		vertices[2].uv = { endX, endY };
		vertices[3].uv = { startX, endY };

		// FIXME: Do we need to recreate the shape again when replacing the UVs?
		//        Can't we pass the UVs at the moment of the shape creation???
		MakeShape();
	}

	// FIXME: This can be done automatically!
	ShapeType Shape::GetEnumFromString(const std::string& value)
	{
		auto shape = ShapeType::Triangle;

		if (value == "triangle")
			shape = ShapeType::Triangle;

		if (value == "circle")
			shape = ShapeType::Circle;

		if (value == "quad")
			shape = ShapeType::Quad;

		return shape;
	}
}
