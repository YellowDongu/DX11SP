#pragma once

#include "MultiObjectBuffer.h"

class WingTrailParticle : public Engine::MultiObjectBuffer
{
private:
	WingTrailParticle(void) = delete;
	WingTrailParticle(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	WingTrailParticle(const WingTrailParticle& other);
	virtual ~WingTrailParticle(void) = default;
	virtual void Free(void) override;
public:
	static WingTrailParticle* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	virtual Engine::Component* Clone(void) override;

	HRESULT Start(void);
	HRESULT Awake(void) override;
	virtual void LateUpdate(void) override;
	virtual void Render(void);

	void UpdatePoints(void);
	HRESULT BindWorldBuffer(void);
	HRESULT BindAdditionalBuffer(void);
	void AddMatrix(const Matrix& matrix) { matrixQueue.push_back(matrix); while (matrixQueue.size() > maxQueueSize) { matrixQueue.pop_front(); } }
	void SetMaxQueueSize(size_t size) { maxQueueSize = size; }
	void LinkTargetPoints(std::vector<Engine::VertexMatrix>* target) { targetPoints = target; }
	bool update = true; // for test
private:
	void CreateVertex(std::vector<StaticModelVertex>& vertex, std::vector<UINT>& index);

	size_t maxQueueSize = 100;
	std::vector<Engine::VertexMatrix>* targetPoints{nullptr};
	std::list<Matrix> matrixQueue;
	std::vector<StaticModelVertex> squareVertex = {
			{ {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f} },
			{ {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f} },
			{ {1.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f} },
			{ {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f} } 
		};
	std::vector<Engine::VertexMatrix> points;
	Engine::Transform* transform{nullptr};
	FLOAT timer{0.0f};
	Engine::Shader* shader;
	//y = -3x^(2) + 2
};
