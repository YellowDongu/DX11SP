#pragma once

class MS01TerrainE : public Engine::GameObject
{
private:
	MS01TerrainE(void) = delete;
	MS01TerrainE(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	MS01TerrainE(const MS01TerrainE& other);
	virtual ~MS01TerrainE(void) = default;
	virtual void Free(void);
public:
	static MS01TerrainE* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	virtual Engine::GameObject* Clone(void) override;

	HRESULT Start(void);
	virtual void Update(void);
	virtual void LateUpdate(void);
	virtual void FixedUpdate(void);
	virtual void Render(void);


private:
	Engine::StaticModel* model{nullptr};
};