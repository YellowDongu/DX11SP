#include "pch.h"
#include "MissileTrail.h"
#include "StaticModel.h"

MissileTrail::MissileTrail(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Engine::GameObject(dxDevice, dxDeviceContext)
{
}

MissileTrail::MissileTrail(const MissileTrail& other) : Engine::GameObject(other)
{
	particle = static_cast<MissileTrailParticle*>(GetComponent(L"MissileTrailParticle"));
}

void MissileTrail::Free(void)
{
	Engine::GameObject::Free();
}

MissileTrail* MissileTrail::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext)
{
	MissileTrail* newInstance = new MissileTrail(dxDevice, dxDeviceContext);
	if (FAILED(newInstance->Start()))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}
	return newInstance;
}

Engine::GameObject* MissileTrail::Clone(void)
{
	return new MissileTrail(*this);
}

HRESULT MissileTrail::Start(void)
{
	particle = MissileTrailParticle::Create(dxDevice, dxDeviceContext);
	if (particle == nullptr)
		return E_FAIL;
	AddComponent(particle, L"MissileTrailParticle");
	maxTime = 0.75f;

	return S_OK;
}

HRESULT MissileTrail::Awake(void)
{
	if (FAILED(particle->Awake()))
		return E_FAIL;
	return S_OK;
}

void MissileTrail::Update(void)
{
	Timer -= DeltaTime();
	for (auto iterator = trailInfoamtion.begin(); iterator != trailInfoamtion.end(); )
	{
		if (iterator->gameObjectPointer != nullptr)
		{
			if (iterator->gameObjectPointer->Destroy())
			{
				iterator->gameObjectPointer = nullptr;
			}
			else
				UpdateTrail(*iterator);
		}
		else
		{
			UpdateTrail(*iterator);
			if (iterator->pointList.empty())
			{
				iterator = trailInfoamtion.erase(iterator);
				continue;
			}
		}


		iterator++;
	}

	if (Timer <= 0.0f)
		Timer += recordTime;

	Engine::GameObject::Update();
}

void MissileTrail::LateUpdate(void)
{
	AddRenderObject(RenderType::Effect, this);
	Engine::GameObject::LateUpdate();
}

void MissileTrail::FixedUpdate(void)
{
	Engine::GameObject::FixedUpdate();
}

void MissileTrail::Render(void)
{
	for (auto& trailInfo : trailInfoamtion)
	{
		if (trailInfo.positionList.empty())
			continue;
		particle->LinkTargetPoints(&trailInfo.pointList);
		particle->LinkList(&trailInfo.positionList);
		particle->Render();
	}
}

HRESULT MissileTrail::AddTrail(Engine::GameObject* object)
{
	for (auto& trailInfo : trailInfoamtion)
	{
		if (trailInfo.gameObjectPointer == object)
			return S_OK;
	}

	MissileTrailParticle::TrailInfo newInfo;
	newInfo.gameObjectPointer = object;
	trailInfoamtion.push_back(newInfo);

	return S_OK;
}

//float Lerp(float a, float b, float t)
//{
//	return a + (b - a) * t;
//}
void GenerateInterpolatedPoints(const Vector3& pointA, const Vector3& pointB, FLOAT interval, std::vector<Vector3>& result)
{
	// 시작과 끝을 XMVECTOR로 변환
	xmVector xmPointA = XMLoadFloat3(&pointA);
	xmVector xmPointB = XMLoadFloat3(&pointB);

	// 두 점 사이의 거리 계산
	FLOAT distance = DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMVectorSubtract(xmPointB, xmPointA)));

	// 점 개수 계산 (시작점 제외)
	INT pointCount = static_cast<INT>(distance / interval);
	float prograss;
	for (int i = 0; i <= pointCount; ++i)
	{
		prograss = static_cast<FLOAT>(i) / pointCount;
		xmVector vi = DirectX::XMVectorLerp(xmPointA, xmPointB, prograss);

		Vector3 point;
		XMStoreFloat3(&point, vi);
		result.push_back(point);
	}
}
void MissileTrail::GenerateInterpolatedPoints(const Vector3& pointA, const Vector3& pointB, FLOAT frontTime, FLOAT interval, std::vector<std::pair<Vector3, FLOAT>>& result)
{
	#define Lerp(a, b, t) a + (b - a) * t

	xmVector xmPointA = XMLoadFloat3(&pointA);
	xmVector xmPointB = XMLoadFloat3(&pointB);

	FLOAT distance = DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMVectorSubtract(xmPointB, xmPointA)));

	INT pointCount = static_cast<INT>(distance / interval);
	FLOAT prograss, time;
	Vector3 point;
	for (int i = 1; i <= pointCount; ++i)
	{
		prograss = static_cast<FLOAT>(i) / pointCount;
		time = Lerp(frontTime, 0.0f, prograss);
		XMStoreFloat3(&point, DirectX::XMVectorLerp(xmPointA, xmPointB, prograss));
		result.push_back({ point, time });
	}
}

