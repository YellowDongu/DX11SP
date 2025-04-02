#include "Base.h"
#define DIRECTINPUT_VERSION	0x0800
#include <dinput.h>
#pragma comment (lib, "dinput8.lib")

enum class KeyType
{
	Escape = DIK_ESCAPE
	, K1 = DIK_1
	, K2 = DIK_2
	, K3 = DIK_3
	, K4 = DIK_4
	, K5 = DIK_5
	, K6 = DIK_6
	, K7 = DIK_7
	, K8 = DIK_8
	, K9 = DIK_9
	, K0 = DIK_0
	, Minus = DIK_MINUS
	, Equal = DIK_EQUALS
	, Back = DIK_BACK
	, Tab = DIK_TAB
	, Q = DIK_Q
	, W = DIK_W
	, E = DIK_E
	, R = DIK_R
	, T = DIK_T
	, Y = DIK_Y
	, U = DIK_U
	, I = DIK_I
	, O = DIK_O
	, P = DIK_P
	, LBracket = DIK_LBRACKET
	, RBracket = DIK_RBRACKET
	, Return = DIK_RETURN
	, LCtrl = DIK_LCONTROL
	, A = DIK_A
	, S = DIK_S
	, D = DIK_D
	, F = DIK_F
	, G = DIK_G
	, H = DIK_H
	, J = DIK_J
	, K = DIK_K
	, L = DIK_L
	, Semicolon = DIK_SEMICOLON
	, Apostrophe = DIK_APOSTROPHE
	, Grave = DIK_GRAVE
	, LShift = DIK_LSHIFT
	, BackSlash = DIK_BACKSLASH
	, Z = DIK_Z
	, X = DIK_X
	, C = DIK_C
	, V = DIK_V
	, B = DIK_B
	, N = DIK_N
	, M = DIK_M
	, Comma = DIK_COMMA
	, Period = DIK_PERIOD
	, Slash = DIK_SLASH
	, Rshift = DIK_RSHIFT
	, Multiply = DIK_MULTIPLY
	, LMenu = DIK_LMENU
	, Space = DIK_SPACE
	, Capital = DIK_CAPITAL
	, F1 = DIK_F1
	, F2 = DIK_F2
	, F3 = DIK_F3
	, F4 = DIK_F4
	, F5 = DIK_F5
	, F6 = DIK_F6
	, F7 = DIK_F7
	, F8 = DIK_F8
	, F9 = DIK_F9
	, F10 = DIK_F10
	, NUMLOCK = DIK_NUMLOCK
	, SCROLL = DIK_SCROLL
	, NUMPAD7 = DIK_NUMPAD7
	, NUMPAD8 = DIK_NUMPAD8
	, NUMPAD9 = DIK_NUMPAD9
	, SUBTRACT = DIK_SUBTRACT
	, NUMPAD4 = DIK_NUMPAD4
	, NUMPAD5 = DIK_NUMPAD5
	, NUMPAD6 = DIK_NUMPAD6
	, ADD = DIK_ADD
	, NUMPAD1 = DIK_NUMPAD1
	, NUMPAD2 = DIK_NUMPAD2
	, NUMPAD3 = DIK_NUMPAD3
	, NUMPAD0 = DIK_NUMPAD0
	, DECIMAL = DIK_DECIMAL
	, OEM_102 = DIK_OEM_102
	, F11 = DIK_F11
	, F12 = DIK_F12
	, F13 = DIK_F13
	, F14 = DIK_F14
	, F15 = DIK_F15
	, KANA = DIK_KANA
	, CONVERT = DIK_CONVERT
	, NOCONVERT = DIK_NOCONVERT
	, YEN = DIK_YEN
	, NUMPADEQUALS = DIK_NUMPADEQUALS
	, PREVTRACK = DIK_PREVTRACK
	, AT = DIK_AT
	//, Close      = DIK_CLOSE
	//, Copy       = DIK_COPY
	//, Paste      = DIK_PASTE
	//, Print      = DIK_PRINT
	, Home = DIK_HOME
	, UP = DIK_UP
	, PGUP = DIK_PGUP
	, LEFT = DIK_LEFT
	, RIGHT = DIK_RIGHT
	, ENDKEY = DIK_END
	, DOWN = DIK_DOWN
	, PGDN = DIK_PGDN
	, INSERT = DIK_INSERT
	, Delete = DIK_DELETE
	//, LMETA      = DIK_LMETA
	//, RMETA      = DIK_RMETA
	, APPS = DIK_APPS
};

enum class MouseKey
{
	LButton,
	RButton,
	MButton,
	Button1,
	//Button2,
	ENDENUM
};
namespace Engine
{

	enum class KeyState
	{
		None,
		Down,
		Press,
		Up,

		ENDENUM
	};

	enum
	{
		KEY_TYPE_COUNT = static_cast<INT32>(UINT8_MAX) + 1,
		KEY_STATE_COUNT = static_cast<INT32>(KeyState::ENDENUM)
	};



	/// <summary>
	/// 키보드와 마우스의 상태를 update()를 통해 확인하고 현재 상태를 다른 객체들에게 전달하기 위한 클래스
	/// 싱글턴 클래스가 아님!
	/// </summary>
	class ENGINEDLL InputManager : public Base
	{
	private:
		InputManager(void) = default;
		virtual ~InputManager(void) = default;
		InputManager(const InputManager&) = delete;
		InputManager& operator=(const InputManager&) = delete;
		virtual void Free(void);
	public:
		static InputManager* Create(const HINSTANCE& hInstance, const HWND& _hwnd);

		void Restart(void);
		void Update(void);
		void FixingMouse(bool value);

		bool getButton(KeyType key) { return getState(key) == KeyState::Press; }
		bool getButtonDown(KeyType key) { return getState(key) == KeyState::Down; }
		bool getButtonUp(KeyType key) { return getState(key) == KeyState::Up; }

		bool getMouseButton(MouseKey key) { return getMouseState(key) == KeyState::Press; }
		bool getMouseButtonDown(MouseKey key) { return getMouseState(key) == KeyState::Down; }
		bool getMouseButtonUp(MouseKey key) { return getMouseState(key) == KeyState::Up; }

		const POINT& getMousePosition(void) const { return mousePosition; }
		const POINT& getMouseMovement(void) const { return mouseMove; }
		const int& getMouseWheel(void) const { return mouseWheel; }

		void setMouseHoldingPosition(Vector2 value) { mouseHoldingPosition = value; }

	private:
		KeyState getState(KeyType key) { return states[static_cast<UINT8>(key)]; }
		KeyState getMouseState(MouseKey key) { return mouseStates[static_cast<int>(key)]; }
		void UpdateAtError(bool onlyMouse = false);

		// mouse Area
		bool fixedMouse = false;
		int mouseWheel = 0;
		POINT mousePosition = { 0,0 };
		POINT mouseMove = { 0,0 };
		Vector2 mouseHoldingPosition = { 0.0f,0.0f };
		DIMOUSESTATE mouseState = { 0,0,0,0 };
		// mouse Area

		// handle & pointer
		HWND hwnd = NULL;
		HINSTANCE hInstance = NULL;
		LPDIRECTINPUT8 dInput = NULL;
		LPDIRECTINPUTDEVICE8 dInputDevice = NULL;
		LPDIRECTINPUTDEVICE8 dMouseDevice = NULL;
		// handle & pointer

		// key Area
		std::vector<KeyState> states = {};
		std::vector<KeyState> mouseStates = {};
		// key Area
	};


}