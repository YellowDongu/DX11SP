#include "pch.h"
#include "Cloud.h"

Cloud::Cloud(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Engine::GameObject(dxDevice, dxDeviceContext)
{
}

Cloud::Cloud(const Cloud& other) : Engine::GameObject(other)
{
	particle = static_cast<CloudEffect*>(GetComponent(L"CloudEffect"));
}

void Cloud::Free(void)
{
	Engine::GameObject::Free();
}

Cloud* Cloud::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, Vector3 position, Vector3 cloudSize)
{
	Cloud* newInstance = new Cloud(dxDevice, dxDeviceContext);
	newInstance->centerPosition = position;
	newInstance->cloudSize = cloudSize;
	if (FAILED(newInstance->Start()))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}
	return newInstance;
}

Engine::GameObject* Cloud::Clone(void)
{
	return new Cloud(*this);
}

HRESULT Cloud::Start(void)
{
	//if (FAILED(CreateTransform()))
	//	return E_FAIL;

	particle = CloudEffect::Create(dxDevice, dxDeviceContext);
	if (particle == nullptr)
		return E_FAIL;
	AddComponent(particle, L"CloudEffect");
	

	return S_OK;
}

HRESULT Cloud::Awake(void)
{
	particle->Awake();
	UINT length = particle->InstanceNumber();
	distributionMatrix.resize(length);
	//xmMatrix worldMatrix = DirectX::XMMatrixIdentity();
	xmMatrix worldMatrix = DirectX::XMMatrixScaling(50.0f, 50.0f, 50.0f);
	FLOAT x, y, z;

	for (UINT i = 0; i < length; i++)
	{
		x = RandomFloat(centerPosition.x - cloudSize.x, centerPosition.x + cloudSize.x);
		y = RandomFloat(centerPosition.y - cloudSize.y, centerPosition.y + cloudSize.y);
		z = RandomFloat(centerPosition.z - cloudSize.z, centerPosition.z + cloudSize.z);

		DirectX::XMStoreFloat4(&distributionMatrix[i].right, worldMatrix.r[0]);
		DirectX::XMStoreFloat4(&distributionMatrix[i].up, worldMatrix.r[1]);
		DirectX::XMStoreFloat4(&distributionMatrix[i].look, worldMatrix.r[2]);
		DirectX::XMStoreFloat4(&distributionMatrix[i].position, DirectX::XMVectorSet(x, y, z, 1.0f));
	}

	particle->LinkMatrixList(distributionMatrix);

	return S_OK;
}

void Cloud::Update(void)
{
	Engine::GameObject::Update();
}
//
//// XMFLOAT4X4 매트릭스에서 위치를 추출하여 거리 계산
//float CalculateDistance(const DirectX::XMFLOAT4X4& worldMatrix, const DirectX::XMFLOAT3& targetPosition)
//{
//	// worldMatrix의 m41, m42, m43는 위치 값 (m14는 변환)
//	DirectX::XMFLOAT3 position(worldMatrix.m41, worldMatrix.m42, worldMatrix.m43);
//	// 두 위치 간의 유클리드 거리 계산
//	float dx = position.x - targetPosition.x;
//	float dy = position.y - targetPosition.y;
//	float dz = position.z - targetPosition.z;
//	return std::sqrt(dx * dx + dy * dy + dz * dz);
//}
//
//// XMFLOAT4X4 매트릭스에서 위치를 추출하여 거리 계산
//float CalculateDistance(const Engine::VertexMatrix& worldMatrix, const Vector3& targetPosition)
//{
//	return (Vector3(worldMatrix.position.x, worldMatrix.position.y, worldMatrix.position.z) - targetPosition).magnitude();
//}
//// 정렬 함수
//void SortMatricesByDistance(std::vector<DirectX::XMFLOAT4X4>& matrices, const DirectX::XMFLOAT3& targetPosition)
//{
//	// 정렬 기준: 위치와 타겟 위치의 거리
//	std::sort(matrices.begin(), matrices.end(),
//			  [&targetPosition](const DirectX::XMFLOAT4X4& a, const DirectX::XMFLOAT4X4& b)
//			  {
//				  float distanceA = CalculateDistance(a, targetPosition);
//				  float distanceB = CalculateDistance(b, targetPosition);
//				  return distanceA < distanceB; // 가까운 순으로 정렬
//			  });
//}
//// 정렬 함수
//void SortMatricesByDistance(std::vector<Engine::VertexMatrix>& matrices, const Vector3& targetPosition)
//{
//	// 정렬 기준: 위치와 타겟 위치의 거리
//	std::sort(matrices.begin(), matrices.end(),
//			  [&targetPosition](const Engine::VertexMatrix& a, const Engine::VertexMatrix& b)
//			  {
//				  float distanceA = CalculateDistance(a, targetPosition);
//				  float distanceB = CalculateDistance(b, targetPosition);
//				  return distanceA < distanceB; // 가까운 순으로 정렬
//			  });
//	return;
//	// 정렬 기준: 위치와 타겟 위치의 거리
//	std::sort(matrices.begin(), matrices.end(),
//			  [&targetPosition](const Engine::VertexMatrix& one, const Engine::VertexMatrix& other)
//			  {
//				  float distanceA = CalculateDistance(one, targetPosition);
//				  float distanceB = CalculateDistance(other, targetPosition);
//
//				  return (Vector3(one.position.x, one.position.y, one.position.z) - targetPosition).magnitude() < (Vector3(other.position.x, other.position.y, other.position.z) - targetPosition).magnitude();
//				  return distanceA < distanceB; // 가까운 순으로 정렬
//			  });
//}
//

void Cloud::LateUpdate(void)
{

	Engine::GameObject::LateUpdate();
	AddRenderObject(RenderType::Effect, this);

}

void Cloud::FixedUpdate(void)
{
	Engine::GameObject::FixedUpdate();
}

void Cloud::Render(void)
{

	static Vector3& cameraPosition = EngineInstance()->RenderManager()->CurrentPipeLineStatus().currnetCamera->GetOwner()->transform()->Position();
	std::sort(distributionMatrix.begin(), distributionMatrix.end(), [&](const Engine::VertexMatrix& one, const Engine::VertexMatrix& other) { return (Vector3(one.position.x, one.position.y, one.position.z) - cameraPosition).magnitude() > (Vector3(other.position.x, other.position.y, other.position.z) - cameraPosition).magnitude(); });
	particle->Render();
}
