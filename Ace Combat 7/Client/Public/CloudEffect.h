#pragma once

#include "MultiObjectBuffer.h"

class CloudEffect : public Engine::MultiObjectBuffer
{
private:
	CloudEffect(void) = delete;
	CloudEffect(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	CloudEffect(const CloudEffect& other);
	virtual ~CloudEffect(void) = default;
	virtual void Free(void);
public:
	static CloudEffect* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	virtual Engine::Component* Clone(void) override;

	HRESULT Start(void);
	virtual HRESULT Awake(void) override;
	virtual void Update(void) override;
	virtual void LateUpdate(void) override;
	virtual void FixedUpdate(void) override;
	virtual void Render(void) override;

	HRESULT BindWorldBuffer(void);
	void LinkMatrixList(std::vector<Engine::VertexMatrix>& pointer) { vertexMatrix = &pointer; }
private:
	std::vector<Engine::VertexMatrix>* vertexMatrix{nullptr};
	//std::vector<Engine::AdditionalVertexInfomation>* vertexMatrix{nullptr};
	ID3D11ShaderResourceView* cloudTexture;
	Vector2 textureFrameSize;
	Engine::Shader* shader;
};