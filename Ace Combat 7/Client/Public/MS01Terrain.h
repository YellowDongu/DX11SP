#pragma once

class MS01Terrain : public Engine::GameObject
{
private:
	MS01Terrain(void) = delete;
	MS01Terrain(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	MS01Terrain(const MS01Terrain& other);
	virtual ~MS01Terrain(void) = default;
	virtual void Free(void);
public:
	static MS01Terrain* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	virtual Engine::GameObject* Clone(void) override;

	HRESULT Start(void);
	virtual void Update(void);
	virtual void LateUpdate(void);
	virtual void FixedUpdate(void);
	virtual void Render(void);


private:
	Engine::StaticModel* model{nullptr};
	ID3D11ShaderResourceView* waveNormalTexture{ nullptr };
};