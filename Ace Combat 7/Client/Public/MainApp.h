#pragma once
#include "Base.h"


class MainApp final : public Base
{
private:
	MainApp(void);
	virtual ~MainApp(void) = default;
	void Free(void);
public:
	static MainApp* Create(void);

	void Start(void);
	HRESULT MainLoop(void);

	void Update(void);
	void LateUpdate(void);
	void FixedUpdate(void);
	void Render(void);

private:
	Engine::Act* sceneManager;
};