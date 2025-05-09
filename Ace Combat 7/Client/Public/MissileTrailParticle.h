#pragma once

#include "MultiObjectBuffer.h"

class MissileTrailParticle : public Engine::MultiObjectBuffer
{
public:
	struct TrailInfo
	{
		Engine::GameObject* gameObjectPointer;
		std::list<std::pair<Matrix, Engine::AdditionalVertexInfomation>> pointList;
	};
	MissileTrailParticle(void) = delete;
	MissileTrailParticle(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	MissileTrailParticle(const MissileTrailParticle& other);
	virtual ~MissileTrailParticle(void) = default;
	virtual void Free(void);
	static MissileTrailParticle* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	virtual Engine::Component* Clone(void);

	HRESULT Start(void);
	virtual HRESULT Awake(void) override;
	virtual void Render(void) override;

	HRESULT UpdateInfomation(void);
	HRESULT BindWorldBuffer(void);
	HRESULT BindAdditionalBuffer(void);
	HRESULT BindTrailConstantBuffer(void);
	void LinkTargetPoints(std::list<std::pair<Matrix, Engine::AdditionalVertexInfomation>>* target) { targets = target; }
private:
	std::vector<Engine::VertexMatrix> refinedMatrix;
	std::vector<Engine::AdditionalVertexInfomation> refinedInfomation;
	std::list<std::pair<Matrix, Engine::AdditionalVertexInfomation>>* targets{ nullptr };
	TrailBuffer trailBufferStorage;
	rotationMatrixBuffer rotationStorage;
	ID3D11ShaderResourceView* texture{nullptr};
	Engine::Shader* shader{nullptr};
	ID3D11Buffer* trailBuffer{ nullptr };
	ID3D11Buffer* rotationBuffer{ nullptr };
};
