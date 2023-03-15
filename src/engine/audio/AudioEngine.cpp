//
// Created by Diego S. Seabra on 15/03/23
//

#include "AudioEngine.h"

using namespace Feather;

void AudioEngine::SetupBackend(AudioBackend* audioBackend)
{
	mBackend = audioBackend;
}

void AudioEngine::Init()
{
	mBackend->Init();
}

void AudioEngine::Update()
{
	mBackend->Update();
}

void AudioEngine::Shutdown()
{
	mBackend->Shutdown();
}
