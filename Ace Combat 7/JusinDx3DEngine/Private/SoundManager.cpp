#include "Foundation.h"
#include "SoundManager.h"

using namespace Engine;

SoundManager::SoundManager(void) : sounds{}, channels{}, fmodSys(nullptr), BGMchannel(nullptr)
{
}

SoundManager::~SoundManager(void)
{

}


SoundManager* SoundManager::Create(void)
{
	SoundManager* newInstance = new SoundManager();
	if (FAILED(newInstance->StartSoundDevice()))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}
	return newInstance;
}

HRESULT SoundManager::StartSoundDevice(void)
{
	if (FMOD::System_Create(&fmodSys) != FMOD_OK)
		return E_FAIL;
	if (fmodSys->init(32, FMOD_INIT_NORMAL, 0) != FMOD_OK)
		return E_FAIL;
	if (fmodSys->update() != FMOD_OK)
		return E_FAIL;

	return S_OK;
}


void SoundManager::Update(void)
{
	if (fmodSys->update() != FMOD_OK)
	{
		Free();
		if (FAILED(StartSoundDevice()))
		{
			Free();
			PostQuitMessage(0);
		}
	}
}

HRESULT SoundManager::LoadSound(std::string path, std::string fileName)
{
	FMOD::Sound* newSound = nullptr;
	if (fmodSys->createSound((path + fileName).c_str(), FMOD_DEFAULT, 0, &newSound) != FMOD_OK)
		return E_FAIL;
	sounds[To_WString(fileName)] = newSound;
	return S_OK;
}

FMOD::Sound*& SoundManager::FindSound(std::wstring soundName)
{
	FMOD::Sound* result = nullptr;
	auto sound = sounds.find(soundName);
	if (sound == sounds.end())
		return result;
	return (*sound).second;
}

void SoundManager::playBGM(std::wstring soundName)
{
	if (checkPlaying(BGMchannel))
		BGMchannel->stop();

	auto sound = sounds.find(soundName);
	if (sound == sounds.end()) return;

	fmodSys->playSound(sound->second, 0, false, &BGMchannel);
	BGMchannel->setMode(FMOD_LOOP_NORMAL);
	BGMchannel->setLoopCount(-1);
}

FMOD::Channel* SoundManager::playNew(std::wstring soundName)
{
	FMOD::Channel* newChannel = nullptr;

	auto sound = sounds.find(soundName);
	if (sound == sounds.end()) return nullptr;

	fmodSys->playSound(sound->second, 0, false, &BGMchannel);
	return newChannel;
}

FMOD::Channel* SoundManager::Play(FMOD::Channel*& channel, FMOD::Sound*& sound)
{
	if (sound == nullptr)
		return nullptr;

	if (fmodSys->playSound(sound, 0, false, &channel) != FMOD_OK)
		return nullptr;
	return channel;
}

HRESULT SoundManager::stopSound(FMOD::Channel*& channel)
{
	if (!checkPlaying(channel))
		return S_OK;
	if (channel->stop() != FMOD_OK)
		return E_FAIL;

	return S_OK;
}

bool SoundManager::checkPlaying(FMOD::Channel*& channel)
{
	bool playing = false;
	if (channel->isPlaying(&playing) != FMOD_OK) return false;
	return playing;
}

void SoundManager::changeVolume(FMOD::Channel*& channel, FLOAT value)
{
	if (value > 1.0f) value = 1.0f;
	else if (value < 0.0f) value = 0.0f;
	channel->setVolume(value);
}

void SoundManager::Free(void)
{
	if (BGMchannel)
		stopSound(BGMchannel);

	for (auto& sound : sounds)
	{
		sound.second->release();
	}
	sounds.clear();

	fmodSys->close();
	fmodSys->release();
}

