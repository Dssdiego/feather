//
// Created by Diego S. Seabra on 30/03/23
//

#include "TextEngine.h"
#include "../profiling/Logger.h"

namespace Feather
{
	void TextEngine::Init()
	{
		auto error = FT_Init_FreeType(&library);
		if (!error)
			Logger::Info("FreeType library initialized");
		else
			Logger::Error("Couldn't initialize the FreeType library", "");
	}
}

