#pragma once


class Title : public Engine::Scene
{
private:
	Title(void) = default;
	Title(const Title&) = delete;
	virtual ~Title(void) = default;
	virtual void Free(void);
public:
	static Title* Create(bool initiate);

	virtual void Start(void);
	virtual void Awake(void);
	virtual void Update(void);
	virtual void LateUpdate(void);
	virtual void FixedUpdate(void);
	virtual void Render(void);
	virtual void End(void);



private:

};
