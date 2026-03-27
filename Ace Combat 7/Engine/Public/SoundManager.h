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
		virtual void Free(void);
	public:
		static SoundManager* Create(void);

		HRESULT Start(void);
		void Update(void);

		HRESULT LoadSound(std::string path, std::string fileName);
		HRESULT LoadSound(std::string path, std::string fileName, FMOD::Sound*& output);
		FMOD::Sound* FindSound(std::wstring soundName);
		HRESULT Play(FMOD::Channel*& channel, FMOD::Sound*& sound, bool loop = false, float startPointMilliSecond = 0.0f);
		HRESULT Play(FMOD::Channel*& channel, wchar_t* soundName, bool loop = false, float startPointMilliSecond = 0.0f);
		HRESULT Play(FMOD::Channel*& channel, const std::wstring& soundName, bool loop = false, float startPointMilliSecond = 0.0f);
		inline FMOD::Channel* Play(FMOD::Sound*& sound, bool loop = false, float startPointMilliSecond = 0.0f) { FMOD::Channel* newChannel = nullptr; if(FAILED(Play(newChannel, sound, loop, startPointMilliSecond))) return nullptr; else return newChannel; }
		inline FMOD::Channel* Play(wchar_t* soundName, bool loop = false, float startPointMilliSecond = 0.0f) { FMOD::Channel* newChannel = nullptr; if(FAILED(Play(newChannel, soundName, loop, startPointMilliSecond))) return nullptr; else return newChannel; }
		inline FMOD::Channel* Play(const std::wstring& soundName, bool loop = false, float startPointMilliSecond = 0.0f) { FMOD::Channel* newChannel = nullptr; if(FAILED(Play(newChannel, soundName, loop, startPointMilliSecond))) return nullptr; else return newChannel; }
		HRESULT stopSound(FMOD::Channel*& channel);
		bool checkPlaying(FMOD::Channel*& channel);
		void changeVolume(FMOD::Channel*& channel, FLOAT value);
		void changePitch(FMOD::Channel*& channel, FLOAT value);
		
	private:
		FMOD::System* fmodSys;
		std::map<std::wstring, FMOD::Sound*> sounds;
		std::map<INT, FMOD::Channel*> channels;
	};
}