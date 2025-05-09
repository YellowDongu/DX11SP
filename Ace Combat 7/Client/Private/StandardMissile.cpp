#include "pch.h"
#include "StandardMissile.h"
#include "GameObject.h"
#include "LineDrawer.h"
#include "MissileTrail.h"

StandardMissile::StandardMissile(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Missile(dxDevice, dxDeviceContext), baseRotation{}
{
}

StandardMissile::StandardMissile(const StandardMissile& other) : Missile(other), baseRotation(other.baseRotation)
{
	debugDraw = static_cast<Engine::LineDrawer*>(GetComponent(L"DebugChaser"));
}

void StandardMissile::Free(void)
{
	Engine::GameObject::Free();
}

StandardMissile* StandardMissile::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext)
{
	StandardMissile* newInstance = new StandardMissile(dxDevice, dxDeviceContext);

	if (FAILED(newInstance->Start()))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}
	return newInstance;
}

#define aircraftGlobalMatrix(matrix) DirectX::XMStoreFloat4x4(&matrix, DirectX::XMMatrixAffineTransformation(DirectX::XMVectorSet(0.01f, 0.01f, 0.01f, 0.0f), DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), DirectX::XMQuaternionRotationRollPitchYaw(DirectX::XMConvertToRadians(-90.0f), DirectX::XMConvertToRadians(-90.0f), DirectX::XMConvertToRadians(0.0f)), DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f)))
Engine::GameObject* StandardMissile::Clone(void)
{
	return new StandardMissile(*this);
}

HRESULT StandardMissile::Start(void)
{
	if (FAILED(CreateTransform()))
		return E_FAIL;
	
	transformComponent->Scale() *= 50.0f;

	::LoadStaticModel(L"../Bin/Resources/Vehicles/Weapons/w_msl_a0/w_msl_a0.model", model);
	//model = Engine::StaticModel::Create(dxDevice, dxDeviceContext, "../Bin/Resources/Vehicles/Weapons/w_msl_a0/w_msl_a0.FBX");
	if (model == nullptr)
		return E_FAIL;
	AddComponent(model, L"StaticModel");
	DirectX::XMStoreFloat4(&baseRotation, DirectX::XMQuaternionRotationRollPitchYaw(DirectX::XMConvertToRadians(90.0f), 0.0f, 0.0f));

	debugDraw = Engine::LineDrawer::Create(dxDevice, dxDeviceContext);
	if (debugDraw == nullptr)
		return E_FAIL;
	AddComponent(debugDraw, L"DebugChaser");
	
	rotationSpeed = Vector2::one() * 1.0f;
	speed = ConvertFeetToWorld(4000.0f);
	maximumLifeTime = 30.0f;
	lifeTime = 0.0f;

	return S_OK;
}

void StandardMissile::Update(void)
{
	if (target != nullptr && target->Destroy())
	{
		target = nullptr;
		return;
	}


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

	positionOffset = Vector3{ 2.8f, 4.4f, 2.75f } * 10.0f;

	Chase();
}

void StandardMissile::LateUpdate(void)
{
	if(!detonated)
		AddRenderObject(RenderType::NonBlend, this);


}

void StandardMissile::FixedUpdate(void)
{
	if (detonated)
		return;
}

void StandardMissile::Render(void)
{
	if (detonated)
		return;
	transformComponent->Render();
	model->Render();
	//debugDraw->Render();
}

Missile* StandardMissile::Launch(Engine::GameObject* shooter, Vector3 LaunchOffsetPosition, Engine::GameObject* target)
{
	if (target == nullptr)
		return nullptr;

	StandardMissile* newInstnace = new StandardMissile(*this);
	Engine::Transform& transform = *shooter->transform();
	newInstnace->transform()->Synchronization(transform);
	Vector3 position = shooter->transform()->Position() + LaunchOffsetPosition;
	memcpy(&newInstnace->transformComponent->Position(), &position, sizeof(Vector3));
	newInstnace->transformComponent->UpdateWorldMatrix();
	newInstnace->target = target;
	newInstnace->debugDraw->AddPosition(transform.Position());


	Engine::Layer* layer = EngineInstance()->SceneManager()->CurrentScene()->FindLayer(L"ParticleLayer");
	MissileTrail* trail = static_cast<MissileTrail*>(layer->GetGameObject(L"MissileTrail"));
	if (trail != nullptr)
		trail->AddTrail(newInstnace);

	return newInstnace;
}

void StandardMissile::Chase(void)
{
	static FLOAT detonationDistance = ConvertFeetToWorld(50.0f);
	transformComponent->Translate(transformComponent->Forward() * 0.005f/*DeltaTime()*/ * speed);

	if (target == nullptr)
		return;

	Vector3 direction = target->transform()->Position() - transformComponent->Position();
	FLOAT distance = direction.magnitude();
	if (distance <= detonationDistance)
	{
		Detonate(); 
		return;
	}

	if (priviousDistance + 2.0f < distance)
		target = nullptr;

	priviousDistance = distance;
	Missile::Rotation(direction);
	rotation = transformComponent->Quaternion();
}