#pragma once

class Trigger abstract : public Base
{
public:
	Trigger(void) = default;
	virtual ~Trigger(void) = default;

	virtual HRESULT Awake(void) {}
	virtual void Update(void) {}
	virtual void LateUpdate(void) {}
	virtual void FixedUpdate(void) {}
	virtual void ActiveTrigger(void) {}

	virtual void SetStandBy(void) { standby = true; }
	virtual void SetSleep(void) { standby = false; }

	bool StandBy(void) { return standby; }
	bool Sleep(void) { return !standby; }
	bool Delete(void) { return deleteThis; }
	void LinkTrigger(Trigger* target) { linkedTrigger.push_back(target); }
	const std::wstring& Name(void) const { return name; }
private:
	bool standby{ false }, deleteThis{false};
	std::wstring name;
	std::vector<Trigger*> linkedTrigger;
	std::vector<Engine::GameObject*> effectedTargets;
};


class TriggerBox : public Engine::GameObject
{
private:
	TriggerBox(void) = delete;
	TriggerBox(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	TriggerBox(const TriggerBox& other);
	virtual ~TriggerBox(void) = default;
	virtual void Free(void) override;
public:
	static TriggerBox* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	virtual Engine::GameObject* Clone(void) override;

	HRESULT Start(void);
	virtual HRESULT Awake(void)  override{ return S_OK; }
	virtual void Update(void) override;
	virtual void LateUpdate(void) override;
	virtual void FixedUpdate(void) override;
	Trigger* FindTrigger(wchar_t* triggerName);
	Trigger* FindTrigger(const std::wstring& triggerName);
	void ForceActiveTrigger(wchar_t* triggerName);

	void AddTrigger(Trigger* trigger) { if (triggers.find(trigger->Name()) != triggers.end()) return; triggers[trigger->Name()] = trigger; }

private:
	std::map<std::wstring, Trigger*> triggers;
};
