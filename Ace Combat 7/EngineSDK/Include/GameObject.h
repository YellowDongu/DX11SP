#pragma once
#include "BaseObject.h"


namespace Engine
{
	class ENGINEDLL Component;
	class ENGINEDLL Transform;

	class ENGINEDLL GameObject : public BaseObject
	{
	protected:
		GameObject(void) = delete;
		GameObject(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
		GameObject(const GameObject& other);
		virtual ~GameObject(void) = default;
		virtual void Free(void);
	public:
		static GameObject* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
		virtual GameObject* Clone(void);
		
		virtual HRESULT Awake(void) { return S_OK; }
		virtual void Update(void);
		virtual void LateUpdate(void);
		virtual void FixedUpdate(void);
		virtual void Render(void) {}

		void SetActive(bool value) { active = value; }
		void AddComponent(Component* component, const std::wstring& key);

		Component* GetComponent(std::wstring key);
		Transform* transform(void) { return transformComponent; }
		bool IsActive(void) { return active; }
		bool Destroy(void) { return destroy; }
		void SetDestroy(bool value) { destroy = value; }

		virtual void Collide(GameObject* target, class Collider* targetCollision);

	protected:
		HRESULT CreateTransform(void);
		bool active{ true }, destroy{ false };

		Transform* transformComponent;
		std::map<std::wstring, Component*> components;
	};
}