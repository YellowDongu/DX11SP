#include "Foundation.h"
#include "ModelFactory.h"

using namespace Engine;

ModelFactory::ModelFactory(ID3D11Device* _device, ID3D11DeviceContext* _context) : device(_device), context(_context)
{
	device->AddRef();
	context->AddRef();
}

void ModelFactory::Free(void)
{
	for (auto& model : models)
	{
		Base::Destroy(model.second);
	}
	models.clear();
	for (auto& model : staticModels)
	{
		Base::Destroy(model.second);
	}
	staticModels.clear();

	device->Release();
	context->Release();
}

ModelFactory* ModelFactory::Create(ID3D11Device* device, ID3D11DeviceContext* context)
{
	ModelFactory* newInstance = new ModelFactory(device, context);

	//if (FAILED(newInstance->Start()))
	//{
	//	Base::Destroy(newInstance);
	//	return nullptr;
	//}

	return newInstance;
}

HRESULT ModelFactory::LoadModel(std::wstring filePath)
{
	auto iterator = models.find(filePath);
	if (iterator != models.end())
		return S_OK;

	ModelLoader loader;
	Model* newInstance = Model::Create(device, context, filePath, loader, vertexSave);
	if (newInstance == nullptr)
		return E_FAIL;

	newInstance->SetAnimation(0);
	models[filePath] = newInstance;
	return S_OK;
}

HRESULT ModelFactory::LoadModel(std::wstring filePath, Model*& model)
{
	auto iterator = models.find(filePath);
	if (iterator != models.end())
	{
		model = static_cast<Model*>(iterator->second->Clone());
		if (model == nullptr)
			return E_FAIL;
		else
		{
			model->SetAnimation(0);
			return S_OK;
		}
	}

	ModelLoader loader;
	Model* newInstance = Model::Create(device, context, filePath, loader, vertexSave);
	if (newInstance == nullptr)
		return E_FAIL;

	models[filePath] = newInstance;

	model = static_cast<Model*>(newInstance->Clone());
	if (model == nullptr)
		return E_FAIL;
	model->SetAnimation(0);
	return S_OK;
}

Model* ModelFactory::GetModel(std::wstring filePath)
{
	auto iterator = models.find(filePath);
	if (iterator == models.end())
		return nullptr;
	else
	{
		Model* clonedModel = static_cast<Model*>(iterator->second->Clone());
		clonedModel->SetAnimation(0);
		return clonedModel;
	}
}

HRESULT ModelFactory::LoadStaticModel(std::wstring filePath)
{
	auto iterator = staticModels.find(filePath);
	if (iterator != staticModels.end())
		return S_OK;

	StaticModel* newInstance = StaticModel::Create(device, context, filePath, ModelLoader(), vertexSave);
	if (newInstance == nullptr)
		return E_FAIL;

	staticModels[filePath] = newInstance;
	return S_OK;
}

HRESULT ModelFactory::LoadStaticModel(std::wstring filePath, StaticModel*& model)
{
	auto iterator = staticModels.find(filePath);
	if (iterator != staticModels.end())
	{
		model = static_cast<StaticModel*>(iterator->second->Clone());
		if (model == nullptr)
			return E_FAIL;
		else
			return S_OK;
	}

	StaticModel* newInstance = StaticModel::Create(device, context, filePath, ModelLoader(), vertexSave);
	if (newInstance == nullptr)
		return E_FAIL;

	staticModels[filePath] = newInstance;

	model = static_cast<StaticModel*>(newInstance->Clone());
	if (model == nullptr)
		return E_FAIL;
	return S_OK;
}

StaticModel* ModelFactory::GetStaticModel(std::wstring filePath)
{
	auto iterator = models.find(filePath);
	if (iterator == models.end())
		return nullptr;
	else
		return static_cast<StaticModel*>(iterator->second->Clone());
}