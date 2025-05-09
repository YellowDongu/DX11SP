#include "pch.h"
#include "GroundAI.h"
#include "FireControlSystem.h"
#include "RMWR.h"

GroundAI::GroundAI(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : AIPilot(dxDevice, dxDeviceContext)
{
}

GroundAI::GroundAI(const GroundAI& other) : AIPilot(other)
{
}

void GroundAI::Free(void)
{
	AIPilot::Free();
}

GroundAI* GroundAI::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, ObjectInfomation& objectInfomation)
{
	GroundAI* newInstance = new GroundAI(dxDevice, dxDeviceContext);
	newInstance->objectInfomation = objectInfomation;
	if (FAILED(newInstance->Start()))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}
	return newInstance;
}

Engine::Component* GroundAI::Clone(void)
{
	return new GroundAI(*this);
}

HRESULT GroundAI::Start(void)
{
	return S_OK;
}

HRESULT GroundAI::Awake(void)
{
	if (gameObject == nullptr)
		return E_FAIL;

	fcs = static_cast<FireControlSystem*>(gameObject->GetComponent(L"FCS"));
	if (fcs == nullptr)
		return E_FAIL;
	target = &fcs->LinkTarget();

	rmwr = static_cast<RMWR*>(gameObject->GetComponent(L"RMWR"));
	if (rmwr == nullptr)
		return E_FAIL;


	return S_OK;
}


void GroundAI::Update(void)
{
	if (*target != nullptr)
	{
		Engine::Transform* transform = gameObject->transform();
		Vector3 directionVector = (*target)->transform()->Position() - transform->Position();
		Vector3 directionAngle = directionVector.getDirection();
		direction.x = 0.0f;
		transform->SetAngle(directionAngle);

		if (fireDistance >= directionVector.magnitude())
			fcs->weaponRelease = fcs->gunFire = true;
		else
			fcs->weaponRelease = fcs->gunFire = false;
	}
}

void GroundAI::LateUpdate(void)
{
}

void GroundAI::FixedUpdate(void)
{
}
