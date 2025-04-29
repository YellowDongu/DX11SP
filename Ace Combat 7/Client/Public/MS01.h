 #pragma once


class MS01 : public Engine::Scene
{
private:
	MS01(void);
	MS01(const MS01& other) = delete;
	virtual ~MS01(void) = default;
	virtual void Free(void);
public:
	static MS01* Create(void);

	virtual void Start(void);
	virtual void Awake(void);
	virtual void Update(void);
	virtual void LateUpdate(void);
	virtual void FixedUpdate(void);
	virtual void Render(void);
	virtual void End(void);

private:


};


