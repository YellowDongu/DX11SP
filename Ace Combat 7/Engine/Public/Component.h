#pragma once
#include "BaseObject.h"


namespace Engine
{
	class GameObject;

	class ENGINEDLL Component abstract : public BaseObject
	{
	protected:
		Component(void) = delete;
		Component(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
		Component(const Component& other);
		virtual ~Component(void) = default;

	public:
		virtual Component* Clone(void) = 0;

		virtual HRESULT Awake(void) { return S_OK; }
		virtual void Update(void) {}
		virtual void LateUpdate(void) {}
		virtual void FixedUpdate(void) {}

		void SetOwner(GameObject* owner) { gameObject = owner; }
		GameObject* GetOwner(void) { return gameObject; }
	protected:
		GameObject* gameObject;

	};

}