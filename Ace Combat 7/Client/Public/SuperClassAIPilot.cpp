#include "pch.h"
#include "SuperClassAIPilot.h"
#include "RMWR.h"
#include "FlightMovement.h"
SuperClassAIPilot::SuperClassAIPilot(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : AIPilot(dxDevice, dxDeviceContext)
{
}

SuperClassAIPilot::SuperClassAIPilot(const SuperClassAIPilot& other) : AIPilot(other)
{
}

void SuperClassAIPilot::Free(void)
{
}

SuperClassAIPilot* SuperClassAIPilot::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, ObjectInfomation& objectInfomation)
{
    SuperClassAIPilot* newInstance = new SuperClassAIPilot(dxDevice, dxDeviceContext);
	newInstance->objectInfomation = objectInfomation;

    if (FAILED(newInstance->Start()))
    {
        Base::Destroy(newInstance);
        return nullptr;
    }

    return newInstance;
}

Engine::Component* SuperClassAIPilot::Clone(void)
{
    return new SuperClassAIPilot(*this);
}

void SuperClassAIPilot::Update(void)
{
	timer -= DeltaTime();
	defend = rmwr->ClosedWarning();

	if (gameObject->transform()->Position().y <= minimumAltitude)
		return;

	if (flightModule->Velocity() > targetVelocity)
	{
		flightModule->throttle = 0.0f;
		flightModule->airbreakActive = true;
	}
	else
	{
		flightModule->throttle = targetVelocity / flightModule->MinimumSpeed() - 1.0f;
		flightModule->throttle += flightModule->IdleThrottle();
		if (flightModule->throttle > 1.0f)
			flightModule->throttle = 1.0f;
		flightModule->airbreakActive = false;
	}

	return;


	if (defend)
	{
		Evade();
	}
	else
	{
		storage.x = -1.0f;
		storage.y = -1.0f;
		storage.z = -1.0f;
		if (target == nullptr)
			SearchEnemy();
		else
		{
			NonCombat();

			return;
			towardEnemy = target->transform()->Position() - gameObject->transform()->Position();
			direction = towardEnemy.normalize();
			enemyDistance = towardEnemy.magnitude();
			if (enemyDistance < ConvertFeetToWorld(7000.0f) * 5.0f)
			{
				ChaseEnemy();

				if (flightModule->yoke.x != 0.0f || flightModule->yoke.y != 0.0f || flightModule->yoke.z != 0.0f)
					flightModule->throttle = 1.0f;
				else
					flightModule->ThrottleIdle();
			}

			NonCombat();
		}
	}
}

void SuperClassAIPilot::FixedUpdate(void)
{

}

void SuperClassAIPilot::LateUpdate(void)
{
}
