//
// Created by Diego S. Seabra on 01/03/23
//

#ifndef FEATHER_RECT_H
#define FEATHER_RECT_H

#include <iostream>
#include <glm/glm.hpp>

namespace Feather
{
	class Rect
	{
	public:
		Rect();
		Rect(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
		Rect(glm::vec2 origin, uint32_t width, uint32_t height);

		uint32_t GetWidth() { return mWidth; }
		uint32_t GetHeight() { return mHeight; }

		glm::vec2 GetOrigin() { return mOrigin; }
		glm::vec2 GetCenter();

	private:
		glm::vec2 mOrigin = glm::vec2(0.f,0.f);

		uint32_t mWidth = 0;
		uint32_t mHeight = 0;
	};
}

#endif //FEATHER_RECT_H
