//
// Created by Diego S. Seabra on 15/03/23
//

#ifndef FEATHER_AUDIO_ENGINE_H
#define FEATHER_AUDIO_ENGINE_H

#include "AudioBackend.h"
#include "backends/MiniAudioBackend.h"

namespace Feather
{
	class AudioEngine {
	public:
		static void SetupBackend(AudioBackend* audioBackend);
		static void Init();
		static void Update();
		static void Shutdown();

	private:
		inline static AudioBackend* mBackend = nullptr;
	};
}

#endif //FEATHER_AUDIO_ENGINE_H
