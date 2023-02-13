//
// Created by Diego S. Seabra on 20/08/22.
//

#ifndef FEATHER_SHAPE_H
#define FEATHER_SHAPE_H

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include "../vulkan/VulkanDevice.h"
#include "../vulkan/VulkanEngine.h"
#include "Vertex.h"
#include "../vulkan/VulkanBuffer.h"
#include <vector>

namespace Feather
{
	enum ShapeType {
		Triangle,
		Circle,
		Quad
	};

	class Shape
	{
	public:
		Shape(ShapeType type);

		virtual void Draw();
		virtual void Destroy();

		void ReplaceUVs(float startX, float startY, float endX, float endY);
		static ShapeType GetEnumFromString(const std::string& value);

	protected:
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;

	private:
		void DefineTriangle();
		void DefineCircle();
		void DefineQuad();

		void MakeShape();
		void CreateVertexBuffer();
		void CreateIndexBuffer();
		void Bind();

		std::shared_ptr<VulkanBuffer> vertexBuffer;
		std::shared_ptr<VulkanBuffer> indexBuffer;

		uint32_t vertexCount;
		uint32_t indexCount;
	};
}

#endif //FEATHER_SHAPE_H