void MissileTrail::UpdateTrail(MissileTrailParticle::TrailInfo& infomation)
{
	if (Timer > 0.0f)
		return;
	auto iteratorFront = infomation.positionList.begin();
	auto iteratorSecond = iteratorFront;
	for (auto iterator = iteratorFront; iterator != infomation.positionList.end(); )
	{
		iterator->second.infoSecond.x += DeltaTime();

		if (iterator->second.infoSecond.x >= iterator->second.infoSecond.y)
		{
			if (iteratorFront == iterator)
			{
				iterator = infomation.positionList.erase(iterator);
				iteratorSecond = iteratorFront = iterator;
			}
			continue;
		}
		if (iterator == iteratorFront)
			iterator->second.infoFirst += DeltaTime();
		else
		{
			iterator->second.infoFirst = iteratorSecond->second.infoSecond.x;
			iteratorSecond++;
		}
		iterator++;
	}

	if (infomation.gameObjectPointer == nullptr)
		return;

	if (infomation.positionList.empty())
		infomation.positionList.push_back({ {infomation.gameObjectPointer->transform()->Position(), 0.0f}, {0.0f, {0.0f,maxTime}} });
	else
	{
		std::vector<std::pair<Vector3, FLOAT>> points;

		Vector3 previousPosition = infomation.positionList.back().first.first;
		FLOAT previousTime = infomation.positionList.back().second.infoSecond.x;
		Vector3 currentPosition = infomation.gameObjectPointer->transform()->Position();
		FLOAT distance = (previousPosition - currentPosition).magnitude();

		if (distance > 0.5f)
			GenerateInterpolatedPoints(previousPosition, currentPosition, previousTime, 0.5f, points);
		else
			points.push_back({ currentPosition, 0.0f });

		for (auto& point : points)
		{
			infomation.positionList.push_back({ {point.first, 0.0f}, {0.0f, {point.second,maxTime}} });
		}

		if (infomation.positionList.size() > particle->InstanceNumber())
			infomation.positionList.pop_front();
	}
}

void MissileTrail::UpdateTrail(MissileTrailParticle::TrailInfo& infomation, bool extinction)
{
	if (Timer > 0.0f)
		return;
	auto iteratorFront = infomation.pointList.begin();
	auto iteratorSecond = iteratorFront;
	for (auto iterator = iteratorFront; iterator != infomation.pointList.end(); )
	{
		iterator->second.infoSecond.x += DeltaTime();

		if (iterator->second.infoSecond.x >= iterator->second.infoSecond.y)
		{
			if (iteratorFront == iterator)
			{
				iterator = infomation.pointList.erase(iterator);
				iteratorSecond = iteratorFront = iterator;
			}
			continue;
		}
		if (iterator == iteratorFront)
			iterator->second.infoFirst += DeltaTime();
		else
		{
			iterator->second.infoFirst = iteratorSecond->second.infoSecond.x;
			iteratorSecond++;
		}
		iterator++;
	}

	if (!extinction)
	{
		if (infomation.pointList.empty())
			infomation.pointList.push_back({ {infomation.gameObjectPointer->transform()->WorldMatrix(), 0.0f}, {0.0f, {0.0f,maxTime}} });
		else
			infomation.pointList.push_back({ {infomation.gameObjectPointer->transform()->WorldMatrix(), infomation.pointList.back().first.second + 0.5f}, {infomation.pointList.back().second.infoFirst + 1.0f, {0.0f,maxTime}} });

		if (infomation.pointList.size() > particle->InstanceNumber())
			infomation.pointList.pop_front();
	}
}
