#pragma once
#include "Component.h"

namespace Engine
{
	class ENGINEDLL PointDrawer : public Component
	{
	private:
		PointDrawer(void) = delete;
		PointDrawer(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
		PointDrawer(const PointDrawer& other);
		virtual ~PointDrawer(void) = default;
		virtual void Free(void) override;
	public:
		static PointDrawer* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, FLOAT radius = 1.0f);
		virtual Engine::Component* Clone(void) override;

		HRESULT Start(void);
		virtual void Update(void) override;
		virtual void LateUpdate(void) override;
		virtual void FixedUpdate(void) override;
		void Render(void);

		void SetPosition(Vector3 value) { memcpy(&position ,&value, sizeof(Vector3)); }
		void SetColor(float4 value) { memcpy(&color, &value, sizeof(float4)); }
		void SetColor(xmVector value) { DirectX::XMStoreFloat4(&color, value); }
		void ChangeRadius(FLOAT value);
	private:
		std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> primitiveBatch;
		std::unique_ptr<DirectX::BasicEffect> basicEffect;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
		std::shared_ptr<DirectX::GeometricPrimitive> sphere;
		float radius = 1.0f;
		Vector3 position = Vector3::zero();
		float4 color = {0.0f, 1.0f, 0.0f, 1.0f};
		const D3D11_VIEWPORT* viewPort{ nullptr };
	};
}
