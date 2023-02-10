//
// Created by Diego S. Seabra on 08/01/23
//

#ifndef FEATHER_MOUSE_H
#define FEATHER_MOUSE_H

#include "../profiling/Logger.h"
#include <yaml.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include "../rendering/Window.h"
#include "Input.h"

namespace Feather
{
	enum CursorMode
	{
		ARROW = 0,		// default mode
		GRABBING = 1
	};

	struct CursorData
	{
		std::string name = "";
		CursorMode mode = CursorMode::ARROW;
		std::string textureName = "";
		glm::vec2 hotspot = glm::vec2(0.f, 0.f);
		GLFWcursor* cursor = nullptr;
	};


	class Mouse
	{
	public:
		static void Init();
		static void Update();
		static void Shutdown();

		static void SetCursor(CursorMode mode);

	private:
		static void LoadCursorsFromYAML();
		static void CreateCursor(CursorData data);

		inline static std::vector<CursorData> cursors{};
	};
}

#endif //FEATHER_MOUSE_H
