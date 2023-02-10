//
// Created by Diego S. Seabra on 31/01/23
//

#ifndef FEATHER_ENGINE_STATUS_PANEL_H
#define FEATHER_ENGINE_STATUS_PANEL_H

#include <string>

namespace Feather
{
	class EngineStatusPanel
	{
	public:
		void Draw();

	private:
		std::string deltaStr = "";
		std::string fpsStr = "";
		std::string frameStr = "";
		std::string drawCallStr = "";
		std::string verticesStr = "";
		std::string indicesStr = "";
		std::string quadsStr = "";
	};
}

#endif //FEATHER_ENGINE_STATUS_PANEL_H
