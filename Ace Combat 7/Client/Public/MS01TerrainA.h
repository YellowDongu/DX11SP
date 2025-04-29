#pragma once

class MS01TerrainA : public Engine::GameObject
{
private:
	MS01TerrainA(void) = delete;
	MS01TerrainA(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	MS01TerrainA(const MS01TerrainA& other);
	virtual ~MS01TerrainA(void) = default;
	virtual void Free(void);
public:
	static MS01TerrainA* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	virtual Engine::GameObject* Clone(void) override;

	HRESULT Start(void);
	virtual void Update(void);
	virtual void LateUpdate(void);
	virtual void FixedUpdate(void);
	virtual void Render(void);


private:
	Engine::StaticModel* model{nullptr};
};