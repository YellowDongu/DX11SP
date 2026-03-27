#include "pch.h"
#include "SimpleBullet.h"
#include "RMWR.h"
Engine::Shader* SimpleBullet::shader = nullptr;

SimpleBullet::SimpleBullet(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Engine::GameObject(dxDevice, dxDeviceContext)
{
}

SimpleBullet::SimpleBullet(const SimpleBullet& other) : Engine::GameObject(other)
{
	model = static_cast<Engine::CubePolygon*>(GetComponent(L"CubePolygon"));
	collider = static_cast<Engine::Collider*>(GetComponent(L"Collider"));
}

void SimpleBullet::Free(void)
{
	Engine::GameObject::Free();
}

SimpleBullet* SimpleBullet::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext)
{
	SimpleBullet* newInsatnce = new SimpleBullet(dxDevice, dxDeviceContext);
	if (FAILED(newInsatnce->Start()))
	{
		Base::Destroy(newInsatnce);
	return nullptr;
	}
	return newInsatnce;
}

Engine::GameObject* SimpleBullet::Clone(void)
{
	return new SimpleBullet(*this);
}

HRESULT SimpleBullet::Start(void)
{
	if (FAILED(CreateTransform()))
		return E_FAIL;
	std::vector<CubeVertex> vertices;
	std::vector<UINT> indices;
	CreateCustomVertex(vertices, indices);
	model = Engine::CubePolygon::Create(dxDevice, dxDeviceContext, vertices, indices);
	if (model == nullptr)
		return E_FAIL;
	AddComponent(model, L"CubePolygon");
	transformComponent->Scale() = Vector3{ 0.01f, 0.01f, 2.5f };

	Engine::OBB::Description description;
	description.center = Vector3::zero();
	description.extents = Vector3::one();
	description.quaternion = transformComponent->Quaternion();

	collider = Engine::Collider::Create(dxDevice, dxDeviceContext, &description);
	AddComponent(collider, L"Collider");
	return S_OK;
}

void SimpleBullet::Update(void)
{
	AddCollider(collider);
	lifeTime += DeltaTime();
	if (lifeTime >= maxTime)
	{
		destroy = true;
		return;
	}

	transformComponent->Position() += transformComponent->Forward() * DeltaTime() * bulletSpeed;
	transformComponent->UpdateWorldMatrix();
	collider->Update();
}

void SimpleBullet::LateUpdate(void)
{
	collider->LateUpdate();
	AddRenderObject(RenderType::NonBlendNonLight, this);
}

void SimpleBullet::FixedUpdate(void)
{
}

void SimpleBullet::Render(void)
{
	if (SimpleBullet::shader == nullptr)
		SimpleBullet::shader = GetShader(L"DefaultCubeShader.hlsl");

	Engine::Shader* currentShader = GetCurrentShader();

	SetShader(SimpleBullet::shader->shaderFile);
	SimpleBullet::shader->PassNumber(2);

	transformComponent->Render();
	model->Render();

	SetShader(currentShader->shaderFile);
	currentShader->PassNumber(0);

	//collider->Render();
}

SimpleBullet* SimpleBullet::Shoot(Engine::GameObject* shooter, Vector3 offsetPosition, float4 quaternion)
{
	SimpleBullet* newInstance = new SimpleBullet(*this);
	newInstance->transform()->Position() = offsetPosition;
	newInstance->transform()->SetAngle(quaternion);
	newInstance->collider->SetException(shooter);
	return newInstance;
}

void SimpleBullet::Collide(Engine::GameObject* target, Engine::Collider* targetCollision)
{
	RMWR* rmwr = static_cast<RMWR*>(target->GetComponent(L"RMWR"));
	if (rmwr != nullptr)
	{
		rmwr->HitMe(damage);
	}
}

void SimpleBullet::CreateCustomVertex(std::vector<CubeVertex>& vertices, std::vector<UINT>& indices)
{
	vertices.clear();
	vertices.resize(8);


	vertices[0].position = { -0.5f, 0.5f,-0.5f };
	vertices[1].position = { 0.5f, 0.5f,-0.5f };
	vertices[2].position = { 0.5f,-0.5f,-0.5f };
	vertices[3].position = { -0.5f,-0.5f,-0.5f };
	vertices[4].position = { -0.5f, 0.5f, 0.5f };
	vertices[5].position = { 0.5f, 0.5f, 0.5f };
	vertices[6].position = { 0.5f,-0.5f, 0.5f };
	vertices[7].position = { -0.5f,-0.5f, 0.5f };

	DirectX::XMFLOAT3 color = { 1.0f, 1.0f, 0.0f };
	for (size_t i = 0; i < 8; i++)
	{
		memcpy(&vertices[i].texturePosition, &color, sizeof(DirectX::XMFLOAT3));
	}

	indices.clear();
	indices.resize(36);
	/* +X */
	indices[0] = 1; indices[1] = 5; indices[2] = 6;
	indices[3] = 1; indices[4] = 6; indices[5] = 2;

	/* -X */
	indices[6] = 4; indices[7] = 0; indices[8] = 3;
	indices[9] = 4; indices[10] = 3; indices[11] = 7;

	/* +Y */
	indices[12] = 4; indices[13] = 5; indices[14] = 1;
	indices[15] = 4; indices[16] = 1; indices[17] = 0;

	/* -Y */
	indices[18] = 3; indices[19] = 2; indices[20] = 6;
	indices[21] = 3; indices[22] = 6; indices[23] = 7;

	/* +Z */
	indices[24] = 5; indices[25] = 4; indices[26] = 7;
	indices[27] = 5; indices[28] = 7; indices[29] = 6;

	/* -Z */
	indices[30] = 0; indices[31] = 1; indices[32] = 2;
	indices[33] = 0; indices[34] = 2; indices[35] = 3;
}
