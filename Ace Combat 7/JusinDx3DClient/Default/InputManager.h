#pragma once
/*

// 데이터 영역용
#define DECLARE_SINGLETON(className)\
private:\
	className() {}\
public:\
	static className* GetInstance()\
	{\
		static className s_instance;\
		return &s_instance;\
	}\

// 동적할당 - 힙 영역용
#define CREATE_SINGLETON(className)\
private:\
	className();\
	~className();\
	className(const className& rhs) = delete;\
	className operator = (className& rObj) = delete;\
	static className* m_pInstance;\
public:\
	static className* GetInstance()\
	{\
		if (!m_pInstance)\
		{\
			m_pInstance = new className;\
		}\
		return m_pInstance;\
	}\
	static void	DestroyInstance()\
	{\
		if (m_pInstance)\
		{\
			delete m_pInstance;\
			m_pInstance = nullptr;\
		}\
	}\

// 인스턴스 불러오는 법
#define GET_SINGLETON(className) className::GetInstance()
// 동적할당 해제
#define DELETE_SINGLETON(className) className::DestroyInstance()

#define UINT8_MAX        0xffui8

enum class KeyType
{
	LeftMouse = VK_LBUTTON,
	RightMouse = VK_RBUTTON,

	Up = VK_UP,
	Down = VK_DOWN,
	Left = VK_LEFT,
	Right = VK_RIGHT,
	Space = VK_SPACE,

	Q = 'Q',
	W = 'W',
	E = 'E',
	R = 'R',
	T = 'T',
	Y = 'Y',
	U = 'U',
	I = 'I',
	O = 'O',
	P = 'P',

	A = 'A',
	S = 'S',
	D = 'D',
	F = 'F',
	G = 'G',
	H = 'H',
	J = 'J',
	K = 'K',
	L = 'L',

	Z = 'Z',
	X = 'X',
	C = 'C',
	V = 'V',
	B = 'B',
	N = 'N',
	M = 'M',
	Shift = VK_SHIFT,
	Ctrl = VK_CONTROL,
	Menu = VK_MENU,
	Escape = VK_ESCAPE,
	Caps = VK_CAPITAL,

	Delete = VK_DELETE,
	Insert = VK_INSERT,
	End = VK_END,
	//Pgup = vk_,
	//Pgdn = VK_INSERT,

	F1 = VK_F1,
	F2 = VK_F2,
	F3 = VK_F3,
	F4 = VK_F4,
	F5 = VK_F5,
	F6 = VK_F6,
	F7 = VK_F7,
	F8 = VK_F8,
	F9 = VK_F9,
	F10 = VK_F10,
	F11 = VK_F11,
	F12 = VK_F12,

	NUMPAD0 = VK_NUMPAD0,
	NUMPAD1 = VK_NUMPAD1,
	NUMPAD2 = VK_NUMPAD2,
	NUMPAD3 = VK_NUMPAD3,
	NUMPAD4 = VK_NUMPAD4,
	NUMPAD5 = VK_NUMPAD5,
	NUMPAD6 = VK_NUMPAD6,
	NUMPAD7 = VK_NUMPAD7,
	NUMPAD8 = VK_NUMPAD8,
	NUMPAD9 = VK_NUMPAD9,

};

enum class KeyState
{
	None,
	Down,
	Press,
	Up,

	END
};

enum
{
	KEY_TYPE_COUNT = static_cast<INT32>(UINT8_MAX) + 1,
	KEY_STATE_COUNT = static_cast<INT32>(KeyState::END)
};



class InputManager
{
	DECLARE_SINGLETON(InputManager)

public:
	void Init(HWND _hwnd);
	void update();

	bool getButton(KeyType key) { return getState(key) == KeyState::Press; }
	bool getButtonDown(KeyType key) { return getState(key) == KeyState::Down; }
	bool getButtonUp(KeyType key) { return getState(key) == KeyState::Up; }
	POINT getMousePos() { return mousePos; }


private:
	KeyState getState(KeyType key) { return states[static_cast<UINT8>(key)]; }

	HWND hwnd = 0;
	std::vector<KeyState> states = {};
	POINT mousePos = { (0,0) };
};

#define Input GET_SINGLETON(InputManager)


class PlayerInput
{
	DECLARE_SINGLETON(PlayerInput)

public:
	int horizontal() { return horizontalInput; }
	int vertical() { return verticalInput; }

	void Update();
	void changeKeyVMinus(KeyType key) { vMinus = key; }
	void changeKeyvPlus(KeyType key) { vPlus = key; }
	void changeKeyhMinus(KeyType key) { hMinus = key; }
	void changeKeyhPlus(KeyType key) { hPlus = key; }

private:
	int horizontalInput = 0, verticalInput = 0;
	KeyType vMinus = KeyType::S;
	KeyType vPlus = KeyType::W;
	KeyType hMinus = KeyType::A;
	KeyType hPlus = KeyType::D;

	void checkInput();
};

#define vhInput GET_SINGLETON(PlayerInput)

*/
