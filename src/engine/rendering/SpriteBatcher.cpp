//
// Created by Diego S. Seabra on 28/10/22.
//

#include "SpriteBatcher.h"
#include "vulkan/VulkanBuffer.h"
#include "vulkan/VulkanDevice.h"
#include "vulkan/VulkanEngine.h"
#include "SpriteAtlas.h"
#include <Tracy.hpp>

namespace Feather
{
	//
	// External
	//

	void SpriteBatcher::Create()
	{
		ZoneScopedC();

		// REVIEW: There is a better way to update the vertex/indices buffer?
		if (vertexBuffer == nullptr)
			CreateVertexBuffer();
		else
			UpdateVertexBuffer();

		if (indexBuffer == nullptr)
			CreateIndexBuffer();
		else
			UpdateIndexBuffer();
	}

	void SpriteBatcher::Clear()
	{
		ZoneScopedC();

		drawCalls = 0;

		//if (vertices.size() > 0)
		vertices.resize(0);
		//vertices.clear();

		//if (indices.size() > 0)
		indices.resize(0);
		//indices.clear();

		idxOffset = 0;
		quadCount = 0;
	}

	void SpriteBatcher::Draw()
	{
		ZoneScopedC();

		Bind();
		vkCmdDrawIndexed(VulkanEngine::GetCurrentCommandBuffer(), static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

		//    std::cout << "draw indices: " << indices.size() << std::endl;

		drawCalls++;
	}

	void SpriteBatcher::Shutdown()
	{
		Logger::Debug("Destroying SpriteBatcher");

		// REVIEW: These pointers will be deallocated when the shutdown method finishes its execution?
		vertexBuffer->Destroy();
		indexBuffer->Destroy();
	}


	//
	// Internal
	//

	void SpriteBatcher::CreateVertexBuffer()
	{
		vertexCount = static_cast<uint32_t>(vertices.size());
		assert(vertexCount >= 3 && "Vertex count must be at least 3 so that we can draw triangles ;)");

		VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;
		uint32_t vertexSize = sizeof(vertices[0]);

		// REVIEW: Should we destroy the staging buffer after using it?
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

		Logger::Debug("Created vertex buffer");
	}

	void SpriteBatcher::UpdateVertexBuffer()
	{
		//std::cout << "before update vertex buffer: " << vertexBuffer->Size() << " | " << vertices.size() << std::endl;
		// REVIEW: Is the offset correct?
		vkCmdUpdateBuffer(VulkanEngine::GetCurrentCommandBuffer(), vertexBuffer->GetBuffer(), 0, vertexBuffer->Size(), vertices.data());
		//std::cout << "after update vertex buffer: " << vertexBuffer->Size() << " | " << vertices.size() << std::endl;
		//std::cout << std::endl << std::endl;
	}

	void SpriteBatcher::CreateIndexBuffer()
	{
		// REVIEW: Do we really need a staging buffer?
		indexCount = static_cast<uint32_t>(indices.size());
		assert(vertexCount > 0 && "Index count must be at least 1 so that we can draw triangles with indices ;)");

		VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();
		uint32_t indexSize = sizeof(indices[0]);

		// REVIEW: Should we destroy the staging buffer after using it?
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

		Logger::Debug("Created index buffer");
	}

	void SpriteBatcher::UpdateIndexBuffer()
	{
		// REVIEW: Is the offset correct?
		vkCmdUpdateBuffer(VulkanEngine::GetCurrentCommandBuffer(), indexBuffer->GetBuffer(), 0, indexBuffer->Size(), indices.data());
	}

	void SpriteBatcher::Bind()
	{
		ZoneScopedC();

		// bind the vertex buffer
		VkBuffer buffers[] = { vertexBuffer->GetBuffer() };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(VulkanEngine::GetCurrentCommandBuffer(), 0, 1, buffers, offsets);

		// bind the index buffer
		vkCmdBindIndexBuffer(VulkanEngine::GetCurrentCommandBuffer(), indexBuffer->GetBuffer(), 0, VK_INDEX_TYPE_UINT32);

		//    Logger::Debug("Binded vertex and index buffers");
	}

	// TODO: Implement >> Flush sprites to be rendered immediately
	void SpriteBatcher::Flush()
	{

	}

	std::array<Vertex, 4> SpriteBatcher::MakeQuad(glm::vec3 pos, glm::vec4 color, std::vector<glm::vec2> uvs, uint32_t texId, glm::mat4 mvp)
	{
		ZoneScopedC();

		float size = 1.f;

		Vertex v0 = {
				{ -0.5f, -0.5f, 0.f},
				color,
				uvs[0],
				texId,
				mvp[0],
				mvp[1],
				mvp[2],
				mvp[3]
		};

		Vertex v1 = {
				{ 0.5f, -0.5f, 0.f},
				color,
				uvs[1],
				texId,
				mvp[0],
				mvp[1],
				mvp[2],
				mvp[3]
		};

		Vertex v2 = {
				{ 0.5f, 0.5f, 0.f},
				color,
				uvs[2],
				texId,
				mvp[0],
				mvp[1],
				mvp[2],
				mvp[3]
		};

		Vertex v3 = {
				{ -0.5f, 0.5f, 0.f},
				color,
				uvs[3],
				texId,
				mvp[0],
				mvp[1],
				mvp[2],
				mvp[3]
		};

		return { v0, v1, v2, v3 };
	}

	void SpriteBatcher::AddQuad(std::array<Vertex, 4> quadVertices)
	{
		ZoneScopedC();

		vertices.insert(vertices.end(), {
				quadVertices[0],
				quadVertices[1],
				quadVertices[2],
				quadVertices[3],
			});

		indices.insert(indices.end(), {
				0 + idxOffset,
				1 + idxOffset,
				2 + idxOffset,

				2 + idxOffset,
				3 + idxOffset,
				0 + idxOffset
			});

		idxOffset += 4;
		quadCount++;

		//    std::cout << "==Add Quad==" << std::endl;
		//    std::cout << "vertices: " << vertexCount << std::endl;
		//    std::cout << "indices: " << indexCount << std::endl;
		//    std::cout << "=========" << std::endl;
	}
}

