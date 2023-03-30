//
// Created by Diego S. Seabra on 30/03/23
//

#ifndef FEATHER_TEXT_ENGINE_H
#define FEATHER_TEXT_ENGINE_H

#include <ft2build.h>
#include FT_FREETYPE_H

namespace Feather
{
	class TextEngine
	{
	public:
		static void Init();

	private:
		inline static FT_Library library{};
	};
}

#endif //FEATHER_TEXT_ENGINE_H
