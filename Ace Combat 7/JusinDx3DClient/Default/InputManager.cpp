#include "pch.h"
#include "InputManager.h"


/*

void InputManager::Init(HWND _hwnd)
{
	hwnd = _hwnd;
	states.resize(KEY_TYPE_COUNT, KeyState::None);
}

void InputManager::update()
{ // 프레임마다 키를 모두 스캔해서 갱신
	BYTE asciiKeys[KEY_TYPE_COUNT] = {};
	if (!::GetKeyboardState(asciiKeys)) { return; }

	for (UINT32 key = 0; key < KEY_TYPE_COUNT; key++)
	{
		if (asciiKeys[key] & 0x80)
		{
			KeyState& state = states[key];
			if (state == KeyState::END) { state = KeyState::None; }
			if (state == KeyState::Press) { continue; }

			if (state == KeyState::Down)
			{
				state = KeyState::Press;
			}
			else
			{
				state = KeyState::Down;
			}

		}
		else
		{
			KeyState& state = states[key];
			if (state == KeyState::END) { state = KeyState::None; }
			if (state == KeyState::None) { continue; }

			if (state == KeyState::Up)
			{
				state = KeyState::None;
			}
			else
			{
				state = KeyState::Up;
			}
		}
	}

	GetCursorPos(&mousePos);
	ScreenToClient(hwnd, &mousePos);
}




void PlayerInput::Update()
{
	checkInput();
}

void PlayerInput::checkInput()
{
	if (Input->getButton(hMinus))
	{
		horizontalInput = -1;
	}
	else if (Input->getButton(hPlus))
	{
		horizontalInput = 1;
	}
	else
	{
		horizontalInput = 0;
	}

	if (Input->getButton(vMinus))
	{
		verticalInput = -1;
	}
	else if (Input->getButton(vPlus))
	{
		verticalInput = 1;
	}
	else
	{
		verticalInput = 0;
	}
}


*/

