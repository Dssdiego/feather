//
// Created by Diego S. Seabra on 15/03/23
//

#include "MiniaudioBackend.h"
#include "../../profiling/Logger.h"

using namespace Feather;

void MiniAudioBackend::Init()
{
	Logger::Info("Initializing miniaudio backend");
}

void MiniAudioBackend::Update()
{
}

void MiniAudioBackend::Shutdown()
{
	Logger::Info("Shutting down miniaudio backend");
}
