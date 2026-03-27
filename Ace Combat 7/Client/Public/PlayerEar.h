#pragma once
#include "Component.h"

class PlayerEar : public Engine::Component
{
private:
	PlayerEar(void) = delete;
	PlayerEar(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	PlayerEar(const PlayerEar& other);
	virtual ~PlayerEar(void) = default;
	virtual void Free(void) override;
public:
	static PlayerEar* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, struct AircraftMetaData& metaData);
	virtual Engine::Component* Clone(void) override;

	HRESULT Start(void);
	virtual HRESULT Awake(void) override;
	virtual void Update(void) override;
	virtual void LateUpdate(void) override;
	virtual void FixedUpdate(void) override;

	void Weapons(void);
	void CombatWarning(void);
	void FlightWarning(void);
	void Enviorment(void);
	void Explosion(Vector3 worldPosition);
private:
	FLOAT timer = 0.0f, timer2 = 0.0f, timer3 = 0.0f;
	FLOAT engineRPMPitch = 0.0f, lockTimer{0.0f};
	bool locked{ false }, fireGun{false};
	AircraftMetaData* playerAircraftInfo;
	FMOD::Sound* standardMissileFired{ nullptr };
	FMOD::Sound* specialMissileFired{nullptr};
	FMOD::Sound* missileWarning{nullptr};
	FMOD::Sound* raderWarning{nullptr};
	FMOD::Sound* Warning{nullptr};
	FMOD::Sound* missileWarningVoice{nullptr};
	FMOD::Sound* raderWarningVoice{nullptr};
	FMOD::Sound* stallWarningVoice{nullptr};
	FMOD::Sound* engineOutside{nullptr};
	FMOD::Sound* engineInside{nullptr};
	FMOD::Sound* standardLock{nullptr};
	FMOD::Sound* gun{nullptr};
	FMOD::Sound* explosion{nullptr};

	FMOD::Channel* lockSoundChannel{nullptr};
	FMOD::Channel* gunChannel{nullptr};
	FMOD::Channel* voiceChannel{nullptr};
	FMOD::Channel* warningChannel{nullptr};
	FMOD::Channel* engineEffectChannel{nullptr};

	class FlightMovement* flightModule{nullptr};
	class FireControlSystem* fcs{nullptr};
	class RadarMissileWarningReceiver* rmwr{nullptr};


};
