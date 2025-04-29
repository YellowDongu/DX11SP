#pragma once

class MS01TerrainD : public Engine::GameObject
{
private:
	MS01TerrainD(void) = delete;
	MS01TerrainD(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	MS01TerrainD(const MS01TerrainD& other);
	virtual ~MS01TerrainD(void) = default;
	virtual void Free(void);
public:
	static MS01TerrainD* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	virtual Engine::GameObject* Clone(void) override;

	HRESULT Start(void);
	virtual void Update(void);
	virtual void LateUpdate(void);
	virtual void FixedUpdate(void);
	virtual void Render(void);


private:
	Engine::StaticModel* model{nullptr};
};