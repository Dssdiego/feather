//
// Created by Diego S. Seabra on 15/03/23
//

#ifndef FEATHER_AUDIO_BACKEND_H
#define FEATHER_AUDIO_BACKEND_H

#include <string>

namespace Feather
{
	class AudioBackend
	{
	public:
		virtual void Init() {};
		virtual void Update() {};
		virtual void Shutdown() {};

		virtual void PlaySound2D(const std::string& soundPath) {};
	};
}

#endif //FEATHER_AUDIO_ENGINE_H
