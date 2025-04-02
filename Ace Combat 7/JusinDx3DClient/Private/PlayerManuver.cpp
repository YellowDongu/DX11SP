#include "pch.h"
#include "PlayerManuver.h"
#include "FlightMovement.h"
#include "InputManager.h"


PlayerManuver::PlayerManuver(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Engine::Component(dxDevice, dxDeviceContext), flightModule(nullptr)
{
}

PlayerManuver::PlayerManuver(const Component& other) : Engine::Component(other), flightModule(nullptr)
{
}

void PlayerManuver::Free(void)
{
}

PlayerManuver* PlayerManuver::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext)
{
	PlayerManuver* newInstance = new PlayerManuver(dxDevice, dxDeviceContext);

	//if (FAILED(newInstance->Start()))
	//{
	//	Base::Destroy(newInstance);
	//	return nullptr;
	//}
	return newInstance;
}

Engine::Component* PlayerManuver::Clone(void)
{
	return new PlayerManuver(*this);
}

void PlayerManuver::Update(void)
{
	if (flightModule == nullptr)
	{
		flightModule = dynamic_cast<FlightMovement*>(gameObject->GetComponent(L"FlightMovement"));
		if (flightModule == nullptr)
			return;
	}


	// get input
}

void PlayerManuver::LateUpdate(void)
{
	//if (flightModule == nullptr)
	//	return;


	// accept input
}

void PlayerManuver::FixedUpdate(void)
{
	//if (flightModule == nullptr)
	//	return;
}
