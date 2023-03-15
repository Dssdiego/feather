//
// Created by Diego S. Seabra on 15/03/23
//

#define MINIAUDIO_IMPLEMENTATION

#include "MiniaudioBackend.h"
#include "../../profiling/Logger.h"

using namespace Feather;

void MiniAudioBackend::Init()
{
    result = ma_engine_init(NULL, &engine);
    if (result != MA_SUCCESS)
        Logger::Error("Could not initialize miniaudio backend", "");
    else 
	    Logger::Info("Miniaudio backend initialized");

    PlaySound2D("assets/sounds/start.wav");
}

void MiniAudioBackend::PlaySound2D(const std::string& soundPath)
{
    ma_engine_play_sound(&engine, soundPath.c_str(), NULL);
}

void MiniAudioBackend::Shutdown()
{
	Logger::Info("Shutting down miniaudio backend");

    ma_engine_uninit(&engine);
}

