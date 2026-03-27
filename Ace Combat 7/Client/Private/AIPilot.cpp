#include "pch.h"
#include "AIPilot.h"
#include "FlightMovement.h"
#include "AircraftBoneHandler.h"
#include "FireControlSystem.h"
#include "AutoPilot.h"
#include "RMWR.h"

AIPilot::AIPilot(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Engine::Component(dxDevice, dxDeviceContext)
{
}

AIPilot::AIPilot(const AIPilot& other) : Engine::Component(other), objectInfomation(other.objectInfomation)
{
}

void AIPilot::Free(void)
{
}

AIPilot* AIPilot::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, ObjectInfomation& objectInfomation)
{
	AIPilot* newInstance = new AIPilot(dxDevice, dxDeviceContext);
	newInstance->objectInfomation = objectInfomation;
	if (FAILED(newInstance->Start()))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}

	return newInstance;
}

Engine::Component* AIPilot::Clone(void)
{
	return nullptr;
}

HRESULT AIPilot::Awake(void)
{
	if (gameObject == nullptr)
		return E_FAIL;

	flightModule = static_cast<FlightMovement*>(gameObject->GetComponent(L"FlightMovement"));
	boneHandler = static_cast<AircraftBoneHandler*>(gameObject->GetComponent(L"AircraftBoneHandler"));
	fcs = static_cast<FireControlSystem*>(gameObject->GetComponent(L"FCS"));
	autoPilot = static_cast<AutoPilot*>(gameObject->GetComponent(L"AutoPilot"));
	rmwr = static_cast<RMWR*>(gameObject->GetComponent(L"RMWR"));
	autoPilot->SetMinimumAltitude(minimumAltitude);
	//Engine::Layer* layer = EngineInstance()->SceneManager()->CurrentScene()->FindLayer();

	leader = objectInfomation.leader;
	wingManPositionLocal = objectInfomation.wingmanPosition;
	//enemyLayer;
	return S_OK;
}

HRESULT AIPilot::Start(void)
{

	return S_OK;
}

void AIPilot::Update(void)
{
	if (leader != nullptr && leader->Destroy())
		leader = nullptr;


	static FLOAT chaseDistance = ConvertFeetToWorld(10000.0f) * 2.5f;
	static FLOAT throttleMinimunDistance = ConvertFeetToWorld(2000.0f) * 2.5f;
	timer -= DeltaTime();
	defend = rmwr->ClosedWarning();

	if (gameObject->transform()->Position().y <= minimumAltitude)
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
		target = fcs->LinkTarget();
		if (target != nullptr)
		{
			towardEnemy = target->transform()->Position() - gameObject->transform()->Position();
			direction = towardEnemy.normalize();
			enemyDistance = towardEnemy.magnitude();
			if (leader == nullptr)
				ChaseEnemy();
			else
			{

				if (enemyDistance < chaseDistance)
				{
					ChaseEnemy();

					flightModule->throttle = enemyDistance / throttleMinimunDistance;
					if (flightModule->throttle > 1.0f)
						flightModule->throttle = 1.0f;
					else if(flightModule->throttle < flightModule->IdleThrottle())
						flightModule->ThrottleIdle();
				}
				else
				{
					FormationFlight();
					fcs->gunFire = false;
				}

			}

		}
		else
		{
			fcs->gunFire = false;

			if (leader != nullptr)
				FormationFlight();

			if (target == nullptr)
				SearchEnemy();
		}

	}
}

void AIPilot::FixedUpdate(void)
{
}

void AIPilot::LateUpdate(void)
{
	if (leader != nullptr && leader->Destroy())
		leader = nullptr;


	//if (!target->IsActive())
	//	target = nullptr;
	//
}

void AIPilot::NonCombat(void)
{
	if (leader == nullptr)
		LeadFlight();
	else
		FormationFlight();
}

void AIPilot::LeadFlight(void)
{
	ChaseEnemy();
}

