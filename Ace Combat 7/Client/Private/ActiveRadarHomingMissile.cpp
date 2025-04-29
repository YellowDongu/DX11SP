#include "pch.h"
#include "ActiveRadarHomingMissile.h"
#include "FireControlSystem.h"
#include "LineDrawer.h"

ActiveRadarHomingMissile::ActiveRadarHomingMissile(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Missile(dxDevice, dxDeviceContext)
{
}

ActiveRadarHomingMissile::ActiveRadarHomingMissile(const ActiveRadarHomingMissile& other) : Missile(other)
{
	debugDraw = static_cast<Engine::LineDrawer*>(GetComponent(L"DebugChaser"));
}

void ActiveRadarHomingMissile::Free(void)
{
	Engine::GameObject::Free();
}

ActiveRadarHomingMissile* ActiveRadarHomingMissile::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext)
{
	ActiveRadarHomingMissile* newInstance = new ActiveRadarHomingMissile(dxDevice, dxDeviceContext);

	if (FAILED(newInstance->Start()))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}
	return newInstance;
}

Engine::GameObject* ActiveRadarHomingMissile::Clone(void)
{
	return new ARHM(*this);
}

Missile* ActiveRadarHomingMissile::Launch(Engine::GameObject* shooter, Vector3 LaunchOffsetPosition, Engine::GameObject* target)
{
	if (target == nullptr)
		return nullptr;

	ARHM* newInstnace = new ARHM(*this);
	Engine::Transform& transform = *shooter->transform();
	newInstnace->transform()->Synchronization(transform);
	Vector3 position = transform.Position() + LaunchOffsetPosition;
	memcpy(&newInstnace->transformComponent->Position(), &position, sizeof(Vector3));
	newInstnace->transformComponent->UpdateWorldMatrix();
	newInstnace->target = target;
	newInstnace->debugDraw->AddPosition(transform.Position());
	newInstnace->fcs = static_cast<FCS*>(shooter->GetComponent(L"FCS"));

	return newInstnace;
}

HRESULT ActiveRadarHomingMissile::Start(void)
{
	if (FAILED(CreateTransform()))
		return E_FAIL;

	transformComponent->Scale() *= 50.0f;

	//::LoadStaticModel(L"../Bin/Resources/Vehicles/Weapons/w_msl_a0/w_msl_a0.model", model);
	model = Engine::StaticModel::Create(dxDevice, dxDeviceContext, "../Bin/Resources/Vehicles/Weapons/w_msl_a0/w_msl_a0.FBX");
	if (model == nullptr)
		return E_FAIL;
	AddComponent(model, L"StaticModel");
	DirectX::XMStoreFloat4(&baseRotation, DirectX::XMQuaternionRotationRollPitchYaw(DirectX::XMConvertToRadians(90.0f), 0.0f, 0.0f));

	debugDraw = Engine::LineDrawer::Create(dxDevice, dxDeviceContext);
	if (debugDraw == nullptr)
		return E_FAIL;
	AddComponent(debugDraw, L"DebugChaser");

	rotationSpeed = Vector2::one() * 1.5f;
	speed = ConvertFeetToWorld(5000.0f);
	maximumLifeTime = 35.0f;
	lifeTime = 0.0f;
	maximumLockDistance = ConvertFeetToWorld(9000.0f) * 5.0f;
	maximumLockDirection = std::cosf(DirectX::XMConvertToRadians(25.0f));
	missileCount = 50;
	return S_OK;
}

void ActiveRadarHomingMissile::Update(void)
{
	lifeTime += DeltaTime();
	if (lifeTime >= maximumLifeTime)
	{
		detonated = true;
		/*delete this;*/ return;
	}

	timer += DeltaTime();
	if (timer >= 0.005f)
	{
		timer -= 0.005f;

		debugDraw->AddPosition(transformComponent->Position());
		if (debugDraw->Size() >= 100)
			debugDraw->PopFrontPosition();
	}

	Chase();
}

void ActiveRadarHomingMissile::LateUpdate(void)
{
	if (detonated)
		return;

	priviousPosition = currentPosition;
	currentPosition = target->transform()->Position();


	AddRenderObject(RenderType::NonBlend, this);
}

void ActiveRadarHomingMissile::FixedUpdate(void)
{
	if (detonated)
		return;
}

void ActiveRadarHomingMissile::Render(void)
{
	transformComponent->Render();
	model->Render();
	debugDraw->Render();
}


void ActiveRadarHomingMissile::Chase(void)
{
	if (fcs->Locked() && fcs->LinkTarget() == target)
	{
		Vector3 direction = target->transform()->Position() - transformComponent->Position();
		float distance = direction.magnitude();
		if (distance <= ConvertFeetToWorld(50.0f))
		{
			Detonate();
			return;
		}

		predictedPosition = currentPosition + (currentPosition - priviousPosition) * (distance / (speed * 0.5f));
		direction = (predictedPosition - transformComponent->Position()).normalize();

		Missile::Rotation(direction);
	}

	transformComponent->Translate(transformComponent->Forward() * 0.005f/*DeltaTime()*/ * speed);
	rotation = transformComponent->Quaternion();
}
