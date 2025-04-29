#include "pch.h"
#include "RMWR.h"
#include "FireControlSystem.h"
#include "Missile.h"

RadarMissileWarningReceiver::RadarMissileWarningReceiver(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Engine::Component(dxDevice, dxDeviceContext)
{
}

RadarMissileWarningReceiver::RadarMissileWarningReceiver(const RadarMissileWarningReceiver& other) : Engine::Component(other)
{
}

void RadarMissileWarningReceiver::Free(void)
{
}

RadarMissileWarningReceiver* RadarMissileWarningReceiver::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext)
{
	RMWR* newInstance = new RMWR(dxDevice, dxDeviceContext);
	return newInstance;
}

Engine::Component* RadarMissileWarningReceiver::Clone(void)
{
	return new RadarMissileWarningReceiver(*this);
}

void RadarMissileWarningReceiver::Update(void)
{
	warning = false;

	for (auto iterator = hunters.begin(); iterator != hunters.end(); )
	{
		if ((*iterator)->LinkTarget() != gameObject)
			iterator = hunters.erase(iterator);
		else
		{
			if (!warning)
				warning = (*iterator)->LockStatus() != 1.0f;
			else
				break;
			iterator++;
		}
	}


}


void RadarMissileWarningReceiver::LateUpdate(void)
{
	missileWarning = false;
	closedWarning = false;
	for (auto iterator = chasedMissile.begin(); iterator != chasedMissile.end(); )
	{
		if (closedWarning)
			break;
		if ((*iterator)->Detonated())
			iterator = chasedMissile.erase(iterator);
		else
		{
			if (!closedWarning)
			{
				missileWarning = true;
				closedWarning = (gameObject->transform()->Position() - (*iterator)->transform()->Position()).magnitude() < ConvertFeetToWorld(1500.0f);
				float test = (gameObject->transform()->Position() - (*iterator)->transform()->Position()).magnitude();
				if (test < ConvertFeetToWorld(2000.0f))
					int alarm = 0;
			}
			iterator++;
		}
	}
}

void RadarMissileWarningReceiver::FixedUpdate(void)
{
}
