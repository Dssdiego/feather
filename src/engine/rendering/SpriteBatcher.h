//
// Created by Diego S. Seabra on 28/10/22.
//

#ifndef FEATHER_SPRITEBATCHER_H
#define FEATHER_SPRITEBATCHER_H

#include <iostream>
#include <vector>
#include "shapes/Vertex.h"
#include "vulkan/VulkanBuffer.h"
#include "vulkan/VulkanPipeline.h"

namespace Feather
{
	class SpriteBatcher
	{
	public:
		static void Create();
		static void Clear();
		static void Draw();
		static void Shutdown();

		static std::array<Vertex, 4> MakeQuad(glm::vec3 pos, glm::vec4 color, std::vector<glm::vec2> uvs, uint32_t texId, glm::mat4 mvp);
		static void AddQuad(std::array<Vertex, 4> quadVertices);

		static size_t GetDrawCallCount() { return drawCalls; }
		static size_t GetVerticesCount() { return vertexCount; }
		static size_t GetIndicesCount() { return indexCount; }
		static size_t GetQuadCount() { return quadCount; }

	private:
		static void CreateVertexBuffer();
		static void UpdateVertexBuffer();
		static void CreateIndexBuffer();
		static void UpdateIndexBuffer();

		static void Bind();
		static void Flush();

		inline static std::vector<Vertex> vertices{};
		inline static std::vector<uint32_t> indices{};

		inline static uint32_t vertexCount = 0; // number of vertices in the batch
		inline static uint32_t indexCount = 0; // number of indices in the batch
		inline static uint32_t quadCount = 0; // number of quads in the batch

		inline static std::shared_ptr<VulkanBuffer> vertexBuffer{};
		inline static std::shared_ptr<VulkanBuffer> indexBuffer{};

		inline static size_t drawCalls = 0;
		inline static uint32_t idxOffset = 0;
	};
}

#endif //FEATHER_SPRITEBATCHER_H
