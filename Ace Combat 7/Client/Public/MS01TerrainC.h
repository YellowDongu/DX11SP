#pragma once

class MS01TerrainC : public Engine::GameObject
{
private:
	MS01TerrainC(void) = delete;
	MS01TerrainC(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	MS01TerrainC(const MS01TerrainC& other);
	virtual ~MS01TerrainC(void) = default;
	virtual void Free(void);
public:
	static MS01TerrainC* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	virtual Engine::GameObject* Clone(void) override;

	HRESULT Start(void);
	virtual void Update(void);
	virtual void LateUpdate(void);
	virtual void FixedUpdate(void);
	virtual void Render(void);


private:
	Engine::StaticModel* model{nullptr};
};