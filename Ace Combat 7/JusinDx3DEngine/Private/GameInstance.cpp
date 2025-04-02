#include "Foundation.h"
#include "GameInstance.h"


Engine::GameInstance* instance = nullptr;

#ifdef _DEBUG

#define CheckInstance(returnValue)			   \
if (EngineInstance() == nullptr)			   \
{											   \
	ErrMsg(L"GameInstance Not Initiated");	   \
	return returnValue;						   \
}											   \

#else
#define CheckInstance(returnValue)			  \

#endif // DEBUG


Engine::GameInstance* EngineInstance(void)
{
	if (!instance)
	{
		return nullptr;
	}
	return instance;
}

void DestroyGameInstance(void)
{
	Base::DestroyInstance(instance);
}

#pragma region device
ENGINEDLL ID3D11Device* DxDevice(void)
{
	CheckInstance(nullptr);
	return EngineInstance()->DxDevice();
}
ENGINEDLL ID3D11DeviceContext* DxDeviceContext(void)
{
	CheckInstance(nullptr);
	return EngineInstance()->DxDeviceContext();
}
#pragma endregion

#pragma region Timer
ENGINEDLL FLOAT DeltaTime(void)
{
	CheckInstance(0.0f);
	return EngineInstance()->Time()->deltaTime();
}
#pragma endregion

#pragma region Renderer
ENGINEDLL void AddRenderObject(RenderType type, Engine::GameObject* object)
{
	CheckInstance( );
	EngineInstance()->RenderManager()->AddRenderObject(type, object);
}
#pragma endregion

#pragma region Texture
ENGINEDLL HRESULT LoadTexture(const std::wstring path, const std::wstring key, ID3D11ShaderResourceView*& texture)
{
	CheckInstance(E_FAIL);
	HRESULT result;

	if (FAILED(EngineInstance()->GetTexture(key, texture)))
	{
		result = EngineInstance()->SetTexture(key, path);
		if (FAILED(result))
		{
			ErrMsg((std::wstring(L"FailToLoadTexture : ") + path).c_str());
			return result;
		}

		return EngineInstance()->GetTexture(key, texture);
	}

	return S_OK;
}
#pragma endregion

#pragma region shaders
Engine::Shader* GetShader(const std::wstring& key)
{
	CheckInstance(nullptr);
	return EngineInstance()->shaders()->GetShader(key);
}

Engine::Shader* GetCurrentShader(void)
{
	CheckInstance(nullptr);
	return EngineInstance()->shaders()->CurrentShader();
}

Engine::Shader* LoadShader(const std::wstring& filePath, D3D11_INPUT_ELEMENT_DESC** elements, UINT elementsSize)
{
	CheckInstance(nullptr);
	return EngineInstance()->shaders()->LoadShader(filePath, elements, elementsSize);
}

Engine::Shader* LoadShader(const std::wstring& filePath, std::vector<D3D11_INPUT_ELEMENT_DESC>& elements)
{
	CheckInstance(nullptr);
	return EngineInstance()->shaders()->LoadShader(filePath, elements);
}

HRESULT Render(ID3D11Buffer* indexBuffer, ID3D11Buffer* vertexBuffer, UINT stride, UINT offset, DXGI_FORMAT indexFormat, D3D11_PRIMITIVE_TOPOLOGY topology)
{
	CheckInstance(E_FAIL);
	return EngineInstance()->shaders()->CurrentShader()->Render(indexBuffer, vertexBuffer, stride, offset, indexFormat, topology);
}

ENGINEDLL HRESULT SetShader(void)
{
	CheckInstance(E_FAIL);
	return EngineInstance()->shaders()->CurrentShader()->SetShader();
}

ENGINEDLL HRESULT ApplyShader(void)
{
	CheckInstance(E_FAIL);
	return EngineInstance()->shaders()->CurrentShader()->ApplyShader();
}

ENGINEDLL HRESULT SetShader(const std::wstring& key)
{
	CheckInstance(E_FAIL);
	return EngineInstance()->shaders()->SetShader(key);
}

ENGINEDLL HRESULT AddShader(const std::wstring key, Engine::Shader* newShader)
{
	CheckInstance(E_FAIL);
	return EngineInstance()->shaders()->AddShader(key, newShader);
}

ENGINEDLL HRESULT SetMatrix(const std::string& variableName, const Matrix& matrix)
{
	CheckInstance(E_FAIL);
	if (EngineInstance()->shaders()->CurrentShader() != nullptr)
		return EngineInstance()->shaders()->CurrentShader()->BindMatrix(variableName, matrix);
	else return E_FAIL;
}

ENGINEDLL HRESULT BindConstantBuffer(const std::string& variableName, ID3D11Buffer* buffer)
{
	CheckInstance(E_FAIL);
	if (buffer == nullptr)
		return E_FAIL;	

	if (EngineInstance()->shaders()->CurrentShader() != nullptr)
		return EngineInstance()->shaders()->CurrentShader()->BindConstantBuffer(variableName, buffer);
	else return E_FAIL;
}

ENGINEDLL HRESULT SetTexture(const std::string& variableName, ID3D11ShaderResourceView* texture)
{
	CheckInstance(E_FAIL);

	if (EngineInstance()->shaders()->CurrentShader() != nullptr)
		return EngineInstance()->shaders()->CurrentShader()->BindTexture(variableName, texture);
	else return E_FAIL;
}

ENGINEDLL HRESULT SetConstantBuffer(const std::string& variableName, ID3D11Buffer* buffer)
{
	CheckInstance(E_FAIL);
	if (buffer == nullptr)
		return E_FAIL;

	if (EngineInstance()->shaders()->CurrentShader() != nullptr)
		return EngineInstance()->shaders()->CurrentShader()->SetConstantBuffer(variableName, buffer);
	else return E_FAIL;
}
#pragma endregion

