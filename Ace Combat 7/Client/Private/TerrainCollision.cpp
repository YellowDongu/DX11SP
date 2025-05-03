#include "pch.h"
#include "TerrainCollision.h"


TerrainCollision::TerrainCollision(ID3D11Device* device, ID3D11DeviceContext* context) : Engine::Component(device, context)
{
}

TerrainCollision::TerrainCollision(const TerrainCollision& other) : Engine::Component(other)
{
}

void TerrainCollision::Free(void)
{

}

TerrainCollision* TerrainCollision::Create(ID3D11Device* device, ID3D11DeviceContext* context)
{
	TerrainCollision* newInstance = new TerrainCollision(device, context);
	//newInstance->SetNamePreset();
	if (FAILED(newInstance->Start()))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}
	return newInstance;
}

Engine::Component* TerrainCollision::Clone(void)
{
	return new TerrainCollision(*this);
}

HRESULT TerrainCollision::Start(void)
{
	navMesh = EngineInstance()->ColliderSystem()->CurrentNavMesh();
	if (navMesh == nullptr)
		return E_FAIL;

	return S_OK;
}

HRESULT TerrainCollision::Awake(void)
{
	nodeIndex = navMesh->GetTriangleIndex(gameObject->transform()->Position());
	currentNode = navMesh->GetNode(nodeIndex);

	return S_OK;
}

void TerrainCollision::LateUpdate(void)
{
	if (nodeIndex == -1)
		return;
	Engine::Transform* myTransform = gameObject->transform();
	Vector3& myPosition = myTransform->Position();
	if (!navMesh->Inside(myPosition, nodeIndex))
	{
		currentNode = navMesh->GetNode(nodeIndex);
		if (nodeIndex == -1)
			return;
	}
	static FLOAT minimumAltitude = ConvertFeetToWorld(2000.0f);
	if (myPosition.y >= minimumAltitude)
		return;

	FLOAT currentAltitude;// = gameObject->transform()->Position().y;
	Vector3 reflectedDirection;
	currentAltitude = DirectX::XMVectorGetY(DirectX::XMVector3TransformCoord(DirectX::XMLoadFloat3(&frontLocalPosition), DirectX::XMLoadFloat4x4(&myTransform->WorldMatrix())));
	if (currentAltitude > myPosition.y)
		currentAltitude = myPosition.y;
	
	if (currentNode->getHeight(myPosition) >= currentAltitude)
	{
		DirectX::XMStoreFloat3(&reflectedDirection, currentNode->ReflectionDirection(myTransform->Forward()));
		reflectedDirection = reflectedDirection.getDirection();
		reflectedDirection.z = myTransform->Angle().z;

		myTransform->SetAngle(reflectedDirection);
	}


}

void TerrainCollision::FixedUpdate(void)
{
	if (nodeIndex == -1)
	{
		nodeIndex = navMesh->GetTriangleIndex(gameObject->transform()->Position());
		currentNode = navMesh->GetNode(nodeIndex);
	}

}
