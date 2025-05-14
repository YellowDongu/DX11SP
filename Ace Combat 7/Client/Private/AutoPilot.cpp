#include "pch.h"
#include "AutoPilot.h"
#include "FlightMovement.h"

AutoPilot::AutoPilot(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Engine::Component(dxDevice, dxDeviceContext), destination(Vector3::zero())
{
}

AutoPilot::AutoPilot(const AutoPilot& other) : Engine::Component(other), destination(Vector3::zero())
{
}

void AutoPilot::Free(void)
{
}

AutoPilot* AutoPilot::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, FlightMovement* component)
{
	AutoPilot* newInstnace = new AutoPilot(dxDevice, dxDeviceContext);
	newInstnace->LinkInfomation(component);
	//if (FAILED(newInstnace->Start()))
	//{
	//	Base::Destroy(newInstnace);
	//	return nullptr;
	//}
	return newInstnace;
}

AutoPilot* AutoPilot::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext)
{
	AutoPilot* newInstnace = new AutoPilot(dxDevice, dxDeviceContext);
	return newInstnace;
}

Engine::Component* AutoPilot::Clone(void)
{
	return new AutoPilot(*this);
}

HRESULT AutoPilot::Awake(void)
{
	FlightMovement* flightMovement = static_cast<FlightMovement*>(gameObject->GetComponent(L"FlightMovement"));
	if (flightMovement == nullptr)
		return E_FAIL;
	LinkInfomation(flightMovement);

	return S_OK;
}

void AutoPilot::Update(void)
{
	if (yoke == nullptr)
		return;

	if (transform == nullptr)
		transform = gameObject->transform();

	if (CheckAltitude())
		return;


	if (destination != (Vector3::one() * -1.0f))
	{
		yoke->x = 0.0f;
		yoke->y = 0.0f;
		yoke->z = 0.0f;

		TurnToDestination(destination);
	}
}

