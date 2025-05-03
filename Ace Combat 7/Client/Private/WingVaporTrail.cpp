#include "pch.h"
#include "WingVaporTrail.h"

#include "AIPilot.h"
#include "PlayerPilot.h"

WingVaporTrail::WingVaporTrail(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Engine::GameObject(dxDevice, dxDeviceContext)
{
}

WingVaporTrail::WingVaporTrail(const WingVaporTrail& other) : Engine::GameObject(other)
{
}

void WingVaporTrail::Free(void)
{
	GameObject::Free();
}

WingVaporTrail* WingVaporTrail::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext)
{
	WingVaporTrail* newInstance = new WingVaporTrail(dxDevice, dxDeviceContext);
	if (FAILED(newInstance->Start()))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}
	return newInstance;
}

Engine::GameObject* WingVaporTrail::Clone(void)
{
	return new WingVaporTrail(*this);
}

HRESULT WingVaporTrail::Start(void)
{
	particle = WingTrailParticle::Create(dxDevice, dxDeviceContext);
	if (particle == nullptr)
		return E_FAIL;

	AddComponent(particle, L"WingTrailParticle");
	return S_OK;
}

HRESULT WingVaporTrail::Awake(void)
{
	return S_OK;
}

void WingVaporTrail::Update(void)
{
}

void WingVaporTrail::LateUpdate(void)
{
	AddRenderObject(RenderType::Blend, this);
	Timer += DeltaTime();
	if (Timer < RecordTime)
		return;
	
	Timer -= RecordTime;

	if (points.size() != trailPositions.size())
	{
		points.resize(trailPositions.size());

		for (auto& point : points)
		{
			point.first.resize(MaxListSize);
			point.second.resize(MaxListSize);
		}
	}
	size_t listSize = 0;
	std::list<Matrix>::iterator queueIterator;
	for (size_t i = 0; i < trailPositions.size(); i++)
	{
		trailPositions[i].second.push_back(trailPositions[i].first.gameObjectPointer->transform()->WorldMatrix());
		if (trailPositions[i].second.size() > MaxListSize)
			trailPositions[i].second.pop_front();


		trailPositions[i].first.trailWidthSize = 0.1f;
		UpdatePoints(trailPositions[i].second, points[i].first, trailPositions[i].first.leftOffset, points[i].second, trailPositions[i].first.rightOffset, trailPositions[i].first.trailWidthSize);
		//UpdatePoints(trailPositions[i].second, points[i].first, trailPositions[i].first.leftOffset, trailPositions[i].first.trailWidthSize);
		//UpdatePoints(trailPositions[i].second, points[i].first, trailPositions[i].first.rightOffset, trailPositions[i].first.trailWidthSize);
	}
}

void WingVaporTrail::FixedUpdate(void)
{
}

void WingVaporTrail::Render(void)
{
	for (auto& pointList : points)
	{
		particle->LinkTargetPoints(&pointList.first);
		particle->Render();
		particle->LinkTargetPoints(&pointList.second);
		particle->Render();
	}
}

