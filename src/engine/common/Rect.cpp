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
		mX = x;
		mY = y;
		mWidth = width;
		mHeight = height;
	}
}
