#pragma once

class MainMenu final : public Engine::Scene
{
public:
	MainMenu(void) = default;
	virtual ~MainMenu(void) = default;

	virtual void Start(void);
	virtual void Awake(void);
	virtual void Update(void);
	virtual void LateUpdate(void);
	virtual void FixedUpdate(void);
	virtual void Render(void);
	virtual void End(void);

private:

};