void WingVaporTrail::UpdatePoints(std::list<Matrix>& matrixQueue, std::vector<Engine::VertexMatrix>& points, Vector3& offset, FLOAT& trailWidth)
{

	if (matrixQueue.empty())
	{
		for (auto& matrix : points)
		{
			ZeroMemory(&matrix, sizeof(Engine::VertexMatrix));
		}
		return;
	}
	std::list<Matrix>::iterator matrixQueueIterator = matrixQueue.begin(), matrixQueueIteratorSecond = matrixQueue.begin();
	matrixQueueIteratorSecond++;
	xmMatrix matrix, nextMatrix;
	//xmVector right = DirectX::XMVectorSet(0.5f, 0.0f, 0.0f, 1.0f), left = DirectX::XMVectorSet(-0.5f, 0.0f, 0.0f, 1.0f);
	Vector3 rightOffset = Vector3{ 1.0f, 0.0f, 0.0f } *(trailWidth * 0.5f) + offset, leftOffset = Vector3{ -1.0f, 0.0f, 0.0f } *(trailWidth * 0.5f) + offset;
	xmVector right = DirectX::XMVectorSet(rightOffset.x, rightOffset.y, rightOffset.z, 1.0f), left = DirectX::XMVectorSet(leftOffset.x, leftOffset.y, leftOffset.z, 1.0f);

	nextMatrix = DirectX::XMLoadFloat4x4(&*matrixQueueIterator);
	nextMatrix.r[0] = DirectX::XMVector3Normalize(nextMatrix.r[0]);
	nextMatrix.r[1] = DirectX::XMVector3Normalize(nextMatrix.r[1]);
	nextMatrix.r[2] = DirectX::XMVector3Normalize(nextMatrix.r[2]);

	for (size_t i = 0; i < points.size(); i++)
	{
		if (matrixQueueIteratorSecond == matrixQueue.end())
		{
			if (i == 99 && matrixQueueIterator != matrixQueue.end())
			{
				matrix = DirectX::XMLoadFloat4x4(&*matrixQueueIterator);
				matrix.r[0] = DirectX::XMVector3Normalize(matrix.r[0]);
				matrix.r[1] = DirectX::XMVector3Normalize(matrix.r[1]);
				matrix.r[2] = DirectX::XMVector3Normalize(matrix.r[2]);
				DirectX::XMStoreFloat4(&points[i].right, DirectX::XMVector3TransformCoord(right, matrix));
				DirectX::XMStoreFloat4(&points[i].up, DirectX::XMVector3TransformCoord(left, matrix));
				DirectX::XMStoreFloat4(&points[i].look, DirectX::XMVector3TransformCoord(left, matrix));
				DirectX::XMStoreFloat4(&points[i].position, DirectX::XMVector3TransformCoord(right, matrix));
			}
			else
				ZeroMemory(&points[i], sizeof(Engine::VertexMatrix));
		}
		else
		{
			matrix = nextMatrix;
			nextMatrix = DirectX::XMLoadFloat4x4(&*matrixQueueIteratorSecond);
			nextMatrix.r[0] = DirectX::XMVector3Normalize(nextMatrix.r[0]);
			nextMatrix.r[1] = DirectX::XMVector3Normalize(nextMatrix.r[1]);
			nextMatrix.r[2] = DirectX::XMVector3Normalize(nextMatrix.r[2]);
			DirectX::XMStoreFloat4(&points[i].right, DirectX::XMVector3TransformCoord(right, matrix));
			DirectX::XMStoreFloat4(&points[i].up, DirectX::XMVector3TransformCoord(left, matrix));
			DirectX::XMStoreFloat4(&points[i].look, DirectX::XMVector3TransformCoord(left, nextMatrix));
			DirectX::XMStoreFloat4(&points[i].position, DirectX::XMVector3TransformCoord(right, nextMatrix));

			matrixQueueIterator++;
			matrixQueueIteratorSecond++;
		}
	}


	return;

	//for save
	/*
	if (matrixQueue.empty())
	{
		for (auto& matrix : points)
		{
			ZeroMemory(&matrix, sizeof(Engine::VertexMatrix));
		}
		return;
	}
	std::list<Matrix>::iterator matrixQueueIterator = matrixQueue.begin(), matrixQueueIteratorSecond = matrixQueue.begin();
	matrixQueueIteratorSecond++;
	xmMatrix matrix, nextMatrix;
	//xmVector right = DirectX::XMVectorSet(0.5f, 0.0f, 0.0f, 1.0f), left = DirectX::XMVectorSet(-0.5f, 0.0f, 0.0f, 1.0f);
	Vector3 rightOffset = Vector3{1.0f, 0.0f, 0.0f} * (trailWidth * 0.5f) + offset, leftOffset = Vector3{-1.0f, 0.0f, 0.0f} * (trailWidth * 0.5f) + offset;
	xmVector right = DirectX::XMVectorSet(rightOffset.x, rightOffset.y, rightOffset.z, 1.0f), left = DirectX::XMVectorSet(leftOffset.x, leftOffset.y, leftOffset.z, 1.0f);

	nextMatrix = DirectX::XMLoadFloat4x4(&*matrixQueueIterator);
	nextMatrix.r[0] = DirectX::XMVector3Normalize(nextMatrix.r[0]);
	nextMatrix.r[1] = DirectX::XMVector3Normalize(nextMatrix.r[1]);
	nextMatrix.r[2] = DirectX::XMVector3Normalize(nextMatrix.r[2]);

	for (size_t i = 0; i < points.size(); i++)
	{
		if (matrixQueueIteratorSecond == matrixQueue.end())
		{
			if (i == 99 && matrixQueueIterator != matrixQueue.end())
			{
				matrix = DirectX::XMLoadFloat4x4(&*matrixQueueIterator);
				matrix.r[0] = DirectX::XMVector3Normalize(matrix.r[0]);
				matrix.r[1] = DirectX::XMVector3Normalize(matrix.r[1]);
				matrix.r[2] = DirectX::XMVector3Normalize(matrix.r[2]);
				DirectX::XMStoreFloat4(&points[i].right, DirectX::XMVector3TransformCoord(right, matrix));
				DirectX::XMStoreFloat4(&points[i].up, DirectX::XMVector3TransformCoord(left, matrix));
				DirectX::XMStoreFloat4(&points[i].look, DirectX::XMVector3TransformCoord(left, matrix));
				DirectX::XMStoreFloat4(&points[i].position, DirectX::XMVector3TransformCoord(right, matrix));
			}
			else
				ZeroMemory(&points[i], sizeof(Engine::VertexMatrix));
		}
		else
		{
			matrix = nextMatrix;
			nextMatrix = DirectX::XMLoadFloat4x4(&*matrixQueueIteratorSecond);
			nextMatrix.r[0] = DirectX::XMVector3Normalize(nextMatrix.r[0]);
			nextMatrix.r[1] = DirectX::XMVector3Normalize(nextMatrix.r[1]);
			nextMatrix.r[2] = DirectX::XMVector3Normalize(nextMatrix.r[2]);
			DirectX::XMStoreFloat4(&points[i].right, DirectX::XMVector3TransformCoord(right, matrix));
			DirectX::XMStoreFloat4(&points[i].up, DirectX::XMVector3TransformCoord(left, matrix));
			DirectX::XMStoreFloat4(&points[i].look, DirectX::XMVector3TransformCoord(left, nextMatrix));
			DirectX::XMStoreFloat4(&points[i].position, DirectX::XMVector3TransformCoord(right, nextMatrix));

			matrixQueueIterator++;
			matrixQueueIteratorSecond++;
		}
	}
	*/
}

