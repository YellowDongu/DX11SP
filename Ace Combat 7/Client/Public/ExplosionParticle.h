#pragma once
#include "MultiObjectBuffer.h"


class ExplosionParticle : public Engine::MultiObjectBuffer
{
private:
	ExplosionParticle(void) = delete;
	ExplosionParticle(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	ExplosionParticle(const ExplosionParticle& other);
	virtual ~ExplosionParticle(void) = default;
	virtual void Free(void) override;
public:
	static ExplosionParticle* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, UINT instanceNumber);
	virtual Engine::Component* Clone(void) override;

	HRESULT Start(void);
	virtual HRESULT Awake(void) override;
	virtual void Update(void) override;
	virtual void LateUpdate(void) override;
	virtual void FixedUpdate(void) override;
	virtual void Render(void);

	HRESULT BindWorldBuffer(std::vector<Engine::VertexMatrix>& worldMatrix);
	HRESULT BindAdditionalBuffer(std::vector<Engine::AdditionalVertexInfomation>& additionalInfomation);
	void LinkInfomation(std::vector<Engine::VertexMatrix>* target, std::vector<Engine::AdditionalVertexInfomation>* infomation) { targetPoints = target; additionalInfomations = infomation; }
private:
	std::vector<Engine::VertexMatrix>* targetPoints{ nullptr };
	std::vector<Engine::AdditionalVertexInfomation>* additionalInfomations{ nullptr };

	DirectX::XMFLOAT3 textureInfomation;
	Engine::Transform* transform{ nullptr };
	FLOAT timer{ 0.0f };
	Engine::Shader* shader{nullptr};
	ID3D11ShaderResourceView* explosionAltasTexture{nullptr};
	//y = -3x^(2) + 2
};