void AutoPilot::LateUpdate(void)
{
	if (transform == nullptr)
		transform = gameObject->transform();

	if ((destination - transform->Position()).magnitude() < 100.0f)
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

void AutoPilot::FixedUpdate(void)
{

}

bool AutoPilot::CheckAltitude(void)
{
	if (transform->Position().y >= minimumAltitude)
		return false;

	yoke->x = 0.0f;
	yoke->y = 0.0f;
	yoke->z = 0.0f;

	Vector3 directionDifference = transform->Angle();
	directionDifference.y = 0.0f;
	directionDifference.x = 50.0f - directionDifference.x;
	//Vector3 directionDifference = currentAngle - Vector3::getDirection(transform->Forward());
	float angle = directionDifference.z;
	yoke->x = 0.0f;
	if (std::abs(angle) > 3.0f)
	{
		yoke->z = angle / abs(angle) * -1.0f;

	}
	else
	{
		yoke->z = 0.0f;
		if (std::abs(directionDifference.x) >= 3.0f)
		{
			angle = directionDifference.x;
			yoke->x = angle / abs(angle) * -1.0f;
		}
		else
		{
			yoke->x = 0.0f;
		}
	}

	*throttle = 1.0f;

	return true;
}

void AutoPilot::Leveling(void)
{
	float angle = transform->Angle().z;
	if (std::abs(angle) > 1.5f)
		yoke->z = angle / abs(angle) * -1.0f;
	else
		yoke->z = 0.0f;

	yoke->y = 0.0f;

	angle = transform->Angle().x;
	if (std::abs(angle) > 1.5f)
		yoke->x = angle / abs(angle) * -1.0f;
	else
		yoke->x = 0.0f;
}

void AutoPilot::TurnToDestination(Vector3& destination)
{
	float angle = 0.0f;
	Vector3 direction = (destination - transform->Position()).normalize();
	//Vector3 directionDifference = direction.getDirection() - Vector3::getDirection(transform->Forward());
	Vector3 directionDifference = Vector3::getDirection(transform->Forward()) - direction.getDirection();
	Turn(directionDifference);
	return;

	while (std::fabs(directionDifference.y) > 180.0f)
	{
		if (directionDifference.y > 0)
			directionDifference.y -= 360.0f;
		else
			directionDifference.y += 360.0f;
	}

	if (directionDifference.y > 0) 
	{

		if (std::abs(directionDifference.y) <= 10.0f)
		{
			angle = transform->Angle().z;
			if (std::abs(angle) > 15.0f)
			{
				yoke->z = angle / abs(angle) * -1.0f;
			}
			else
			{
				if (std::abs(angle) > 3.0f)
				{
					yoke->z = angle / abs(angle) * -1.0f;

				}
				else
				{
					yoke->z = 0.0f;
				}

				if (std::abs(directionDifference.x) >= 3.0f)
				{
					angle = directionDifference.x;
					yoke->x = angle / abs(angle) * -1.0f;
				}
				else
				{
					yoke->x = 0.0f;
				}

			}

			if (yoke->x == 0.0f && yoke->z == 0.0f)
			{
				angle = directionDifference.y;
				if (std::abs(angle) >= 0.5f)
				{
					yoke->y = angle / abs(angle);
				}
				else
				{
					yoke->y = 0.0f;
				}
			}


		}
		else
		{
			angle = transform->Angle().z - 90.0f;

			if (std::abs(angle) > 3.0f)
			{
				yoke->z = angle / abs(angle) * -1.0f;
			}
			else
			{
				yoke->z = 0.0f;
				yoke->x = -1.0f;
			}


			angle = directionDifference.x;
			if (std::abs(angle) >= 3.0f)
			{
				yoke->y = angle / abs(angle);
			}
			else
			{
				yoke->y = 0.0f;
			}
		}

	}
	else if(directionDifference.y < 0)
	{
		directionDifference.y *= -1.0f;
		if (directionDifference.y <= 10.0f)
		{
			angle = transform->Angle().z;
			if (std::abs(angle) > 15.0f)
			{
				yoke->z = angle / abs(angle) * -1.0f;
			}
			else
			{
				if (std::abs(angle) > 3.0f)
				{
					yoke->z = angle / abs(angle) * -1.0f;

				}
				else
				{
					yoke->z = 0.0f;
				}

				if (std::abs(directionDifference.x) >= 3.0f)
				{
					angle = directionDifference.x;
					yoke->x = angle / abs(angle) * -1.0f;
				}
				else
				{
					yoke->x = 0.0f;
				}

			}

			if (yoke->x == 0.0f && yoke->z == 0.0f)
			{
				angle = directionDifference.y;
				if (std::abs(angle) >= 0.5f)
				{
					yoke->y = angle / abs(angle) * -1.0f;
				}
				else
				{
					yoke->y = 0.0f;
				}
			}


		}
		else
		{
			angle = transform->Angle().z + 90.0f;

			if (std::abs(angle) > 3.0f)
			{
				yoke->z = angle / abs(angle) * -1.0f;
			}
			else
			{
				yoke->z = 0.0f;
				yoke->x = -1.0f;
			}


			angle = directionDifference.x;
			if (std::abs(angle) >= 3.0f)
			{
				yoke->y = angle / abs(angle);
			}
			else
			{
				yoke->y = 0.0f;
			}
		}

	}
}

void AutoPilot::TurnToDirection(Vector3& targetAngle)
{
	float angle = 0.0f;
	Vector3 directionDifference = targetAngle - Vector3::getDirection(transform->Forward());
	Turn(directionDifference);
	return;
	if (std::abs(directionDifference.y) <= 15.0f)
	{
		angle = transform->Angle().z;
		yoke->x = 0.0f;
		if (std::abs(angle) > 15.0f)
		{
			yoke->z = angle / abs(angle) * -1.0f;
		}
		else
		{
			if (std::abs(angle) > 3.0f)
			{
				yoke->z = angle / abs(angle) * -1.0f;

			}
			else
			{
				yoke->z = 0.0f;
			}

			if (std::abs(directionDifference.x) >= 3.0f)
			{
				angle = directionDifference.x;
				yoke->x = angle / abs(angle);
			}
			else
			{
				yoke->x = 0.0f;
			}

		}

		if (yoke->x == 0.0f && yoke->z == 0.0f)
		{
			angle = directionDifference.y;
			if (std::abs(angle) >= 0.5f)
			{
				yoke->y = angle / abs(angle) * -1.0f;
			}
			else
			{
				yoke->y = 0.0f;
			}
		}


	}
	else
	{
		angle = abs(transform->Angle().z) - 90.0f;

		if (std::abs(angle) > 3.0f)
		{
			yoke->z = angle / abs(angle) * -1.0f;
			yoke->x = 0.0f;
		}
		else
		{
			yoke->z = 0.0f;
			yoke->x = -1.0f;
		}


		angle = directionDifference.x;
		if (std::abs(angle) >= 3.0f)
		{
			yoke->y = angle / abs(angle);
		}
		else
		{
			yoke->y = 0.0f;
		}
	}
}

void AutoPilot::TurnToDirection(Vector3& targetAngle, bool& done)
{
	float angle = 0.0f;
	Vector3 directionDifference = targetAngle - Vector3::getDirection(transform->Forward());
	Turn(directionDifference);
	done = (directionDifference.x < 3.0f) && (directionDifference.y < 3.0f);
	return;
	if (std::abs(directionDifference.y) <= 15.0f)
	{
		angle = transform->Angle().z;
		yoke->x = 0.0f;
		if (std::abs(angle) > 15.0f)
		{
			yoke->z = angle / abs(angle) * -1.0f;
		}
		else
		{
			if (std::abs(angle) > 3.0f)
			{
				yoke->z = angle / abs(angle) * -1.0f;

			}
			else
			{
				yoke->z = 0.0f;
			}

			if (std::abs(directionDifference.x) >= 3.0f)
			{
				angle = directionDifference.x;
				yoke->x = angle / abs(angle);
			}
			else
			{
				yoke->x = 0.0f;
			}

		}

		if (yoke->x == 0.0f && yoke->z == 0.0f)
		{
			angle = directionDifference.y;
			if (std::abs(angle) >= 0.5f)
			{
				yoke->y = angle / abs(angle) * -1.0f;
			}
			else
			{
				yoke->y = 0.0f;
			}
		}


	}
	else
	{
		angle = abs(transform->Angle().z) - 90.0f;

		if (std::abs(angle) > 3.0f)
		{
			yoke->z = angle / abs(angle) * -1.0f;
			yoke->x = 0.0f;
		}
		else
		{
			yoke->z = 0.0f;
			yoke->x = -1.0f;
		}


		angle = directionDifference.x;
		if (std::abs(angle) >= 3.0f)
		{
			yoke->y = angle / abs(angle);
		}
		else
		{
			yoke->y = 0.0f;
		}
	}

	done = (directionDifference.x < 3.0f) && (directionDifference.y < 3.0f);
}

void AutoPilot::Turn(Vector3& directionDifference)
{
	float angle = 0.0f;
	while (std::fabs(directionDifference.y) > 180.0f)
	{
		if (directionDifference.y > 0)
			directionDifference.y -= 360.0f;
		else
			directionDifference.y += 360.0f;
	}

	if (directionDifference.y >= 0)
	{

		if (std::abs(directionDifference.y) <= 10.0f)
		{
			angle = transform->Angle().z;
			if (std::abs(angle) > 15.0f)
			{
				yoke->z = angle / abs(angle) * -1.0f;
			}
			else
			{
				if (std::abs(angle) > 3.0f)
				{
					yoke->z = angle / abs(angle) * -1.0f;

				}
				else
				{
					yoke->z = 0.0f;
				}

				if (std::abs(directionDifference.x) >= 3.0f)
				{
					angle = directionDifference.x;
					yoke->x = angle / abs(angle) * -1.0f;
					//yoke->x = angle / abs(angle);
				}
				else
				{
					yoke->x = 0.0f;
				}

			}

			if (yoke->x == 0.0f && yoke->z == 0.0f)
			{
				angle = directionDifference.y;
				if (std::abs(angle) >= 0.5f)
				{
					yoke->y = angle / abs(angle);
				}
				else
				{
					yoke->y = 0.0f;
				}
			}


		}
		else
		{
			angle = transform->Angle().z - 90.0f;

			if (std::abs(angle) > 3.0f)
			{
				yoke->z = angle / abs(angle) * -1.0f;
			}
			else
			{
				yoke->z = 0.0f;
				yoke->x = -1.0f;
			}


			angle = directionDifference.x;
			if (std::abs(angle) >= 3.0f)
			{
				yoke->y = angle / abs(angle);
			}
			else
			{
				yoke->y = 0.0f;
			}
		}

	}
	else if (directionDifference.y < 0)
	{
		directionDifference.y *= -1.0f;
		if (directionDifference.y <= 10.0f)
		{
			angle = transform->Angle().z;
			if (std::abs(angle) > 15.0f)
			{
				yoke->z = angle / abs(angle) * -1.0f;
			}
			else
			{
				if (std::abs(angle) > 3.0f)
				{
					yoke->z = angle / abs(angle) * -1.0f;

				}
				else
				{
					yoke->z = 0.0f;
				}

				if (std::abs(directionDifference.x) >= 3.0f)
				{
					angle = directionDifference.x;
					yoke->x = angle / abs(angle) * -1.0f;
				}
				else
				{
					yoke->x = 0.0f;
				}

			}

			if (yoke->x == 0.0f && yoke->z == 0.0f)
			{
				angle = directionDifference.y;
				if (std::abs(angle) >= 0.5f)
				{
					yoke->y = angle / abs(angle) * -1.0f;
				}
				else
				{
					yoke->y = 0.0f;
				}
			}


		}
		else
		{
			angle = transform->Angle().z + 90.0f;

			if (std::abs(angle) > 3.0f)
			{
				yoke->z = angle / abs(angle) * -1.0f;
			}
			else
			{
				yoke->z = 0.0f;
				yoke->x = -1.0f;
			}


			angle = directionDifference.x;
			if (std::abs(angle) >= 3.0f)
			{
				yoke->y = angle / abs(angle);
			}
			else
			{
				yoke->y = 0.0f;
			}
		}

	}
}
float AutoPilot::CalculateRollAngle(Vector3 forward, Vector3 up, Vector3 right, Vector3 targetDirection)
{
	Vector3 target = targetDirection.normalize();

	Vector3 projTarget = (target - forward * Vector3::dot(target, forward)).normalize();
	return atan2(Vector3::dot(Vector3::cross(up, projTarget), forward), Vector3::dot(up, projTarget));
}

void AutoPilot::LinkInfomation(FlightMovement* component)
{
	yoke = &component->yoke;
	throttle = &component->throttle;
	airBreak = &component->airbreakActive;
	throttleIdle = component->IdleThrottle();
	currentVelocity = &component->Velocity();
}