void WingVaporTrail::UpdatePoints(std::list<Matrix>& matrixQueue, std::vector<Engine::VertexMatrix>& leftpoints, Vector3& leftOffset, std::vector<Engine::VertexMatrix>& rightPoints, Vector3& rightOffset, FLOAT& trailWidth)
{
	if (matrixQueue.empty())
	{
		for (auto& matrix : leftpoints)
		{
			ZeroMemory(&matrix, sizeof(Engine::VertexMatrix));
		}
		for (auto& matrix : rightPoints)
		{
			ZeroMemory(&matrix, sizeof(Engine::VertexMatrix));
		}
		return;
	}
	std::list<Matrix>::iterator matrixQueueIterator = matrixQueue.begin(), matrixQueueIteratorSecond = matrixQueue.begin();
	matrixQueueIteratorSecond++;
	xmMatrix matrix, nextMatrix;
	//xmVector right = DirectX::XMVectorSet(0.5f, 0.0f, 0.0f, 1.0f), left = DirectX::XMVectorSet(-0.5f, 0.0f, 0.0f, 1.0f);
	Vector3 leftWingRightOffset = Vector3{ 1.0f, 0.0f, 0.0f } *(trailWidth * 0.5f) + leftOffset, leftWingLeftOffset = Vector3{ -1.0f, 0.0f, 0.0f } *(trailWidth * 0.5f) + leftOffset;
	Vector3 rightWingRightOffset = Vector3{ 1.0f, 0.0f, 0.0f } *(trailWidth * 0.5f) + rightOffset, rightWingLeftOffset = Vector3{ -1.0f, 0.0f, 0.0f } *(trailWidth * 0.5f) + rightOffset;

	xmVector leftWingRight = DirectX::XMVectorSet(leftWingRightOffset.x, leftWingRightOffset.y, leftWingRightOffset.z, 1.0f), leftWingLeft = DirectX::XMVectorSet(leftWingLeftOffset.x, leftWingLeftOffset.y, leftWingLeftOffset.z, 1.0f);
	xmVector rightWingRight = DirectX::XMVectorSet(rightWingRightOffset.x, rightWingRightOffset.y, rightWingRightOffset.z, 1.0f), rightWingLeft = DirectX::XMVectorSet(rightWingLeftOffset.x, rightWingLeftOffset.y, rightWingLeftOffset.z, 1.0f);

	nextMatrix = DirectX::XMLoadFloat4x4(&*matrixQueueIterator);
	nextMatrix.r[0] = DirectX::XMVector3Normalize(nextMatrix.r[0]);
	nextMatrix.r[1] = DirectX::XMVector3Normalize(nextMatrix.r[1]);
	nextMatrix.r[2] = DirectX::XMVector3Normalize(nextMatrix.r[2]);

	for (size_t i = 0; i < leftpoints.size(); i++)
	{
		if (matrixQueueIteratorSecond == matrixQueue.end())
		{
			ZeroMemory(&leftpoints[i], sizeof(Engine::VertexMatrix));
			ZeroMemory(&rightPoints[i], sizeof(Engine::VertexMatrix));
		}
		else
		{
			matrix = nextMatrix;
			nextMatrix = DirectX::XMLoadFloat4x4(&*matrixQueueIteratorSecond);
			nextMatrix.r[0] = DirectX::XMVector3Normalize(nextMatrix.r[0]);
			nextMatrix.r[1] = DirectX::XMVector3Normalize(nextMatrix.r[1]);
			nextMatrix.r[2] = DirectX::XMVector3Normalize(nextMatrix.r[2]);

			DirectX::XMStoreFloat4(&leftpoints[i].right, DirectX::XMVector3TransformCoord(leftWingRight, matrix));
			DirectX::XMStoreFloat4(&leftpoints[i].up, DirectX::XMVector3TransformCoord(leftWingLeft, matrix));
			DirectX::XMStoreFloat4(&leftpoints[i].look, DirectX::XMVector3TransformCoord(leftWingLeft, nextMatrix));
			DirectX::XMStoreFloat4(&leftpoints[i].position, DirectX::XMVector3TransformCoord(leftWingRight, nextMatrix));

			DirectX::XMStoreFloat4(&rightPoints[i].right, DirectX::XMVector3TransformCoord(rightWingRight, matrix));
			DirectX::XMStoreFloat4(&rightPoints[i].up, DirectX::XMVector3TransformCoord(rightWingLeft, matrix));
			DirectX::XMStoreFloat4(&rightPoints[i].look, DirectX::XMVector3TransformCoord(rightWingLeft, nextMatrix));
			DirectX::XMStoreFloat4(&rightPoints[i].position, DirectX::XMVector3TransformCoord(rightWingRight, nextMatrix));

			matrixQueueIterator++;
			matrixQueueIteratorSecond++;
		}
	}
}


