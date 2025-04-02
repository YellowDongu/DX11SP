#pragma once
#include "TimeManager.h"
#include "DXDevice.h"
#include "SoundManager.h"
#include "ClonePod.h"
#include "Renderer.h"
#include "TextureManager.h"
#include "ShaderLoader.h"
#include "Act.h"
#include "ModelFactory.h"
#include "InputManager.h"
#include "Text.h"

namespace Engine
{
	class ENGINEDLL GameInstance final : public Base
	{
	private:
		GameInstance(void);
		virtual ~GameInstance(void);
		virtual void Free(void);
	public:
		static GameInstance* Create(DeviceInfomation& infomation);

		HRESULT EntireInitialize(void);
		void EntireUpdate(void);

		TimeManager* Time(void);

		DXDevice* Device(void);
		ID3D11Device* DxDevice(void);
		ID3D11DeviceContext* DxDeviceContext(void);

		SoundManager* Sound(void);
		InputManager* Input(void);


		ClonePod* clonePod(void);



		Renderer* RenderManager(void);
		void AddRenderObject(RenderType type, GameObject* object);

		TextMaster* TextManager(void);

		TextureManager* textures(void);
		HRESULT SetTexture(std::wstring key, std::wstring path);
		HRESULT GetTexture(const std::wstring& key, ID3D11ShaderResourceView*& texture);


		ShaderLoader* shaders(void);

		Act* SceneManager(void);

		ModelFactory* ModelManager(void);


	private:

		DXDevice* dxDevice = nullptr;
		TimeManager* timeManager = nullptr;
		SoundManager* soundManager = nullptr;
		ClonePod* prototypeManager = nullptr;
		Renderer* rendererInstance = nullptr;
		TextureManager* textureManager = nullptr;
		ShaderLoader* shaderManager = nullptr;
		Act* act = nullptr;
		ModelFactory* modelManager = nullptr;
		InputManager* input = nullptr;
		TextMaster* textMaster = nullptr;

		ID3D11Device* device = nullptr;
		ID3D11DeviceContext* Context = nullptr;

		DeviceInfomation infomation;
	};
}

ENGINEDLL Engine::GameInstance* EngineInstance(void);
ENGINEDLL void DestroyGameInstance(void);
ENGINEDLL inline Engine::SoundManager* Sound(void) { return EngineInstance()->Sound(); }


#pragma region DXDevice
ENGINEDLL inline HRESULT LoadModel(std::wstring filePath) { if (EngineInstance() == nullptr) return E_FAIL; return EngineInstance()->ModelManager()->LoadModel(filePath); }
ENGINEDLL inline HRESULT LoadModel(std::wstring filePath, Engine::Model*& model) { if (EngineInstance() == nullptr) return E_FAIL; return EngineInstance()->ModelManager()->LoadModel(filePath, model);}
ENGINEDLL inline Engine::Model* GetModel(std::wstring filePath) { if (EngineInstance() == nullptr) return nullptr; return EngineInstance()->ModelManager()->GetModel(filePath);}
#pragma endregion

#pragma region Model
ENGINEDLL inline Engine::InputManager* Input(void) { if (EngineInstance() == nullptr) return nullptr; return EngineInstance()->Input(); }
//ENGINEDLL inline bool GetButton(KeyType key) { if (EngineInstance() == nullptr) return false; return EngineInstance()->Input()->getButton(key); }
//ENGINEDLL inline bool GetButtonDown(KeyType key) { if (EngineInstance() == nullptr) return false; return EngineInstance()->Input()->getButtonDown(key); }
//ENGINEDLL inline bool GetButtonUp(KeyType key) { if (EngineInstance() == nullptr) return false; return EngineInstance()->Input()->getButtonUp(key); }
//ENGINEDLL inline const POINT getMousePosition(void) { if (EngineInstance() == nullptr) return POINT{}; return EngineInstance()->Input()->getMousePosition(); }
//ENGINEDLL inline const POINT getMouseMovement(void) { if (EngineInstance() == nullptr) return POINT{}; return EngineInstance()->Input()->getMouseMovement(); }
//ENGINEDLL inline const int getMouseWheel(void) { if (EngineInstance() == nullptr) return 0; return EngineInstance()->Input()->getMouseWheel(); }
#pragma endregion

#pragma region Input
ENGINEDLL inline Engine::DXDevice* Device(void) { return EngineInstance()->Device(); }
ENGINEDLL ID3D11Device* DxDevice(void);
ENGINEDLL ID3D11DeviceContext* DxDeviceContext(void);

#pragma endregion

#pragma region Time
ENGINEDLL inline Engine::TimeManager* Time(void) { return EngineInstance()->Time(); }
ENGINEDLL FLOAT DeltaTime(void);

#pragma endregion

#pragma region Text
ENGINEDLL inline Engine::Text* CreateText(const std::wstring fontName) { return EngineInstance()->TextManager()->CreateText(fontName); }
ENGINEDLL inline HRESULT LoadFont(const std::wstring fontName) { return EngineInstance()->TextManager()->LoadFont(fontName); }
ENGINEDLL inline HRESULT ChangeFont(const std::wstring fontName, Engine::Text& text) { return EngineInstance()->TextManager()->ChangeFont(fontName, text); }
ENGINEDLL FLOAT DeltaTime(void);
#pragma endregion

#pragma region Renderer
ENGINEDLL void AddRenderObject(RenderType type, Engine::GameObject* object);
#pragma endregion

#pragma region texture
ENGINEDLL HRESULT LoadTexture(const std::wstring path, const std::wstring key, ID3D11ShaderResourceView*& texture);
#pragma endregion

#pragma region shaders
ENGINEDLL Engine::Shader* GetShader(const std::wstring& key);
ENGINEDLL Engine::Shader* GetCurrentShader(void);
ENGINEDLL Engine::Shader* LoadShader(const std::wstring& filePath, D3D11_INPUT_ELEMENT_DESC** elements, UINT elementsSize);
ENGINEDLL Engine::Shader* LoadShader(const std::wstring& filePath, std::vector<D3D11_INPUT_ELEMENT_DESC>& elements);
ENGINEDLL HRESULT Render(ID3D11Buffer* indexBuffer, ID3D11Buffer* vertexBuffer, UINT stride, UINT offset, DXGI_FORMAT indexFormat, D3D11_PRIMITIVE_TOPOLOGY topology);
ENGINEDLL HRESULT SetShader(void);
ENGINEDLL HRESULT ApplyShader(void);
ENGINEDLL HRESULT SetShader(const std::wstring& key);
ENGINEDLL HRESULT AddShader(const std::wstring key, Engine::Shader* newShader);
ENGINEDLL HRESULT SetMatrix(const std::string& variableName, const Matrix& matrix);
ENGINEDLL HRESULT BindConstantBuffer(const std::string& variableName, ID3D11Buffer* buffer);
ENGINEDLL HRESULT SetTexture(const std::string& variableName, ID3D11ShaderResourceView* texture);
ENGINEDLL HRESULT SetConstantBuffer(const std::string& variableName, ID3D11Buffer* buffer);
#pragma endregion