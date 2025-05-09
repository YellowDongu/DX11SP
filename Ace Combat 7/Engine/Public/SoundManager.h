#pragma once
#include "Base.h"

namespace Engine
{
	class ENGINEDLL SoundManager final : public Base
	{
	private:
		SoundManager(void);
		virtual ~SoundManager(void);

		SoundManager(const SoundManager&) = delete;
		SoundManager& operator=(const SoundManager&) = delete;
	public:
		static SoundManager* Create(void);
		void Update(void);

		HRESULT LoadSound(std::string path, std::string fileName);
		FMOD::Sound*& FindSound(std::wstring soundName);
		void playBGM(std::wstring soundName);
		FMOD::Channel* playNew(std::wstring soundName);
		FMOD::Channel* Play(FMOD::Channel*& channel, FMOD::Sound*& sound, float startPointMilliSecond = 0.0f);
		FMOD::Channel* Play(FMOD::Channel*& channel, wchar_t* soundName, float startPointMilliSecond = 0.0f);
		FMOD::Channel* Play(FMOD::Channel*& channel, const std::wstring& soundName, float startPointMilliSecond = 0.0f);
		HRESULT stopSound(FMOD::Channel*& channel);
		bool checkPlaying(FMOD::Channel*& channel);
		void changeVolume(FMOD::Channel*& channel, FLOAT value);
		void changePitch(FMOD::Channel*& channel, FLOAT value);
		FMOD::Channel*& GetBGMchannel(void) { return BGMchannel; }
		
	private:
		HRESULT StartSoundDevice(void);
		virtual void Free(void);

		FMOD::System* fmodSys;
		std::map<std::wstring, FMOD::Sound*> sounds;
		std::map<INT, FMOD::Channel*> channels;
		FMOD::Channel* BGMchannel;
	};
}