void AIPilot::FormationFlight(void)
{
	if (leaderFlightModule == nullptr)
		leaderFlightModule = static_cast<FlightMovement*>(leader->GetComponent(L"FlightMovement"));

	FLOAT offsetZ = (10.0f * static_cast<FLOAT>(1 + (squadNumber / 2))) * -1.0f;
	FLOAT offsetX = offsetZ * static_cast<FLOAT>(1 - 2 * (squadNumber % 2));

	Vector3 position = Vector3{ offsetX * 0.75f, 0.0f, offsetZ * 0.01f };
	
	DirectX::XMStoreFloat3(&position, DirectX::XMVector3TransformCoord(DirectX::XMLoadFloat3(&position), DirectX::XMLoadFloat4x4(&leader->transform()->WorldMatrix())));

	if (position.y < minimumAltitude)
		position.y = minimumAltitude;

	autoPilot->SetDestination(position);

	FLOAT distanceValue = (position - gameObject->transform()->Position()).magnitude();
	distanceValue /= ConvertFeetToWorld(500.0f) * 5.0f;
	if (distanceValue >= 1.0f)
		distanceValue = 1.0f;

	static const FLOAT& leaderVelocity = leaderFlightModule->Velocity();
	FLOAT targetVelocity = leaderVelocity * (0.9f + distanceValue * distanceValue);
	if (flightModule->Velocity() > targetVelocity)
	{
		flightModule->throttle = 0.0f;
		flightModule->airbreakActive = true;
	}
	else
	{
		flightModule->throttle = targetVelocity / leaderVelocity - 1.0f;
		flightModule->throttle += flightModule->IdleThrottle();
		if (flightModule->throttle > 1.0f)
			flightModule->throttle = 1.0f;
		flightModule->airbreakActive = false;
	}
}


void AIPilot::Evade(void)
{
	bool done;
	if (storage == Vector3::one() * -1.0f)
	{
		storage = gameObject->transform()->Angle();
		storage.y += 90.0f * (static_cast<INT>(::RandomFloat(0.0f, 1.0f)) == 1)? 1.0f : -1.0f;
		if (storage.y > 180.0f)
			storage.y -= 360.0f;
		if (storage.y < -180.0f)
			storage.y += 360.0f;
		autoPilot->SetDestination(Vector3::one() * -1.0f);
	}
	autoPilot->TurnToDirection(storage, done);
	if(done)
	{
		storage = gameObject->transform()->Angle();
		storage.y += 90.0f * (static_cast<INT>(::RandomFloat(0.0f, 1.0f)) == 1) ? 1.0f : -1.0f;
		if (storage.y > 180.0f)
			storage.y -= 360.0f;
		if (storage.y < -180.0f)
			storage.y += 360.0f;
		autoPilot->SetDestination(Vector3::one() * -1.0f);
	}
}

void AIPilot::SearchEnemy(void)
{
	return;
	leader = target = EngineInstance()->SceneManager()->CurrentScene()->FindLayer(L"Ally")->GetGameObject(L"Player");
	fcs->SetForceTargetChange(target);
}

void AIPilot::ChaseEnemy(void)
{
	autoPilot->SetDestination(target->transform()->Position());

	if (fcs->Locked() && timer <= 0.0f)
	{
		fcs->weaponRelease = true;
		timer = 2.5f;
	}

	float directionAmount = DirectX::XMVectorGetX(DirectX::XMVector3Dot(DirectX::XMLoadFloat3(&gameObject->transform()->Forward()), DirectX::XMLoadFloat3(&direction)));

	if (enemyDistance <= ConvertFeetToWorld(4000.0f) && directionAmount > std::cosf(DirectX::XMConvertToRadians(5.0f)))
		fcs->gunFire = true;
	else
		fcs->gunFire = false;
}

void AIPilot::setDestination(Vector3 position)
{
	autoPilot->SetDestination(position);
}
