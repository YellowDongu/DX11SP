#pragma once
#include "Base.h"
#include "ModelLoader.h"
#include "Model.h"

namespace Engine
{
	class Model;
	class ModelFactory final : public Base
	{
	private:
		ModelFactory(void) = delete;
		ModelFactory(ID3D11Device* device, ID3D11DeviceContext* context);
		virtual ~ModelFactory(void) = default;
		virtual void Free(void);
	public:
		static ModelFactory* Create(ID3D11Device* device, ID3D11DeviceContext* context);

		HRESULT LoadModel(std::wstring filePath);
		HRESULT LoadModel(std::wstring filePath, Model*& model);
		Model* GetModel(std::wstring filePath);
	private:
		ID3D11Device* device;
		ID3D11DeviceContext* context;
		std::map<std::wstring, Model*> models;
	};
}