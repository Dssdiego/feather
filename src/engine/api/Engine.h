//
// Created by Diego S. Seabra on 08/03/22.
//

#ifndef FEATHER_GAME_H
#define FEATHER_GAME_H

#include <Tracy.hpp>
#include <string>
#include "../common/structs.h"
#include "../audio/AudioEngine.h"
#include "../input/Input.h"
#include "../rendering/Window.h"
#include "../rendering/vulkan/VulkanEngine.h"
#include "../profiling/Logger.h"
#include "../sdks/GeforceNow.h"

namespace Feather
{
	class Engine
	{
	public:
		void Init(EngineConfig* pConfig); // initializes everything in the engine
		void Run();  // main loop
		void Draw(); // draw loop
		void Cleanup(); // shuts down the engine

	private:
		size_t frames = 0;
		int frameCount = 0;
		double previousTime = glfwGetTime();
	};
}

#endif //FEATHER_GAME_H
