#include "Foundation.h"
#include "Collider.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Transform.h"

using namespace Engine;

Collider::Collider(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Component(dxDevice, dxDeviceContext),
device(dxDevice),
context(dxDeviceContext)
{
	device->AddRef();
	context->AddRef();
}

Engine::Collider::Collider(const Collider& other) : Component(other), device(other.device), context(other.context), clone(true),
#ifdef ColliderDebug
batch(other.batch), effect(other.effect), inputLayout(other.inputLayout)
#endif
{
	device->AddRef();
	context->AddRef();
	inputLayout->AddRef();
	if (other.collidingBody != nullptr)
		collidingBody = other.collidingBody->Clone();
}

void Collider::Free(void)
{

	if (collidingBody != nullptr)
	{
		delete collidingBody;
		collidingBody = nullptr;
	}

	#ifdef ColliderDebug
	if (!clone)
	{
		if (batch != nullptr)
		{
			delete batch;
			batch = nullptr;
		}
		if (effect != nullptr)
		{
			delete effect;
			effect = nullptr;
		}
	}
	inputLayout->Release();
	inputLayout = nullptr;
	#endif

	context->Release();
	device->Release();
}

Collider* Collider::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, Bounder::Description* description)
{
	Collider* newInstance = new Collider(dxDevice, dxDeviceContext);
	if (FAILED(newInstance->CreateCollider(description)))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}
	return newInstance;
}

Component* Collider::Clone(void)
{
	return new Collider(*this);
}

HRESULT Collider::Start(void)
{
	return E_NOTIMPL;
}

void Collider::Update(void)
{
}

void Collider::LateUpdate(void)
{
	collidingBody->Update(gameObject->transform()->WorldMatrix());
}

void Collider::FixedUpdate(void)
{
}

void Collider::Render(void)
{
	return;
	#ifdef ColliderDebug
	effect->SetWorld(DirectX::XMMatrixIdentity());
	const PipelineStatus& pipeline = ::GetPipeLineStatus();
	effect->SetView(DirectX::XMLoadFloat4x4(&pipeline.viewMatrix));
	effect->SetProjection(DirectX::XMLoadFloat4x4(&pipeline.projectionMatrix));
	context->IASetInputLayout(inputLayout);

	effect->Apply(context);

	collidingBody->DebugRender(batch, { 1.f, 0.f, 0.f, 1.f });
	#endif
	updated = false;
}

HRESULT Collider::CreateCollider(Bounder::Description* description)
{
	if (description == nullptr)
		return E_FAIL;

	#ifdef ColliderDebug
	batch = new DirectX::PrimitiveBatch<DirectX::VertexPositionColor>(context);
	effect = new DirectX::BasicEffect(device);

	const void* pShaderByteCode = { nullptr };
	size_t	iShaderCodeLength = {};

	effect->SetVertexColorEnabled(true);
	effect->GetVertexShaderBytecode(&pShaderByteCode, &iShaderCodeLength);

	if (FAILED(device->CreateInputLayout(DirectX::VertexPositionColor::InputElements, DirectX::VertexPositionColor::InputElementCount, pShaderByteCode, iShaderCodeLength, &inputLayout)))
		return E_FAIL;
	#endif

	switch (description->colliderType)
	{
	case ColliderType::AABB:
		collidingBody = AABB::Create(*static_cast<AABB::Description*>(description));
		break;
	case ColliderType::OBB:
		collidingBody = OBB::Create(*static_cast<OBB::Description*>(description));
		break;
	case ColliderType::Sphere:
		collidingBody = Sphere::Create(*static_cast<Sphere::Description*>(description));
		break;
	case ColliderType::Frustum:
		collidingBody = Frustum::Create(*static_cast<Frustum::Description*>(description));
		break;
	default:
		collidingBody = nullptr;
		break;
	}

	if (collidingBody == nullptr)
		return E_FAIL;
	return S_OK;
}

bool Collider::Intersect(Collider* other)
{
	for (auto exceptObject : exception)
	{
		if (other->gameObject == exceptObject)
			return false;
	}

	Transform* transform = gameObject->transform();
	Vector3 direction;
	float amount;
	float minDistance = other->collidingBody->GetInterectRadius() + collidingBody->GetInterectRadius();
	float distance = (other->collidingBody->GetWorldCenter() - collidingBody->GetWorldCenter()).magnitude();

	if (minDistance <= distance)
		return false;

	return collidingBody->Intersect(other->collidingBody, direction, amount);
}


bool AABB::Intersect(Bounder* other, Vector3& direction, float& amount)
{
	bool intersected = false;

	switch (other->colliderType)
	{
	case Engine::ColliderType::AABB:
		intersected = bounder.Intersects(static_cast<AABB*>(other)->bounder);
		break;
	case Engine::ColliderType::OBB:
		intersected = bounder.Intersects(static_cast<OBB*>(other)->bounder);
		break;
	case Engine::ColliderType::Sphere:
		intersected = bounder.Intersects(static_cast<Sphere*>(other)->bounder);
		break;
	case Engine::ColliderType::Frustum:
		intersected = bounder.Intersects(static_cast<Frustum*>(other)->bounder);
		break;
	default:
		intersected = false;
		break;
	}

	direction = Vector3::zero();
	amount = 0.0f;

	return intersected;
}


bool OBB::Intersect(Bounder* other, Vector3& direction, float& amount)
{
	bool intersected = false;

	switch (other->colliderType)
	{
	case Engine::ColliderType::AABB:
		intersected = bounder.Intersects(static_cast<AABB*>(other)->bounder);
		break;
	case Engine::ColliderType::OBB:
		intersected = bounder.Intersects(static_cast<OBB*>(other)->bounder);
		break;
	case Engine::ColliderType::Sphere:
		intersected = bounder.Intersects(static_cast<Sphere*>(other)->bounder);
		break;
	case Engine::ColliderType::Frustum:
		intersected = bounder.Intersects(static_cast<Frustum*>(other)->bounder);
		break;
	default:
		intersected = false;
		break;
	}

	direction = Vector3::zero();
	amount = 0.0f;

	return intersected;
}

bool Sphere::Intersect(Bounder* other, Vector3& direction, float& amount)
{
	bool intersected = false;

	switch (other->colliderType)
	{
	case Engine::ColliderType::AABB:
		intersected = bounder.Intersects(static_cast<AABB*>(other)->bounder);
		break;
	case Engine::ColliderType::OBB:
		intersected = bounder.Intersects(static_cast<OBB*>(other)->bounder);
		break;
	case Engine::ColliderType::Sphere:
		intersected = bounder.Intersects(static_cast<Sphere*>(other)->bounder);
		break;
	case Engine::ColliderType::Frustum:
		intersected = bounder.Intersects(static_cast<Frustum*>(other)->bounder);
		break;
	default:
		intersected = false;
		break;
	}

	direction = Vector3::zero();
	amount = 0.0f;

	return intersected;
}
