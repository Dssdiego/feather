//
// Created by Diego S. Seabra on 01/03/23
//

#ifndef FEATHER_RECT_H
#define FEATHER_RECT_H

#include <iostream>

namespace Feather
{
	class Rect
	{
	public:
		Rect();
		Rect(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

		uint32_t GetWidth() { return mWidth; }
		uint32_t GetHeight() { return mHeight; }
		uint32_t GetX() { return mX; }
		uint32_t GetY() { return mY; }

	private:
		uint32_t mX = 0;
		uint32_t mY = 0;
		uint32_t mWidth = 0;
		uint32_t mHeight = 0;
	};
}

#endif //FEATHER_RECT_H
