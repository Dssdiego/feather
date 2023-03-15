//
// Created by Diego S. Seabra on 15/03/23
//

#ifndef FEATHER_MINIAUDIO_BACKEND_H
#define FEATHER_MINIAUDIO_BACKEND_H

#include "../AudioBackend.h"

#include <iostream>
#include <miniaudio.h>

namespace Feather
{
	class MiniAudioBackend : public AudioBackend
	{
	public:
		void Init() override;
		void Shutdown() override;

		void PlaySound2D(const std::string& soundPath) override;

	private:
		ma_result result;
		ma_engine engine;
	};
}

#endif //FEATHER_AUDIO_ENGINE_H
