#include "pch.h"
#include "PlayerEar.h"

#include "FlightMovement.h"
#include "FireControlSystem.h"
#include "RMWR.h"

PlayerEar::PlayerEar(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Engine::Component(dxDevice, dxDeviceContext), playerAircraftInfo(nullptr), standardMissileFired(nullptr), specialMissileFired(nullptr), missileWarning(nullptr), raderWarning(nullptr), flightModule(nullptr), fcs(nullptr), rmwr(nullptr)
{
}

PlayerEar::PlayerEar(const PlayerEar& other) : Engine::Component(other), playerAircraftInfo(other.playerAircraftInfo), standardMissileFired(other.standardMissileFired), specialMissileFired(other.specialMissileFired), missileWarning(other.missileWarning), raderWarning(other.raderWarning), flightModule(other.flightModule), fcs(other.fcs), rmwr(other.rmwr)
{
}

void PlayerEar::Free(void)
{
}

PlayerEar* PlayerEar::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, AircraftMetaData& metaData)
{
	PlayerEar* newInstnace = new PlayerEar(dxDevice, dxDeviceContext);
	newInstnace->playerAircraftInfo = &metaData;

	if (FAILED(newInstnace->Start()))
	{
		Base::Destroy(newInstnace);
		return nullptr;
	}

	return newInstnace;
}

Engine::Component* PlayerEar::Clone(void)
{
	return new PlayerEar(*this);
}

HRESULT PlayerEar::Start(void)
{
	Sound()->LoadSound("../Bin/Resources/Sounds/Effects/", "VoiceMissile.wav");
	Sound()->LoadSound("../Bin/Resources/Sounds/Effects/", "MissileWarn.wav");
	Sound()->LoadSound("../Bin/Resources/Sounds/Effects/", "MissileFired.wav");
	Sound()->LoadSound("../Bin/Resources/Sounds/Effects/", "Valcan20mm.wav");

	return S_OK;
}

HRESULT PlayerEar::Awake(void)
{
	flightModule = static_cast<FlightMovement*>(gameObject->GetComponent(L"FlightMovement"));
	fcs = static_cast<FireControlSystem*>(gameObject->GetComponent(L"FCS"));
	rmwr = static_cast<RMWR*>(gameObject->GetComponent(L"RMWR"));


	missileWarningVoice = Sound()->FindSound(L"VoiceMissile.wav");
	missileWarning = Sound()->FindSound(L"MissileWarn.wav");
	standardMissileFired = Sound()->FindSound(L"MissileFired.wav");
	gun = Sound()->FindSound(L"Valcan20mm.wav");

	return E_NOTIMPL;
}

void PlayerEar::Update(void)
{
	CombatWarning();
}

void PlayerEar::LateUpdate(void)
{
	MissileFired();
}

void PlayerEar::FixedUpdate(void)
{
}

void PlayerEar::MissileFired(void)
{
	if (fcs->StandardMissileFired())
		Sound()->playNew(L"MissileFired.wav");
	if (fcs->gunFire)
	{
		if (!Sound()->checkPlaying(gunChannel))
			Sound()->Play(gunChannel, gun);
	}
	else
	{
		if (Sound()->checkPlaying(gunChannel))
			Sound()->stopSound(gunChannel);
	}


}

void PlayerEar::CombatWarning(void)
{
	timer -= DeltaTime();
	timer2 -= DeltaTime();
	if (rmwr->ClosedWarning())
	{
		if (!Sound()->checkPlaying(voiceChannel) && timer2 < 0.0f)
		{
			Sound()->Play(voiceChannel, missileWarningVoice);
			timer2 = 2.0f;
		}

		if (!Sound()->checkPlaying(warningChannel) && timer < 0.0f)
		{
			Sound()->Play(warningChannel, missileWarning);
			timer = 0.1f;
		}

	}
	else if (rmwr->MissileWarning())
	{
		if (!Sound()->checkPlaying(voiceChannel) && timer2 < 0.0f)
		{
			Sound()->Play(voiceChannel, missileWarningVoice);
			timer2 = 2.0f;
		}

		if (!Sound()->checkPlaying(warningChannel) && timer < 0.0f)
		{
			Sound()->Play(warningChannel, missileWarning);
			timer = 1.0f;
		}
	}
	else if (rmwr->Warning())
	{

	}


}

void PlayerEar::FlightWarning(void)
{
	if ((flightModule->Velocity() / flightModule->MinimumSpeed()) < 0.75f)
	{
		//stallWarning
	}
}
