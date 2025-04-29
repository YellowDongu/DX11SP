#pragma once


class SkyBox : public Engine::GameObject
{
private:
	SkyBox(void) = delete;
	SkyBox(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	SkyBox(const SkyBox&) = delete;
	virtual ~SkyBox(void) = default;
	virtual void Free(void);
public:
	static SkyBox* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, Engine::GameObject* mainCamera);
	virtual GameObject* Clone(void) override;

	HRESULT Start(void);
	HRESULT Awake(void);

	virtual void Update(void) override;
	virtual void LateUpdate(void) override;
	virtual void FixedUpdate(void) override;
	virtual void Render(void) override;

	HRESULT CreateMesh(ID3D11Device* dxDevice, ID3D11Buffer*& vertexBuffer, ID3D11Buffer*& indexBuffer, UINT& offset, UINT& stride, UINT& indexCount);
	void CreateMeshData(std::vector<CubeVertex>& vertices, std::vector<UINT>& indices);
	HRESULT CreateBuffer(ID3D11Device* dxDevice, ID3D11Buffer*& vertexBuffer, ID3D11Buffer*& indexBuffer, std::vector<CubeVertex>& vertices, std::vector<UINT>& indices);

private:
	bool originObject;
	float4 skyColorBegin;
	float4 skyColorEnd;
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	Engine::Shader* shader;
	Engine::GameObject* mainCamera;
	UINT offset, stride, indexCount;
};