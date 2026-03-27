#pragma once

enum class TriggerType
{
	KillSpawnTrigger,
	TimeSpawnTrigger,
	PositionSpawnTrigger,
	End
};

struct GameObjectSpawnInfomation
{
	Vector3 position;
	Vector3 angle;
	std::wstring name;
	std::wstring layerName;
	Engine::GameObject* gameObject;
};

class Trigger abstract : public Base
{
protected:
	struct TriggerDescription
	{
		std::wstring triggerName;
		TriggerType triggerType;
		bool standByStatus = false;
		UINT sceneNumber;
		std::vector<Trigger*> linkedTrigger;

		TriggerDescription() : triggerType(TriggerType::End) {}
	};
public:
	Trigger(void) = default;
	virtual ~Trigger(void) = default;

	virtual HRESULT Awake(void) { return S_OK; }
	virtual void Update(void) {}
	virtual void LateUpdate(void) {}
	virtual void FixedUpdate(void) {}
	virtual void ActiveTrigger(void) {}

	void SetBaseInfomation(Trigger::TriggerDescription* description);

	virtual void SetStandBy(void) { standby = true; }
	virtual void SetSleep(void) { standby = false; }

	bool StandBy(void) { return standby; }
	bool Sleep(void) { return !standby; }
	bool Delete(void) { return deleteThis; }
	void LinkTrigger(Trigger* target) { linkedTrigger.push_back(target); }
	const std::wstring& Name(void) const { return name; }
protected:
	bool standby{ false }, deleteThis{false};
	std::wstring name;
	UINT sceneNumber;
	std::vector<Trigger*> linkedTrigger;
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
	virtual HRESULT Awake(void) override;
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


// 해당 레이어의 모든 객체들이 비활성화 상태일때 작동
class KillSpawnTrigger : public Trigger
{
	KillSpawnTrigger(void) = default;
	virtual ~KillSpawnTrigger(void) = default;
	virtual void Free(void) override;
public:
	struct TriggerDescription : public Trigger::TriggerDescription
	{
		std::wstring targetLayer;
		std::vector<GameObjectSpawnInfomation> spawnTargets;

		TriggerDescription() { triggerType = TriggerType::KillSpawnTrigger; }
	};
	static KillSpawnTrigger* Create(KillSpawnTrigger::TriggerDescription& triggerDescription);

	HRESULT Start(KillSpawnTrigger::TriggerDescription& description);
	virtual HRESULT Awake(void) override;
	virtual void Update(void) override;
	virtual void LateUpdate(void) override;
	virtual void FixedUpdate(void) override;
	virtual void ActiveTrigger(void) override;


protected:
	Engine::Layer* targetLayer{nullptr};
	std::wstring targetLayerName;
	std::vector<GameObjectSpawnInfomation> spawnTargets;
};


// 타이머가 특정 시간일때 발동
class TimeSpawnTrigger : public Trigger
{
	TimeSpawnTrigger(void) = default;
	virtual ~TimeSpawnTrigger(void) = default;
public:
	struct TriggerDescription : public Trigger::TriggerDescription
	{
		FLOAT* missionTimer;
		std::vector<GameObjectSpawnInfomation> spawnTargets;

		TriggerDescription() { triggerType = TriggerType::TimeSpawnTrigger; }
	};
	static KillSpawnTrigger* Create(TimeSpawnTrigger::TriggerDescription& triggerDescription);

	virtual HRESULT Awake(void) override;
	virtual void Update(void) override;
	virtual void LateUpdate(void) override;
	virtual void ActiveTrigger(void) override;


protected:
	FLOAT* Timer;
	std::vector<GameObjectSpawnInfomation> spawnTargets;
};


// 플레이어의 위치가 근처일때 작동
class PositionSpawnTrigger : public Trigger
{
	PositionSpawnTrigger(void) = default;
	virtual ~PositionSpawnTrigger(void) = default;
	virtual void Free(void) override;
public:
	struct TriggerDescription : public Trigger::TriggerDescription
	{
		Vector3 position;
		FLOAT arrivalDistance = ConvertFeetToWorld(100.0f) * 5.0f;
		std::vector<Engine::GameObject*> surveillanceTarget;
		bool altitudeActive{ false };
		std::vector<GameObjectSpawnInfomation> spawnTargets;

		TriggerDescription() { triggerType = TriggerType::PositionSpawnTrigger; }
	};
	static PositionSpawnTrigger* Create(PositionSpawnTrigger::TriggerDescription& triggerDescription);

	HRESULT Start(PositionSpawnTrigger::TriggerDescription& description);
	virtual HRESULT Awake(void) override;
	virtual void Update(void) override;
	virtual void LateUpdate(void) override;
	virtual void ActiveTrigger(void) override;


protected:
	Vector3 position;
	FLOAT arrivalDistance = ConvertFeetToWorld(100.0f) * 5.0f;
	std::vector<Engine::GameObject*> surveillanceTarget;
	std::vector<GameObjectSpawnInfomation> spawnTargets;
	bool altitudeActive{ false };
};

// 미션 마지막에 작동
class MS01EndTrigger : public Trigger
{
	MS01EndTrigger(void) = default;
	virtual ~MS01EndTrigger(void) = default;
	virtual void Free(void) override;
public:
	static MS01EndTrigger* Create(void);

	HRESULT Start(void);
	virtual HRESULT Awake(void) override;
	virtual void Update(void) override;
	virtual void LateUpdate(void) override;
	virtual void ActiveTrigger(void) override;

protected:
	class ScreenFadeOut* fadeOutScreen{nullptr};
	class PlayerStatusHeadUpDisplay* psHUD{nullptr};
	FLOAT timer{ 5.0f }, activeTimer{5.0f};

	FMOD::Sound* missionEndSound{nullptr};
	Engine::Text* text{ nullptr };
};
