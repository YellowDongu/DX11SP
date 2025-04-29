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
	HRESULT Awake(void);
	virtual void Update(void) override;
	virtual void LateUpdate(void) override;
	virtual void FixedUpdate(void) override;

	void MissileFired(void);
	void CombatWarning(void);
	void FlightWarning(void);
private:
	FLOAT timer = 0.0f;
	FLOAT timer2 = 0.0f;
	FLOAT timer3 = 0.0f;
	AircraftMetaData* playerAircraftInfo;
	FMOD::Sound* standardMissileFired;
	FMOD::Sound* specialMissileFired;

	FMOD::Sound* missileWarning;
	FMOD::Sound* raderWarning;
	FMOD::Sound* Warning;
	FMOD::Sound* missileWarningVoice;
	FMOD::Sound* raderWarningVoice;
	FMOD::Sound* stallWarningVoice;
	FMOD::Sound* gun{nullptr};

	FMOD::Channel* gunChannel{nullptr};
	FMOD::Channel* voiceChannel{nullptr};
	FMOD::Channel* warningChannel{nullptr};

	class FlightMovement* flightModule;
	class FireControlSystem* fcs;
	class RadarMissileWarningReceiver* rmwr;


};
