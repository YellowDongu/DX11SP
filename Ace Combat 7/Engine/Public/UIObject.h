#pragma once
#include "GameObject.h"

struct ENGINEDLL UIParts
{
	ID3D11Buffer* vertexBuffer = nullptr;
	ID3D11Buffer* indexBuffer = nullptr;
	ID3D11ShaderResourceView* texture = nullptr;
	UINT indexCount = 0;
	Vector2 scale;

	HRESULT LoadUITexture(const std::wstring filePath);

	void Free(void)
	{
		if (vertexBuffer != nullptr) { vertexBuffer->Release(); vertexBuffer = nullptr; }
		if (indexBuffer != nullptr) { indexBuffer->Release(); indexBuffer = nullptr; }
		if (texture != nullptr) { texture->Release(); texture = nullptr; }
	}
};
namespace Engine
{
	class ENGINEDLL UIObject : public GameObject
	{
	protected:
		UIObject(void) = delete;
		UIObject(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : GameObject(dxDevice, dxDeviceContext), CenterPosition{}, scale{}, angle(0.0f), active(true) {}
		virtual ~UIObject(void) = default;
	public:
		virtual HRESULT Start(void) = 0;
		virtual HRESULT Awake(void) = 0;
		virtual void FixedUpdate(void) = 0;
		virtual void Update(void) = 0;
		virtual void LateUpdate(void) = 0;
		virtual void Render(void) = 0;

		HRESULT CreateScale(ID3D11ShaderResourceView* textureView, Vector2& scale);
		HRESULT CreateVertex(ID3D11Buffer*& vertexBuffer, ID3D11Buffer*& indexBuffer, UINT& vertexCount, const Vector2& scale);
		DirectX::XMMATRIX CreateMatrix(const Vector2& position, const Vector2& scale, FLOAT angle);

		void SetActive(bool value) { active = value; }
		bool IsActive(void) { return active; }
	protected:
		HRESULT CreateBuffer(ID3D11Device* dxDevice, ID3D11Buffer*& vertexBuffer, ID3D11Buffer*& indexBuffer, std::vector<UIVertex>& vertices, std::vector<UINT>& indices);
		void CreateVertex(std::vector<UIVertex>& vertex, std::vector<UINT>& index, const Vector2& scale);
		
		bool active;
		Vector2 CenterPosition;
		Vector2 scale;
		FLOAT angle;
	};
}