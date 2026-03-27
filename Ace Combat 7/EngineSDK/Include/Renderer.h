#pragma once

#include "Base.h"

enum class RenderType
{
	Camera,
	Priority,
	NonBlend,
	NonBlendNonLight,
	Blend,
	Effect,
	UI,
	End
};

namespace Engine
{
	class Shader;
	class GameObject;

	struct PipelineStatus
	{
		class Shader* currentShader;
		class Shader* currentUIShader;
		class Camera* currnetCamera;
		Matrix worldMatrix;
		Matrix viewMatrix;
		Matrix projectionMatrix;
		Matrix viewProjectionMatrix;
		Matrix orthoProjectionMatrix;

		ID3D11ShaderResourceView* currentDiffuseTexture;
		ID3D11ShaderResourceView* currentNormalTexture;
		//ID3D11ShaderResourceView* currentDiffuseTexture;

		const D3D11_VIEWPORT* viewPortInfomation;
	};


	class ENGINEDLL Renderer final : public Base
	{
	public:
		Renderer(void) = delete;
		Renderer(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
		virtual ~Renderer(void) = default;

		virtual void Free(void) override;
	public:
		static Renderer* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, const D3D11_VIEWPORT* viewPortInfomation);

		HRESULT Start(void);

		void AddRenderObject(RenderType type, GameObject* object);
		void Render(void);
		void ClearRenderList(void);
		void RemoveRenderObject(RenderType type, GameObject* object);
		void RemoveRenderObject(GameObject* object);

		void ChangeShader(Shader* shader) { SetShader(shader); } // delete this
		void ChangeUIShader(Shader* shader) { pipelineStatus.currentUIShader = shader; }
		void SkipRender(void) { skip = true; }

		HRESULT BindCamera(Camera* camera);
		HRESULT BindVariable(const std::string& variableName, void* variable, size_t variableSize);
		HRESULT BindVariable(char* variableName, void* variable, size_t variableSize);
		HRESULT BindMatrix(const std::string& variableName, const Matrix& matrix);
		HRESULT BindMatrix(char* variableName, const Matrix& matrix);
		HRESULT BindViewProjectionMatrix(const Matrix& viewMatrix, const Matrix& projectionMatrix);
		HRESULT BindWorldMatrix(const Matrix& matrix);
		HRESULT BindMatrix(const std::string& variableName, fxmMatrix matrix);
		HRESULT BindMatrix(char* variableName, fxmMatrix matrix);
		HRESULT BindWorldMatrix(fxmMatrix matrix);
		HRESULT BindViewProjectionMatrix(fxmMatrix viewMatrix, fxmMatrix projectionMatrix);

		HRESULT SetShader(Shader* shader);
		Shader* GetCurrentShader(void) { return pipelineStatus.currentShader; }

		const Matrix& WorldMatrix(void) const { return pipelineStatus.worldMatrix; }
		const Matrix& ViewMatrix(void) const { return pipelineStatus.viewMatrix; }
		const Matrix& ProjectionMatrix(void) const { return pipelineStatus.projectionMatrix; }
		const Matrix& ViewProjectionMatrix(void) const { return pipelineStatus.viewProjectionMatrix; }
		const Matrix& OrthoProjectionMatrix(void) const { return pipelineStatus.orthoProjectionMatrix; }
		const PipelineStatus& CurrentPipeLineStatus(void) const { return pipelineStatus; }
	private:
		void BaseCameraSetting(void);
		void PriorityRender(void);
		void NonAlphaRender(void);
		void ShaderRender(void);
		void DeferredRender(void);
		void NonAlphaNonLightRender(void);
		void AlphaRender(void);
		void EffectRender(void);
		void UIRender(void);

		PipelineStatus pipelineStatus{};
		ID3D11Device* dxDevice;
		ID3D11DeviceContext* dxDeviceContext;
		std::wstring uiShaderName;
		std::list<GameObject*> renderTargets[static_cast<INT>(RenderType::End)];
		// for post processing shader
		Matrix worldMatrix, viewMatrix;
		Matrix viewProjectionMatrix;
		FLOAT width, height;

		Shader* deferredShader;
		class RectanglePolygon* deferredBuffer;
		bool skip;
	};
}