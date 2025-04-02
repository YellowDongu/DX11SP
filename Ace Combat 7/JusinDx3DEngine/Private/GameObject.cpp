#include "Foundation.h"
#include "GameObject.h"
#include "Transform.h" // include component

using namespace Engine;


GameObject::GameObject(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : BaseObject(dxDevice, dxDeviceContext), components{}, transformComponent(nullptr)
{
}

GameObject::GameObject(const GameObject& other) : BaseObject(other), components{}, transformComponent(nullptr)
{
	transformComponent = static_cast<Transform*>(other.transformComponent->Clone());
	for (auto& component : other.components)
	{
		if (component.second == other.transformComponent)
			AddComponent(transformComponent, component.first);

		AddComponent(component.second->Clone(), component.first);
	}
}


GameObject::~GameObject(void)
{
}

GameObject* GameObject::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext)
{
	GameObject* newInstance = new GameObject(dxDevice, dxDeviceContext);
	return newInstance;
}

GameObject* GameObject::Clone(void)
{
	return new GameObject(*this);
}

void GameObject::Update(void)
{
	for (auto& component : components)
	{
		component.second->Update();
	}

}

void GameObject::LateUpdate(void)
{
	for (auto& component : components)
	{
		component.second->LateUpdate();
	}
}

void GameObject::FixedUpdate(void)
{
	for (auto& component : components)
	{
		component.second->FixedUpdate();
	}
}

void GameObject::AddComponent(Component* component, const std::wstring& key)
{
	if (component == nullptr)
		return;

	component->SetOwner(this);
	components[key] = component;
}

Component* GameObject::GetComponent(std::wstring key)
{
	auto component = components.find(key);
	if (component == components.end())
		return nullptr;

	return (*component).second;
}

HRESULT GameObject::CreateTransform(void)
{
	transformComponent = Transform::Create(dxDevice, dxDeviceContext);
	if (transformComponent != nullptr)
	{
		AddComponent(transformComponent, L"Transform");
		return  S_OK;
	}
	else return E_FAIL;
}

void GameObject::Free(void)
{
	transformComponent = nullptr;
	for (auto& component : components)
	{
		Base::Destroy(component.second);
	}
}
