#pragma once


#include "Base.h"


enum class RenderType
{
	Camera,
	Priority,
	NonBlend,
	Blend,
	UI,
	End
};

namespace Engine
{
	class Shader;
	class GameObject;
	class Renderer final : public Base
	{
	public:
		Renderer(void) = delete;
		Renderer(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
		virtual ~Renderer(void) = default;

		virtual void Free(void) override;
	public:
		static Renderer* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, DeviceInfomation& deviceInfo);
		void AddRenderObject(RenderType type, GameObject* object);
		void Render(void);
		void ClearRenderList(void);
		void RemoveRenderObject(RenderType type, GameObject* object);
		void RemoveRenderObject(GameObject* object);


	private:
		void BaseCameraSetting(void);
		void AlphaRender(void);
		void NonAlphaRender(void);
		void PriorityRender(void);
		void UIRender(void);

		ID3D11Device* dxDevice;
		ID3D11DeviceContext* dxDeviceContext;
		std::list<GameObject*> renderTargets[static_cast<INT>(RenderType::End)];

		Vector2 resolution;
	};
}