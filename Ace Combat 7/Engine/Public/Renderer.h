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
	struct PipelineStatus
	{
		Matrix worldMatrix;
		class Shader* currentShader;
		class Shader* currentUIShader;
		class Camera* currnetCamera;
		Matrix viewMatrix;
		Matrix projectionMatrix;
		Matrix UIProjectionMatrix;

		ID3D11ShaderResourceView* currentDiffuseTexture;
		ID3D11ShaderResourceView* currentNormalTexture;
		//ID3D11ShaderResourceView* currentDiffuseTexture;

		const D3D11_VIEWPORT* viewPortInfomation;
	};



	class Shader;
	class GameObject;
	class ENGINEDLL Renderer final : public Base
	{
	public:
		Renderer(void) = delete;
		Renderer(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
		virtual ~Renderer(void) = default;

		virtual void Free(void) override;
	public:
		static Renderer* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, const D3D11_VIEWPORT* viewPortInfomation);
		void AddRenderObject(RenderType type, GameObject* object);
		void Render(void);
		void ClearRenderList(void);
		void RemoveRenderObject(RenderType type, GameObject* object);
		void RemoveRenderObject(GameObject* object);

		void ChangeShader(Shader* shader) { pipelineStatus.currentShader = shader; }
		void ChangeUIShader(Shader* shader) { pipelineStatus.currentUIShader = shader; }

	private:
		void BaseCameraSetting(void);
		void AlphaRender(void);
		void NonAlphaRender(void);
		void PriorityRender(void);
		void UIRender(void);

		PipelineStatus pipelineStatus;
		ID3D11Device* dxDevice;
		ID3D11DeviceContext* dxDeviceContext;
		std::wstring uiShaderName;
		std::list<GameObject*> renderTargets[static_cast<INT>(RenderType::End)];
	};
}