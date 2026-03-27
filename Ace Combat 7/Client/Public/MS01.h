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

	HRESULT SetTrigger(void);
	HRESULT SetTerrain(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, Engine::Layer* layer);
	void SetPlayerInfomation(ObjectInfomation& infomation) { playerInfomation = infomation; aircraftSelected = true; }
private:
	bool aircraftSelected{ false };
	ObjectInfomation playerInfomation;

	class Player* player{ nullptr };
	class F15E* allyObject{ nullptr };
	class MainCamera* mainCamera{ nullptr };
	FMOD::Channel* bgmChannel{ nullptr };
	FMOD::Sound* bgm{ nullptr };
};


