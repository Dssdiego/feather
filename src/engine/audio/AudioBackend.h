//
// Created by Diego S. Seabra on 15/03/23
//

#ifndef FEATHER_AUDIO_BACKEND_H
#define FEATHER_AUDIO_BACKEND_H

namespace Feather
{
	class AudioBackend
	{
	public:
		virtual void Init() {};
		virtual void Update() {};
		virtual void Shutdown() {};
	};
}

#endif //FEATHER_AUDIO_ENGINE_H