void WingVaporTrail::EnlistGameObject(Engine::GameObject* objectPointer)
{
	const ObjectInfomation* pilotInfomation{nullptr};

	Engine::Component* output = objectPointer->GetComponent(L"AIPilot");
	if (output == nullptr)
	{
		output = objectPointer->GetComponent(L"SuperClassAIPilot");
		if (output == nullptr)
		{
			output = objectPointer->GetComponent(L"PlayerPilot");
			if (output == nullptr)
				return;

			pilotInfomation = &static_cast<PlayerPilot*>(output)->LinkObjectInfomation();
		}
		else
		{
			pilotInfomation = &static_cast<AIPilot*>(output)->LinkObjectInfomation();
		}
	}
	else
	{
		pilotInfomation = &static_cast<AIPilot*>(output)->LinkObjectInfomation();
	}
	if (pilotInfomation == nullptr)
		return;

	AircraftInfo infomation;
	infomation.gameObjectPointer = objectPointer;
	infomation.leftOffset = pilotInfomation->aircraftInfomation.WingTrailLeftOffset;
	infomation.rightOffset = pilotInfomation->aircraftInfomation.WingTrailRightOffset;
	infomation.trailWidthSize = pilotInfomation->aircraftInfomation.WingTrailWidthOffset;
	if (infomation.trailWidthSize == 0.0f)
		infomation.trailWidthSize = 1.0f;

	trailPositions.push_back({ infomation , std::list<Matrix>() });
}
