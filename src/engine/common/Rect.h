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
		Rect(float x, float y, float width, float height);
		Rect(glm::vec2 origin, float width, float height, bool isCenter = false);

		float GetWidth() { return mWidth; }
		float GetHeight() { return mHeight; }

		glm::vec2 GetOrigin() { return mOrigin; }
		glm::vec2 GetCenter() { return glm::vec2(mOrigin.x + mWidth / 2.f, mOrigin.y + mHeight / 2.f); }

		glm::vec2 GetSize() { return glm::vec2(mWidth, mHeight); }

		static bool IsInside(Rect& r1, Rect& r2);
		static bool IsIntersecting(Rect& r1, Rect& r2); // AABB collision detection (two axes: x/y)
	
	private:
		glm::vec2 mOrigin = glm::vec2(0.f, 0.f);

		uint32_t mWidth = 0;
		uint32_t mHeight = 0;
	};
}

#endif //FEATHER_RECT_H
