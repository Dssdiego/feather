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

		LoadFont("assets/fonts/OpenSans-Regular.ttf");
	}

	void TextEngine::LoadFont(const std::string& filePath)
	{
		FT_Face face;

		auto error = FT_New_Face(library, filePath.c_str(), 0, &face);
		if (error == FT_Err_Unknown_File_Format)
			Logger::Error("Unknown font format", "");
		else if (error)
			Logger::Error("Could not load font", "");
		else
			Logger::Info("Font loaded");
	}
}

