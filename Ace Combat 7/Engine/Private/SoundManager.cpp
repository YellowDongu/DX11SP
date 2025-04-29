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
	if (sounds.find(To_WString(fileName)) != sounds.end())
		return S_OK;

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

	fmodSys->playSound(sound->second, 0, false, &newChannel);
	return newChannel;
}

FMOD::Channel* SoundManager::Play(FMOD::Channel*& channel, FMOD::Sound*& sound, float time)
{
	if (sound == nullptr)
		return nullptr;

	if (time == 0.0f)
	{
		if (fmodSys->playSound(sound, 0, false, &channel) != FMOD_OK)
			return nullptr;
	}
	else
	{
		if (fmodSys->playSound(sound, 0, true, &channel) != FMOD_OK)
			return nullptr;
		int startPos;
		float frequency;

		sound->getFormat(nullptr, nullptr, nullptr, &startPos);
		sound->getDefaults(&frequency, nullptr);
		unsigned int pcmStart = static_cast<unsigned int>((time / 1000.0f) * frequency);
		channel->setPosition(pcmStart, FMOD_TIMEUNIT_PCM);

		channel->setPaused(false);
	}

	return channel;
}

FMOD::Channel* SoundManager::Play(FMOD::Channel*& channel, wchar_t* soundName, float time)
{
	auto sound = sounds.find(soundName);
	if (sound == sounds.end())
		return nullptr;

	if (time == 0.0f)
	{
		if (fmodSys->playSound(sound->second, 0, false, &channel) != FMOD_OK)
			return nullptr;
	}
	else
	{
		if (fmodSys->playSound(sound->second, 0, true, &channel) != FMOD_OK)
			return nullptr;

		int startPos;
		float frequency;
		sound->second->getFormat(nullptr, nullptr, nullptr, &startPos);
		sound->second->getDefaults(&frequency, nullptr);

		unsigned int pcmStart = static_cast<unsigned int>((time / 1000.0f) * frequency);
		channel->setPosition(pcmStart, FMOD_TIMEUNIT_PCM);
		channel->setPaused(false);
	}
	return channel;
}

FMOD::Channel* SoundManager::Play(FMOD::Channel*& channel, const std::wstring& soundName, float time)
{
	auto sound = sounds.find(soundName);
	if (sound == sounds.end())
		return nullptr;

	if (time == 0.0f)
	{
		if (fmodSys->playSound(sound->second, 0, false, &channel) != FMOD_OK)
			return nullptr;
	}
	else
	{
		if (fmodSys->playSound(sound->second, 0, true, &channel) != FMOD_OK)
			return nullptr;

		int startPos;
		float frequency;
		sound->second->getFormat(nullptr, nullptr, nullptr, &startPos);
		sound->second->getDefaults(&frequency, nullptr);

		unsigned int pcmStart = static_cast<unsigned int>((time / 1000.0f) * frequency);
		channel->setPosition(pcmStart, FMOD_TIMEUNIT_PCM);
		channel->setPaused(false);
	}
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

void SoundManager::changePitch(FMOD::Channel*& channel, FLOAT value)
{
	channel->setPitch(value);
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

