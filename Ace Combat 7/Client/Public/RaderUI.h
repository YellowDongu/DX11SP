#pragma once


class RaderUI : public Engine::UIObject
{
private:
	RaderUI(void) = delete;
	RaderUI(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	RaderUI(const RaderUI&) = delete;
	virtual ~RaderUI(void) = default;
	virtual void Free(void) override;
public:
	static RaderUI* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, Engine::GameObject* player, Engine::Layer** layers, UINT layerNumber);

	HRESULT Start(void) override;
	HRESULT Awake(void) override;
	void FixedUpdate(void) override;
	void Update(void) override;
	void LateUpdate(void) override;
	void Render(void) override;

	HRESULT CreateCustomVertex(ID3D11Buffer*& vertexBuffer, ID3D11Buffer*& indexBuffer, UINT& vertexCount, const Vector2& scale);
	void CreateCustomVertex(std::vector<UIVertex>& vertex, std::vector<UINT>& index, const Vector2& scale);
private:
	UINT raderState = 0;

	UIParts HUDOutline, HUDCircleL, HUDCircleM, HUDCircleS, RaderInnerLines;
	UIParts rader, guideLine, aircraft, targetAircraft;
	Engine::GameObject* player;
	std::vector<Engine::Layer*> searchLayers;

	Vector2 HUDOutlineScale;


	const UINT stride = sizeof(UIVertex);
	std::string diffuseTexture = "diffuseTexture";
	std::string world = "worldMatrix";
};
