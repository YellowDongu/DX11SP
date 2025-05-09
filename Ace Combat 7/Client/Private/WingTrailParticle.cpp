#include "pch.h"
#include "WingTrailParticle.h"
#include "DefaultParticleShader.h"

WingTrailParticle::WingTrailParticle(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Engine::MultiObjectBuffer(dxDevice, dxDeviceContext)
{
}

WingTrailParticle::WingTrailParticle(const WingTrailParticle& other) : Engine::MultiObjectBuffer(other)
{
}

void WingTrailParticle::Free(void)
{
	Engine::MultiObjectBuffer::Free();
}

WingTrailParticle* WingTrailParticle::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext)
{
	WingTrailParticle* newInstance = new WingTrailParticle(dxDevice, dxDeviceContext);

	if (FAILED(newInstance->Start()))
	{
		Base::Destroy(newInstance);
	return nullptr;
	}
	return newInstance;
}

Engine::Component* WingTrailParticle::Clone(void)
{
	return nullptr;
}

HRESULT WingTrailParticle::Start(void)
{
	std::vector<UINT> indices = { 0 };
	std::vector<StaticModelVertex> vertices = { {{ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f }} };
	instanceNumber = 100;
	if (FAILED(CreatePointIndexBuffer(dxDevice, indexBuffer, instanceNumber, indexCount, wholeIndexCount, indices)))
		return E_FAIL;
	if (FAILED(CreateVertexBuffer(dxDevice, vertexBuffer, vertexStride, vertices)))
		return E_FAIL;
	if (FAILED(CreateAdditionalVertexInfomationBuffer(dxDevice, additionalBuffer, additionalInfoStride, instanceNumber)))
		return E_FAIL;
	if (FAILED(CreateWorldBuffer(dxDevice, worldBuffer, instanceNumber, worldStride)))
		return E_FAIL;

	points.resize(instanceNumber);
	return S_OK;
}

HRESULT WingTrailParticle::Awake(void)
{
	//transform = gameObject->transform();
	//if (transform == nullptr)
	//	return E_FAIL;

	shader = GetShader(L"DefaultParticleShader");
	if (shader == nullptr)
	{
		shader = Engine::DefaultParticleShader::Create(dxDevice, dxDeviceContext);
		AddShader(L"DefaultParticleShader", shader);
	}
	return S_OK;
}

void WingTrailParticle::LateUpdate(void)
{
	if(!update)
		return;
	if (timer >= 0.01f)
	{
		timer -= 0.01f;
		matrixQueue.push_back(transform->WorldMatrix());
		while (matrixQueue.size() > maxQueueSize) 
		{
			matrixQueue.pop_front();
		}
	}
	timer += DeltaTime();
	UpdatePoints();
}

void WingTrailParticle::Render(void)
{
	const std::wstring& currentShaderName = ::GetCurrentShaderName();
	SetShader(shader);
	shader->PassNumber(1);
	SetViewProjectionMatrix();

	BindWorldBuffer();
	BindAdditionalBuffer();
	__super::Render();
	SetShader(currentShaderName);
}


void WingTrailParticle::UpdatePoints(void)
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
	xmVector right = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f), left = DirectX::XMVectorSet(-1.0f, 0.0f, 0.0f, 1.0f);
	for (size_t i = 0; i < points.size(); i++)
	{
		if (matrixQueueIteratorSecond == matrixQueue.end())
		{
			ZeroMemory(&points[i], sizeof(Engine::VertexMatrix));
		}
		else
		{
			matrix = DirectX::XMLoadFloat4x4(&*matrixQueueIterator);
			//matrix.r[0] = DirectX::XMVector3Normalize(matrix.r[0]);
			//matrix.r[1] = DirectX::XMVector3Normalize(matrix.r[1]);
			//matrix.r[2] = DirectX::XMVector3Normalize(matrix.r[2]);
			nextMatrix = DirectX::XMLoadFloat4x4(&*matrixQueueIteratorSecond);
			//nextMatrix.r[0] = DirectX::XMVector3Normalize(nextMatrix.r[0]);
			//nextMatrix.r[1] = DirectX::XMVector3Normalize(nextMatrix.r[1]);
			//nextMatrix.r[2] = DirectX::XMVector3Normalize(nextMatrix.r[2]);
			DirectX::XMStoreFloat4(&points[i].right, DirectX::XMVector3TransformCoord(right, matrix));
			DirectX::XMStoreFloat4(&points[i].up, DirectX::XMVector3TransformCoord(left, matrix));
			DirectX::XMStoreFloat4(&points[i].look, DirectX::XMVector3TransformCoord(left, nextMatrix));
			DirectX::XMStoreFloat4(&points[i].position, DirectX::XMVector3TransformCoord(right, nextMatrix));

			matrixQueueIterator++;
			matrixQueueIteratorSecond++;
		}
	}
}

void WingTrailParticle::CreateVertex(std::vector<StaticModelVertex>& vertex, std::vector<UINT>& index)
{
	vertex.clear();
	index.clear();

	vertex = { { {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f} } };
	index = { 0 };

	//vertex = {
	//	{ {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f} },
	//	{ {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f} },
	//	{ {1.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f} },
	//	{ {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f} } 
	//};
	//
	//index = {
	//0, 1, 2,
	//0, 2, 3
	//};
}

HRESULT WingTrailParticle::BindWorldBuffer(void)
{
	D3D11_MAPPED_SUBRESOURCE		worldSubResource{};

	if (FAILED(dxDeviceContext->Map(worldBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &worldSubResource)))
	{
		targetPoints = nullptr;
		return E_FAIL;
	}
	Engine::VertexMatrix* matrixBuffer = static_cast<Engine::VertexMatrix*>(worldSubResource.pData);

	if (targetPoints != nullptr)
		memcpy(matrixBuffer, targetPoints->data(), sizeof(Engine::VertexMatrix) * targetPoints->size());
	else
		memcpy(matrixBuffer, points.data(), sizeof(Engine::VertexMatrix) * points.size());

	dxDeviceContext->Unmap(worldBuffer, 0);
	targetPoints = nullptr;
	return S_OK;
}

HRESULT WingTrailParticle::BindAdditionalBuffer(void)
{
	D3D11_MAPPED_SUBRESOURCE		subResource{};
	Engine::AdditionalVertexInfomation defaultStructForTemp;
	defaultStructForTemp.infoFirst = 1.0f;
	defaultStructForTemp.infoSecond = { 1.0f,1.0f };
	if (FAILED(dxDeviceContext->Map(additionalBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource)))
		return E_FAIL;
	Engine::AdditionalVertexInfomation* buffer = static_cast<Engine::AdditionalVertexInfomation*>(subResource.pData);
	FLOAT first = 0.0f, second = 0.0f;
	//y = -15(x - 0.5) ^ (2) + 2
	for (size_t i = 0; i < points.size(); i++)
	{
		second = -15.0f * (i * 0.01f - 0.5f) * (i * 0.01f - 0.5f) + 2.0f;
		if (second > 1.0f)
			second = 1.0f;
		else if (second < 0.0f)
			second = 0.0f;

		defaultStructForTemp.infoFirst = second;

		defaultStructForTemp.infoSecond.x = first;
		defaultStructForTemp.infoSecond.y = second;

		memcpy(&buffer[i], &defaultStructForTemp, sizeof(Engine::AdditionalVertexInfomation));

		first = second;
	}

	dxDeviceContext->Unmap(additionalBuffer, 0);

	return S_OK;
}
