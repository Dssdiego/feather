//
// Created by Diego S. Seabra on 01/03/23
//

#include "Rect.h"

namespace Feather
{
	// Constructors
	Rect::Rect() { }

	Rect::Rect(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		mOrigin = glm::vec2(x, y);
		mWidth = width;
		mHeight = height;
	}

	Rect::Rect(glm::vec2 origin, uint32_t width, uint32_t height)
	{
		
	}
	
	// Getters/Setters
	glm::vec2 Rect::GetCenter()
	{
		// origin X + half Width
		// origin Y + half Height
		return glm::vec2();
	}
}
