#include "pch.h"
#include "SuperClassPilot.h"


SuperClassPilot::SuperClassPilot(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : AutoPilot(dxDevice, dxDeviceContext)
{
}

SuperClassPilot::SuperClassPilot(const SuperClassPilot& other) : AutoPilot(other)
{
}

void SuperClassPilot::Free(void)
{
	AutoPilot::Free();
}

SuperClassPilot* SuperClassPilot::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext)
{
	SuperClassPilot* newInstance = new SuperClassPilot(dxDevice, dxDeviceContext);
	return newInstance;
}

Engine::Component* SuperClassPilot::Clone(void)
{
	return new SuperClassPilot(*this);
}

void SuperClassPilot::Turn(Vector3& directionDifference)
{
	float angle = 0.0f;
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
			angle = transform->Angle().z - 60.0f;

			if (std::abs(angle) > 3.0f)
			{
				yoke->z = angle / abs(angle) * -1.0f;
			}
			else
			{
				yoke->z = 0.0f;
			}
			yoke->x = 0.0f;


			angle = 0.0f;
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
			angle = transform->Angle().z + 60.0f;

			if (std::abs(angle) > 3.0f)
			{
				yoke->z = angle / abs(angle) * -1.0f;
			}
			else
			{
				yoke->z = 0.0f;
			}
			yoke->x = 0.0f;


			angle = 0.0f;
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
