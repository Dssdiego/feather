//
// Created by Diego S. Seabra on 15/03/23
//

#ifndef FEATHER_MINIAUDIO_BACKEND_H
#define FEATHER_MINIAUDIO_BACKEND_H

#include "../AudioBackend.h"
#include <iostream>

namespace Feather
{
	class MiniAudioBackend : public AudioBackend
	{
		void Init() override;
		void Update() override;
		void Shutdown() override;
	};
}

#endif //FEATHER_AUDIO_ENGINE_H
