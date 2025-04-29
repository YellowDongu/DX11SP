#pragma once

class OceanTile : public Engine::GameObject
{
private:
	OceanTile(void) = delete;
	OceanTile(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	OceanTile(const OceanTile& other);
	virtual ~OceanTile(void) = default;
	virtual void Free(void) override;
public:
	static OceanTile* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	virtual Engine::GameObject* Clone(void) override;

	HRESULT Start(void);
	virtual void Update(void) override;
	virtual void LateUpdate(void) override;
	virtual void FixedUpdate(void) override;
	virtual void Render(void) override;

private:
	Engine::StaticModel* model{ nullptr };
	ID3D11ShaderResourceView* waveNormalTexture{ nullptr };
};
