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
	Sound()->LoadSound("../Bin/Resources/Sounds/Effects/", "VoiceMissile.wav", missileWarningVoice);
	Sound()->LoadSound("../Bin/Resources/Sounds/Effects/", "MissileWarn.wav", missileWarning);
	Sound()->LoadSound("../Bin/Resources/Sounds/Effects/", "MissileFired.wav", standardMissileFired);
	Sound()->LoadSound("../Bin/Resources/Sounds/Effects/", "281594262.wav", specialMissileFired);
	Sound()->LoadSound("../Bin/Resources/Sounds/Effects/", "305721730.wav", standardLock);
	Sound()->LoadSound("../Bin/Resources/Sounds/Effects/", "Valcan20mm.wav", gun);
	Sound()->LoadSound("../Bin/Resources/Sounds/Effects/", "Engine_Outside.wav", engineOutside);
	Sound()->LoadSound("../Bin/Resources/Sounds/Effects/", "449767323.wav", explosion);
	//Sound()->LoadSound("../Bin/Resources/Sounds/Effects/", "Engine_Outside.wav", engineInside);

	
	return S_OK;	
}

HRESULT PlayerEar::Awake(void)
{
	flightModule = static_cast<FlightMovement*>(gameObject->GetComponent(L"FlightMovement"));
	fcs = static_cast<FireControlSystem*>(gameObject->GetComponent(L"FCS"));
	rmwr = static_cast<RMWR*>(gameObject->GetComponent(L"RMWR"));


	return E_NOTIMPL;
}

void PlayerEar::Update(void)
{
}

void PlayerEar::LateUpdate(void)
{
	Weapons();
	Enviorment();
	CombatWarning();
}

void PlayerEar::FixedUpdate(void)
{
}

float MapRange(float value, float beforeMin, float beforeMax, float afterMin, float afterMax)
{
	return afterMin + ((value - beforeMin) / (beforeMax - beforeMin)) * (afterMax - afterMin);
}
float remapMiddleToDouble(float targetValue, float middle)
{
	if (targetValue <= middle)
		return targetValue / middle;               // 0 ~ x  ¡æ  0 ~ 1
	else
		return 1 + (targetValue - middle) / (1 - middle); // x ~ 1  ¡æ  1 ~ 2
}
void PlayerEar::Weapons(void)
{
	if (fcs->LockStatus() == 0.0f)
	{
		if (!locked)
		{
			::Sound()->Play(lockSoundChannel, standardLock, true);
			locked = true;
		}
	}
	else if (fcs->LockStatus() < 1.0f)
	{
		if (locked)
		{
			::Sound()->stopSound(lockSoundChannel);
			locked = false;
		}
		lockTimer -= DeltaTime();
		if (lockTimer <= 0.0f)
		{
			::Sound()->Play(lockSoundChannel, standardLock);
			lockTimer += fcs->LockStatus() * 0.75f;
		}
	}
	else if (locked)
	{
		::Sound()->stopSound(lockSoundChannel);
		locked = false;
		lockTimer = 0.0f;
	}


	if (fcs->StandardMissileFired())
		::Sound()->Play(standardMissileFired);
	if (fcs->UniqueMissileFired())
		::Sound()->Play(specialMissileFired);

	if (::Input()->getButton(KeyType::LCtrl))
	{
		if (!fireGun)
		{
			::Sound()->Play(gunChannel, gun, true);
			fireGun = true;
		}
	}
	else if (fireGun)
	{
		::Sound()->stopSound(gunChannel);
		fireGun = false;
	}
}

void PlayerEar::CombatWarning(void)
{
	timer -= DeltaTime();
	timer2 -= DeltaTime();
	if (rmwr->ClosedWarning())
	{
		if (!::Sound()->checkPlaying(voiceChannel) && timer2 < 0.0f)
		{
			Sound()->Play(voiceChannel, missileWarningVoice);
			timer2 = 2.0f;
		}

		if (!::Sound()->checkPlaying(warningChannel) && timer < 0.0f)
		{
			Sound()->Play(warningChannel, missileWarning);
			timer = 0.1f;
		}

	}
	else if (rmwr->MissileWarning())
	{
		if (!::Sound()->checkPlaying(voiceChannel) && timer2 < 0.0f)
		{
			Sound()->Play(voiceChannel, missileWarningVoice);
			timer2 = 2.0f;
		}

		if (!::Sound()->checkPlaying(warningChannel) && timer < 0.0f)
		{
			Sound()->Play(warningChannel, missileWarning);
			timer = 1.0f;
		}
	}
	//else if (rmwr->Warning())
	//{
	//
	//}


}

void PlayerEar::FlightWarning(void)
{
	if ((flightModule->Velocity() / flightModule->MinimumSpeed()) < 0.75f)
	{
		//stallWarning
	}
}

void PlayerEar::Enviorment(void)
{
	static float topValue = 1.5f, bottomValue = 0.5f;

	if (!::Sound()->checkPlaying(engineEffectChannel))
	{
		::Sound()->Play(engineEffectChannel, engineOutside, true);
	}
	else
	{
		if (flightModule->throttle > flightModule->IdleThrottle())
		{
			if (engineRPMPitch >= topValue)
				engineRPMPitch = topValue;
			else
				engineRPMPitch += DeltaTime();

		}
		else if (flightModule->throttle < flightModule->IdleThrottle())
		{
			if (engineRPMPitch <= bottomValue)
				engineRPMPitch = bottomValue;
			else
				engineRPMPitch -= DeltaTime();
		}
		else
		{
			if (engineRPMPitch != 1.0f)
			{
				engineRPMPitch += std::copysignf(DeltaTime(), (engineRPMPitch - 1.0f) * -1.0f);

				if (engineRPMPitch < 1.01f && engineRPMPitch > 0.99f)
					engineRPMPitch = 1.0f;
			}
		}
		::Sound()->changePitch(engineEffectChannel, engineRPMPitch);
	}

}

void PlayerEar::Explosion(Vector3 worldPosition)
{
	static FLOAT maxDistance = ::ConvertFeetToWorld(3000.0f) * 2.5f;
	FLOAT distance = (gameObject->transform()->Position() - worldPosition).magnitude();

	FLOAT soundVolume = ::clamp((1.0f - distance / maxDistance) * 1.25f, 0.0f, 1.0f);
	if (soundVolume < 0.1f)
		return;

	FMOD::Channel* tempralChannel = ::Sound()->Play(explosion);
	if(tempralChannel != nullptr)
		tempralChannel->setVolume(soundVolume);
}
