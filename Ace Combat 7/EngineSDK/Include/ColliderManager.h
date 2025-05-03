#pragma once
#include "Collider.h"
#include "NavMesh.h"
namespace Engine
{
	class ENGINEDLL ColliderManager : public Base
	{
	private:
		ColliderManager(void) = delete;
		ColliderManager(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
		ColliderManager(const ColliderManager&) = delete;
		virtual ~ColliderManager(void) = default;
	public:
		virtual void Free(void);
		static ColliderManager* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);

		void Update(void);
		void LateUpdate(void);
		void Render(void);

		HRESULT LoadNavMesh(std::wstring filePath);
		bool CheckCollision(Collider* collider, Collider* otherCollider);
		
		void AddCollider(Collider* collider) { colliders.push_back(collider); }
		void SetDebugRender(bool value) { debugRender = value; }
		NavMesh* CurrentNavMesh(void) { return navMesh; }
	private:
		bool debugRender = false;
		std::list<Collider*> colliders;
		NavMesh* navMesh{nullptr};
		ID3D11Device* device{nullptr};
		ID3D11DeviceContext* context{nullptr};
	};
}