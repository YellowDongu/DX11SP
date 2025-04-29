#pragma once

class MS01TerrainB : public Engine::GameObject
{
private:
	MS01TerrainB(void) = delete;
	MS01TerrainB(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	MS01TerrainB(const MS01TerrainB& other);
	virtual ~MS01TerrainB(void) = default;
	virtual void Free(void);
public:
	static MS01TerrainB* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	virtual Engine::GameObject* Clone(void) override;

	HRESULT Start(void);
	virtual void Update(void);
	virtual void LateUpdate(void);
	virtual void FixedUpdate(void);
	virtual void Render(void);


private:
	Engine::StaticModel* model{nullptr};
};