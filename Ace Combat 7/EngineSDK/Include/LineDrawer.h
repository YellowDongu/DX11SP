#pragma once
#include "Component.h"

namespace Engine
{
	class ENGINEDLL LineDrawer : public Component
	{
	public:
		LineDrawer(void) = delete;
		LineDrawer(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
		LineDrawer(const LineDrawer& other);
		virtual ~LineDrawer(void) = default;
		virtual void Free(void);
		static LineDrawer* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);

		virtual Component* Clone(void);

		virtual void Update(void) {}
		virtual void LateUpdate(void) {}
		virtual void FixedUpdate(void) {}

		HRESULT Start(void);
		void Render(void);

		void ClearLines(void) { positions.clear(); }
		void AddPosition(const Vector3& position) { positions.push_back(position); }
		void AddFrontPosition(const Vector3& position) { positions.push_front(position); }
		void PopFrontPosition(void) { positions.pop_front(); }
		UINT Size(void) { return static_cast<UINT>(positions.size()); }
		void SetColor(float4 value) { color = value; }
		void SetColor(fxmVector value) { DirectX::XMStoreFloat4(&color, value); }

	private:
		std::list<Vector3> positions;
		std::shared_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> primitiveBatch;
		std::shared_ptr<DirectX::BasicEffect> basicEffect;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
		float4 color = {1.0f, 0.0f, 0.0f, 1.0f};
	};

}