#include "Foundation.h"

#include "InputManager.h"

using namespace Engine;

/// <summary>
/// 인스턴스를 만들어서 초기화까지 해준 다음 해당 인스턴스를 반환
/// </summary>
/// <param name="_device">directx장치</param>
/// <param name="hInstance">인스턴스의 핸들</param>
/// <param name="_hwnd">창의 핸들</param>
/// <returns></returns>
InputManager* InputManager::Create(const HINSTANCE& _hInstance, const HWND& _hwnd)
{
	InputManager* input = new InputManager();
	input->hwnd = _hwnd;
	input->hInstance = _hInstance;
	input->states.resize(KEY_TYPE_COUNT, KeyState::None);
	input->mouseStates.resize((int)MouseKey::ENDENUM, KeyState::None);

	if (FAILED(DirectInput8Create(_hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&input->dInput, NULL)))
	{
		delete input;
		throw std::runtime_error("Input Device Failed");
		return nullptr;
	}

	// 키보드 장치 생성
	if (FAILED(input->dInput->CreateDevice(GUID_SysKeyboard, &input->dInputDevice, NULL)))
	{
		delete input;
		throw std::runtime_error("Input Device Failed");
		return nullptr;
	}

	// 마우스 장치 생성
	if (FAILED(input->dInput->CreateDevice(GUID_SysMouse, &input->dMouseDevice, NULL)))
	{
		delete input;
		throw std::runtime_error("Input Device Failed");
		return nullptr;
	}

	// 장치에 대한 객체를 설정
	if (FAILED(input->dInputDevice->SetCooperativeLevel(_hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
	{
		delete input;
		throw std::runtime_error("Input Device Failed");
		return nullptr;
	}

	// 장치에 대한 객체를 설정
	if (FAILED(input->dMouseDevice->SetCooperativeLevel(_hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
	{
		delete input;
		throw std::runtime_error("Input Device Failed");
		return nullptr;
	}

	// 키보드 데이터 포맷 설정
	if (FAILED(input->dInputDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		delete input;
		throw std::runtime_error("Input Device Failed");
		return nullptr;
	}

	// 키보드 데이터 포맷 설정
	if (FAILED(input->dMouseDevice->SetDataFormat(&c_dfDIMouse)))
	{
		delete input;
		throw std::runtime_error("Input Device Failed");
		return nullptr;
	}

	return input;
}

void InputManager::Restart(void)
{
	Free();

	for (int i = 0; i < 3; i++)
	{
		if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dInput, NULL)))
		{
			Free();
			continue;
		}

		// 키보드 장치 생성
		if (FAILED(dInput->CreateDevice(GUID_SysKeyboard, &dInputDevice, NULL)))
		{
			Free();
			continue;
		}

		// 마우스 장치 생성
		if (FAILED(dInput->CreateDevice(GUID_SysMouse, &dMouseDevice, NULL)))
		{
			Free();
			continue;
		}

		// 장치에 대한 객체를 설정
		if (FAILED(dInputDevice->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
		{
			Free();
			continue;
		}

		// 장치에 대한 객체를 설정
		if (FAILED(dMouseDevice->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
		{
			Free();
			continue;
		}

		// 키보드 데이터 포맷 설정
		if (FAILED(dInputDevice->SetDataFormat(&c_dfDIKeyboard)))
		{
			Free();
			continue;
		}

		// 키보드 데이터 포맷 설정
		if (FAILED(dMouseDevice->SetDataFormat(&c_dfDIMouse)))
		{
			Free();
			continue;
		}

		return;
	}

	//throw std::runtime_error("Input Device Failed");
}

/// <summary>
/// 지속적으로 키보드와 마우스의 상태를 관찰하기 위한 메서드
/// </summary>
void InputManager::Update(void)
{
	if (dInputDevice == nullptr)
		return;
	// 프레임마다 키를 모두 스캔해서 갱신
	BYTE asciiKeys[KEY_TYPE_COUNT] = {};

	HRESULT hr = dInputDevice->GetDeviceState(sizeof(asciiKeys), (LPVOID)&asciiKeys);
	if (FAILED(hr))
	{
		if (hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED)
		{
			if (FAILED(dInputDevice->Acquire()))
				Restart(); UpdateAtError(); return;
		}
		else return;
	}

	for (UINT32 key = 0; key < KEY_TYPE_COUNT; key++)
	{
		if (asciiKeys[key] & 0x80)
		{
			KeyState& state = states[key];
			if (state == KeyState::Press) { continue; }
			else if (state == KeyState::Down)
			{
				state = KeyState::Press;
				continue;
			}
			else
			{
				state = KeyState::Down;
				continue;
			}
			if (state == KeyState::ENDENUM) { state = KeyState::None; }
		}
		else
		{
			KeyState& state = states[key];
			if (state == KeyState::None) { continue; }
			if (state == KeyState::Up)
			{
				state = KeyState::None;
				continue;
			}
			else
			{
				state = KeyState::Up;
				continue;
			}
			if (state == KeyState::ENDENUM) { state = KeyState::None; }
		}
	}

	if (dMouseDevice == nullptr)
		return;

	hr = dMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), &mouseState);
	if (FAILED(hr))
	{
		if (hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED)
		{
			if (FAILED(dMouseDevice->Acquire()))
				Restart(); UpdateAtError(true); return;
		}
		else return;
	}

	mouseMove.x = mouseState.lX;
	mouseMove.y = mouseState.lY;
	mouseWheel = mouseState.lZ;

	for (int i = (int)MouseKey::LButton; i < (int)MouseKey::ENDENUM; i++)
	{
		if (mouseState.rgbButtons[i] & 0x80)
		{
			KeyState& state = mouseStates[i];
			if (state == KeyState::Press) { continue; }
			else if (state == KeyState::Down)
			{
				state = KeyState::Press;
				continue;
			}
			else
			{
				state = KeyState::Down;
				continue;
			}
			if (state == KeyState::ENDENUM) { state = KeyState::None; }
		}
		else
		{
			KeyState& state = mouseStates[i];
			if (state == KeyState::None) { continue; }
			if (state == KeyState::Up)
			{
				state = KeyState::None;
				continue;
			}
			else
			{
				state = KeyState::Up;
				continue;
			}
			if (state == KeyState::ENDENUM) { state = KeyState::None; }
		}
	}


	POINT previousPosition = mousePosition;
	if (fixedMouse)
	{
		SetCursorPos((int)mouseHoldingPosition.x, (int)mouseHoldingPosition.y);
	}
	GetCursorPos(&mousePosition);
	ScreenToClient(hwnd, &mousePosition);

	// For raycast -> now deactivated
	//raycastViewSpace(device, rayDirection);
	//raycastWorldSpace(device, rayPosition, rayDirection);
	// For raycast -> now deactivated
}

/// <summary>
/// 모든 객체들의 메모리 할당 해제하는 함수
/// </summary>
void InputManager::Free(void)
{
	if (dInputDevice)
	{
		dInputDevice->Unacquire();
		dInputDevice->Release();
		dInputDevice = nullptr;
	}
	if (dMouseDevice)
	{
		dMouseDevice->Unacquire();
		dMouseDevice->Release();
		dMouseDevice = nullptr;
	}
	if (dInput)
	{
		dInput->Release();
		dInput = nullptr;
	}
}


/// <summary>
/// 마우스 위치를 고정시키는 함수
/// </summary>
/// <param name="value">고정 여부</param>
void InputManager::FixingMouse(bool value)
{
	if (fixedMouse == value)
		return;

	fixedMouse = value;
	ShowCursor(value);
}

void InputManager::UpdateAtError(bool onlyMouse)
{
	if (!onlyMouse)
	{
		for (auto& state : states)
		{
			if (state == KeyState::Down)
			{
				state = KeyState::Press;
			}
			else if (state == KeyState::Up)
			{
				state = KeyState::None;
			}
		}
	}
	for (auto& state : mouseStates)
	{

		if (state == KeyState::Down)
		{
			state = KeyState::Press;
		}
		else if (state == KeyState::Up)
		{
			state = KeyState::None;
		}
	}
	mouseMove.x = 0;
	mouseMove.y = 0;
	mouseWheel = 0;
}