#undef CheckInstance


using namespace Engine;

GameInstance::GameInstance() : infomation{}
{
}

GameInstance::~GameInstance()
{
}

GameInstance* GameInstance::Create(DeviceInfomation& _infomation)
{
	if (!instance)
		instance = new GameInstance();

	instance->infomation = _infomation;

	return instance;
}

HRESULT GameInstance::EntireInitialize(void)
{
	if (!Time()) ErrMsg(L"Initialize failed : Time"); return E_FAIL;
	if (!Device()) ErrMsg(L"Initialize failed : Device"); return E_FAIL;
	if (!Sound()) ErrMsg(L"Initialize failed : Sound"); return E_FAIL;
	if (!RenderManager()) ErrMsg(L"Initialize failed : RenderManager"); return E_FAIL;
	if (!clonePod()) ErrMsg(L"Initialize failed : clonePod"); return E_FAIL;
	if (!textures()) ErrMsg(L"Initialize failed : textures"); return E_FAIL;
	if (!ModelManager()) ErrMsg(L"Initialize failed : ModelManager"); return E_FAIL;
	if (!shaders()) ErrMsg(L"Initialize failed : shaders"); return E_FAIL;
	if (!SceneManager()) ErrMsg(L"Initialize failed : SceneManager"); return E_FAIL;
	if (!Input()) ErrMsg(L"Initialize failed : Input"); return E_FAIL;
	if (!TextManager()) ErrMsg(L"Initialize failed : TextManager"); return E_FAIL;

	return S_OK;
}

void GameInstance::EntireUpdate(void)
{
	if (timeManager)
		timeManager->Update();
	if (soundManager)
		soundManager->Update();
	if (input)
		input->Update();
}


TimeManager* GameInstance::Time(void)
{
	if (!timeManager)
	{
		timeManager = TimeManager::Create();
	}
	return timeManager;
}

DXDevice* GameInstance::Device(void)
{
	if (!dxDevice)
	{
		dxDevice = DXDevice::Create(infomation);
		device = dxDevice->GetDevice();
		Context = dxDevice->GetDeviceContext();
		device->AddRef();
		Context->AddRef();
	}
	return dxDevice;
}

ID3D11Device* GameInstance::DxDevice(void)
{
	if (device == nullptr)
	{
		if (Device() == nullptr)
			return nullptr;
	}

	return device;
}

ID3D11DeviceContext* GameInstance::DxDeviceContext(void)
{
	if (Context == nullptr)
	{
		if (Device() == nullptr)
			return nullptr;
	}

	return Context;
}

SoundManager* GameInstance::Sound(void)
{
	if (!soundManager)
	{
		soundManager = SoundManager::Create();
	}
	return soundManager;
}

InputManager* GameInstance::Input(void)
{
	if (!input)
	{
		input = InputManager::Create(infomation.hInstance, infomation.hWnd);
	}
	return input;
}

ClonePod* GameInstance::clonePod(void)
{
	if (prototypeManager)
	{
		prototypeManager = ClonePod::Create();
	}
	return prototypeManager;
}

Renderer* GameInstance::RenderManager(void)
{
	if (!rendererInstance)
	{
		if (device == nullptr || Context == nullptr)
			return nullptr;
		rendererInstance = Renderer::Create(device, Context, infomation);
	}
	return rendererInstance;
}


void GameInstance::AddRenderObject(RenderType type, GameObject* object)
{
	RenderManager()->AddRenderObject(type, object);
}

TextMaster* GameInstance::TextManager(void)
{
	if (!textMaster)
	{
		if (dxDevice == nullptr)
			return nullptr;
		textMaster = TextMaster::Create(device, Context, dxDevice->ViewPortInfomation());
	}
	return textMaster;
}

TextureManager* GameInstance::textures(void)
{
	if (!textureManager)
	{
		if (device == nullptr || Context == nullptr)
			return nullptr;
		textureManager = TextureManager::Create(device, Context);
	}
	return textureManager;
}

HRESULT GameInstance::SetTexture(std::wstring key, std::wstring path)
{
	return textures()->SetTexture(key, path);
}

HRESULT GameInstance::GetTexture(const std::wstring& key, ID3D11ShaderResourceView*& texture)
{
	return textures()->GetTexture(key, texture);
}

ShaderLoader* GameInstance::shaders(void)
{
	if (shaderManager == nullptr)
	{
		if (device == nullptr || Context == nullptr)
			return nullptr;
		shaderManager = ShaderLoader::Create(device, Context);
	}
	return shaderManager;
}

Act* GameInstance::SceneManager(void)
{
	if (act == nullptr)
	{
		act = Act::Create();
	}
	return act;
}

ModelFactory* GameInstance::ModelManager(void)
{
	if (modelManager == nullptr)
	{
		modelManager = ModelFactory::Create(device, Context);
	}
	return modelManager;
}

void GameInstance::Free(void)
{
	Base::DestroyInstance(act);
	Base::DestroyInstance(textMaster);
	Base::DestroyInstance(modelManager);
	Base::DestroyInstance(shaderManager);
	Base::DestroyInstance(prototypeManager);
	Base::DestroyInstance(textureManager);
	Base::DestroyInstance(rendererInstance);
	Base::DestroyInstance(soundManager);
	Base::DestroyInstance(timeManager);
	Base::DestroyInstance(input);




	if (device)
		device->Release();
	if (Context)
		Context->Release();

	device = nullptr;
	Context = nullptr;
	Base::DestroyInstance(dxDevice);

}
