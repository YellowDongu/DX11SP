#pragma once

#include "AutoPilot.h"

class SuperClassAutoPilot : public AutoPilot
{
private:
	SuperClassAutoPilot(void) = delete;
	SuperClassAutoPilot(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	SuperClassAutoPilot(const SuperClassAutoPilot& other);
	virtual ~SuperClassAutoPilot(void) = default;
	virtual void Free(void) override;
public:
	static SuperClassAutoPilot* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	virtual Engine::Component* Clone(void) override;

	virtual void Update(void) override
	{
		if (yoke == nullptr)
			return;

		if (transform == nullptr)
			transform = gameObject->transform();

		if (CheckAltitude())
			return;

		yoke->x = 0.0f;
		yoke->y = 0.0f;
		yoke->z = 0.0f;


		if (destination != (Vector3::one() * -1.0f))
		{
			TurnToDestination(destination);
		}
		else
			Leveling();
	}

	virtual void LateUpdate(void) override
	{
		if (transform == nullptr)
			transform = gameObject->transform();

		if ((destination - transform->Position()).magnitude() < waypointArrivalDistance)
		{
			if (wayPoints.empty())
			{
				destination = Vector3::one() * -1.0f;

			}
			else
			{
				destination = wayPoints.front();
				wayPoints.pop_front();
			}
		}
	}


	virtual void Turn(Vector3& angleDifference) override;
private:
	FLOAT waypointArrivalDistance = ConvertFeetToWorld(200.0f) * 5.0f;
};
