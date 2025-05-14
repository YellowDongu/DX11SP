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
ENGINEDLL IDXGISwapChain* GetSwapChain(void)
{
	CheckInstance(nullptr);
	return EngineInstance()->Device()->GetDXSwapChain();
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
ENGINEDLL HRESULT BindCamera(Engine::Camera* camera)
{
	CheckInstance(E_FAIL);
	return EngineInstance()->RenderManager()->BindCamera(camera);
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
	return ::EngineInstance()->shaders()->GetShader(key);
}

Engine::Shader* GetCurrentShader(void)
{
	CheckInstance(nullptr);
	return ::EngineInstance()->RenderManager()->GetCurrentShader();
	return EngineInstance()->shaders()->CurrentShader();
}

Engine::Shader* LoadShader(const std::wstring& filePath, D3D11_INPUT_ELEMENT_DESC** elements, UINT elementsSize)
{
	CheckInstance(nullptr);
	return ::EngineInstance()->shaders()->LoadShader(filePath, elements, elementsSize);
}

Engine::Shader* LoadShader(const std::wstring& filePath, std::vector<D3D11_INPUT_ELEMENT_DESC>& elements)
{
	CheckInstance(nullptr);
	return ::EngineInstance()->shaders()->LoadShader(filePath, elements);
}

HRESULT Render(ID3D11Buffer* indexBuffer, ID3D11Buffer* vertexBuffer, UINT stride, UINT offset, DXGI_FORMAT indexFormat, D3D11_PRIMITIVE_TOPOLOGY topology)
{
	CheckInstance(E_FAIL);
	return ::GetCurrentShader()->Render(indexBuffer, vertexBuffer, stride, offset, indexFormat, topology);
	return ::EngineInstance()->shaders()->CurrentShader()->Render(indexBuffer, vertexBuffer, stride, offset, indexFormat, topology);
}

ENGINEDLL HRESULT SetShader(void)
{
	CheckInstance(E_FAIL);
	return ::GetCurrentShader()->SetShader();
	return ::EngineInstance()->shaders()->CurrentShader()->SetShader();
}

ENGINEDLL HRESULT ApplyShader(void)
{
	CheckInstance(E_FAIL);
	return GetCurrentShader()->ApplyShader();
	return ::EngineInstance()->shaders()->CurrentShader()->ApplyShader();
}

ENGINEDLL const std::wstring& GetCurrentShaderName(void)
{
	std::wstring null = L"";
	CheckInstance(null);
	return GetCurrentShader()->shaderFile;
	return EngineInstance()->shaders()->CurrentShaderName();
}

ENGINEDLL HRESULT SetShader(Engine::Shader* shader)
{
	CheckInstance(E_FAIL);

	return EngineInstance()->RenderManager()->SetShader(shader);
	return EngineInstance()->shaders()->SetShader(shader);
}

ENGINEDLL HRESULT SetShader(const std::wstring& key)
{
	CheckInstance(E_FAIL);
	Engine::Shader* shader = ::GetShader(key);

	if (shader == nullptr)
		return E_FAIL;
	return EngineInstance()->RenderManager()->SetShader(shader);
	return ::EngineInstance()->shaders()->SetShader(key);
}

ENGINEDLL HRESULT AddShader(const std::wstring key, Engine::Shader* newShader)
{
	CheckInstance(E_FAIL);
	return ::EngineInstance()->shaders()->AddShader(key, newShader);
}

ENGINEDLL HRESULT SetMatrix(const std::string& variableName, const Matrix& matrix)
{
	CheckInstance(E_FAIL);
	return ::EngineInstance()->RenderManager()->BindMatrix(variableName, matrix);

	if (::EngineInstance()->shaders()->CurrentShader() != nullptr)
		return ::EngineInstance()->shaders()->CurrentShader()->BindMatrix(variableName, matrix);
	else return E_FAIL;
}

ENGINEDLL void SetWorldMatrix(const Matrix& matrix)
{
	CheckInstance();
	::EngineInstance()->RenderManager()->BindMatrix(GetCurrentShader()->worldMatrixA, matrix);
}

ENGINEDLL void SetWorldMatrix(void)
{
	CheckInstance();
	::EngineInstance()->RenderManager()->BindMatrix(GetCurrentShader()->worldMatrixA, ::EngineInstance()->RenderManager()->WorldMatrix());
}

ENGINEDLL HRESULT SetViewProjectionMatrix(const Matrix& viewMatrix, const Matrix& projectionMatrix)
{
	CheckInstance(E_FAIL);
	return ::EngineInstance()->RenderManager()->BindViewProjectionMatrix(viewMatrix, projectionMatrix);
}

ENGINEDLL HRESULT SetViewProjectionMatrix(fxmMatrix viewMatrix, fxmMatrix projectionMatrix)
{
	CheckInstance(E_FAIL);
	return ::EngineInstance()->RenderManager()->BindViewProjectionMatrix(viewMatrix, projectionMatrix);

	//if (::EngineInstance()->shaders() == nullptr)
	//	return;
	//
	//::EngineInstance()->ViewProjectionMatrix(matrix);
	//::SetMatrix(::EngineInstance()->shaders()->CurrentShader()->viewProjectionMatrixA, matrix);
}

ENGINEDLL void SetViewProjectionMatrix(void)
{
	CheckInstance();
	::EngineInstance()->RenderManager()->BindViewProjectionMatrix(::EngineInstance()->RenderManager()->ViewMatrix(), ::EngineInstance()->RenderManager()->ProjectionMatrix());
}

ENGINEDLL HRESULT BindConstantBuffer(const std::string& variableName, ID3D11Buffer* buffer)
{
	CheckInstance(E_FAIL);
	if (buffer == nullptr)
		return E_FAIL;	

	if (::GetCurrentShader() != nullptr)
		return ::GetCurrentShader()->BindConstantBuffer(variableName, buffer);
	else return E_FAIL;
}

ENGINEDLL HRESULT SetTexture(const std::string& variableName, ID3D11ShaderResourceView* texture)
{
	CheckInstance(E_FAIL);

	if (::GetCurrentShader() != nullptr)
		return ::GetCurrentShader()->BindTexture(variableName, texture);
	else return E_FAIL;
}

ENGINEDLL const Engine::PipelineStatus& GetPipeLineStatus(void)
{
	return ::EngineInstance()->RenderManager()->CurrentPipeLineStatus();
}

ENGINEDLL HRESULT SetConstantBuffer(const std::string& variableName, ID3D11Buffer* buffer)
{
	CheckInstance(E_FAIL);
	if (buffer == nullptr)
		return E_FAIL;

	if (::GetCurrentShader() != nullptr)
		return ::GetCurrentShader()->SetConstantBuffer(variableName, buffer);
	else return E_FAIL;
}
ENGINEDLL void AddCollider(Engine::Collider* collider)
{
	CheckInstance();
	if (collider == nullptr)
		return;
	::EngineInstance()->ColliderSystem()->AddCollider(collider);
}
ENGINEDLL HRESULT LoadNavMesh(std::wstring filePath)
{
	CheckInstance(E_FAIL);

	return ::EngineInstance()->ColliderSystem()->LoadNavMesh(filePath);
}
ENGINEDLL Engine::NavMesh* GetNavMesh(void)
{
	CheckInstance(nullptr);

	return ::EngineInstance()->ColliderSystem()->CurrentNavMesh();
}
ENGINEDLL HRESULT AddPrefabComponent(wchar_t* Name, Engine::Component* object, Engine::Component** copySlot)
{
	CheckInstance(E_FAIL);
	std::wstring name = Name;
	if (FAILED(::EngineInstance()->clonePod()->SetPrefab(name, object)))
		return E_FAIL;
	if (copySlot != nullptr)
		*copySlot = ::EngineInstance()->clonePod()->CloneComponentPrefab(name);
	return S_OK;
}
ENGINEDLL HRESULT AddPrefabComponent(std::wstring& Name, Engine::Component* object, Engine::Component** copySlot)
{
	CheckInstance(E_FAIL);
	if(FAILED(::EngineInstance()->clonePod()->SetPrefab(Name, object)))
		return E_FAIL;
	if (copySlot != nullptr)
		*copySlot = ::EngineInstance()->clonePod()->CloneComponentPrefab(Name);
	return S_OK;
}
ENGINEDLL Engine::Component* InstantiateComponent(std::wstring& Name)
{
	return ::EngineInstance()->clonePod()->CloneComponentPrefab(Name);
}
ENGINEDLL Engine::Component* InstantiateComponent(wchar_t* Name)
{
	std::wstring name = std::wstring(Name);
	return ::EngineInstance()->clonePod()->CloneComponentPrefab(name);
}
ENGINEDLL HRESULT AddPrefabGameObject(wchar_t* Name, Engine::Component* object, Engine::GameObject** copySlot)
{
	CheckInstance(E_FAIL);
	std::wstring name = Name;
	if (FAILED(::EngineInstance()->clonePod()->SetPrefab(name, object)))
		return E_FAIL;
	if (copySlot != nullptr)
		*copySlot = ::EngineInstance()->clonePod()->CloneGameObjectPrefab(name);
	return S_OK;
}
ENGINEDLL HRESULT AddPrefabGameObject(std::wstring& Name, Engine::Component* object, Engine::GameObject** copySlot)
{
	CheckInstance(E_FAIL);
	if (FAILED(::EngineInstance()->clonePod()->SetPrefab(Name, object)))
		return E_FAIL;
	if (copySlot != nullptr)
		*copySlot = ::EngineInstance()->clonePod()->CloneGameObjectPrefab(Name);
	return S_OK;
}
ENGINEDLL Engine::GameObject* InstantiateGameObject(std::wstring& Name)
{
	return ::EngineInstance()->clonePod()->CloneGameObjectPrefab(Name);
}
ENGINEDLL Engine::GameObject* InstantiateGameObject(wchar_t* Name)
{
	std::wstring name = std::wstring(Name);
	return ::EngineInstance()->clonePod()->CloneGameObjectPrefab(name);
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
	if (!ColliderSystem()) ErrMsg(L"Initialize failed : ColliderManager"); return E_FAIL;

	return S_OK;
}

void GameInstance::TimeUpdate(void)
{
	if (timeManager)
		timeManager->Update();
}

void GameInstance::InTimeUpdate(void)
{
	if (soundManager)
		soundManager->Update();
	if (input)
		input->Update();
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
	if (soundManager == nullptr)
	{
		soundManager = SoundManager::Create();
	}
	return soundManager;
}

InputManager* GameInstance::Input(void)
{
	if (input == nullptr)
	{
		input = InputManager::Create(infomation.hInstance, infomation.hWnd);
	}
	return input;
}

ClonePod* GameInstance::clonePod(void)
{
	if (prototypeManager == nullptr)
	{
		prototypeManager = ClonePod::Create();
	}
	return prototypeManager;
}

DebugConsole* GameInstance::Console(void)
{
	if (console)
		console = DebugConsole::Create();
	return console;
}


Renderer* GameInstance::RenderManager(void)
{
	if (!rendererInstance)
	{
		if (device == nullptr || Context == nullptr)
			return nullptr;
		rendererInstance = Renderer::Create(device, Context, &dxDevice->ViewPortInfomation());
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

ColliderManager* Engine::GameInstance::ColliderSystem(void)
{
	if (colliders == nullptr)
	{
		colliders = ColliderManager::Create(device, Context);
	}
	return colliders;
}

void GameInstance::Free(void)
{
	Base::DestroyInstance(act);
	Base::DestroyInstance(console);
	Base::DestroyInstance(colliders);
	Base::DestroyInstance(textMaster);
	Base::DestroyInstance(prototypeManager);
	Base::DestroyInstance(rendererInstance);
	Base::DestroyInstance(modelManager);
	Base::DestroyInstance(textureManager);
	Base::DestroyInstance(shaderManager);
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
