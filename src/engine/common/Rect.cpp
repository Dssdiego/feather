//
// Created by Diego S. Seabra on 01/03/23
//

#include "Rect.h"

namespace Feather
{
	// Constructors
	Rect::Rect() { }

	Rect::Rect(float x, float y, float width, float height)
	{
		mOrigin = glm::vec2(x, y);
		mWidth = width;
		mHeight = height;
	}

	Rect::Rect(glm::vec2 origin, float width, float height, bool isCenter)
	{
		mOrigin = isCenter ? glm::vec2(origin.x - width / 2.f, origin.y - height / 2.f) : mOrigin = origin;
		mWidth = width;
		mHeight = height;
	}

	bool Rect::IsInside(Rect& r1, Rect& r2)
	{
		return r1.GetOrigin().x > r2.GetOrigin().x && r1.GetSize().x < r2.GetSize().x &&
			   r1.GetOrigin().y > r2.GetOrigin().y && r1.GetSize().y < r2.GetSize().y;
	}

	bool Rect::IsEqual(Rect& r1, Rect& r2)
	{
		return r1.GetOrigin().x == r2.GetOrigin().x && r1.GetSize().x == r2.GetSize().x &&
			   r1.GetOrigin().y == r2.GetOrigin().y && r1.GetSize().y == r2.GetSize().y;
	}

	// AABB collision detection (two axes: x/y)
	bool Rect::IsIntersecting(Rect& r1, Rect& r2)
	{
		return r1.GetSize().x > r2.GetOrigin().x && r1.GetOrigin().x < r2.GetSize().x &&
			   r1.GetSize().y > r2.GetOrigin().y && r1.GetOrigin().y < r2.GetSize().y;
	}
}
