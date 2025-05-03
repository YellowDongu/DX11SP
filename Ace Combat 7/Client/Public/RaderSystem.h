#pragma once

#include "RaderSystem.h"

class RaderSystem : public Engine::Component
{
public:
	RaderSystem(void) = delete;
	RaderSystem(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	RaderSystem(const RaderSystem& other);
	virtual ~RaderSystem(void) = default;
	virtual void Free(void);
	static RaderSystem* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	virtual Engine::Component* Clone(void);
	
	virtual HRESULT Awake(void) override;
	virtual void Update(void) override;
	virtual void LateUpdate(void) override;
	virtual void FixedUpdate(void) override;

	void SearchEnemy(void);
	HRESULT LayerSearch(void);
private:
	bool allyFaction = false;

	std::vector<Engine::Layer*> allyLayer;
	std::vector<Engine::Layer*> enemyLayer;
	//static std::vector<Engine::Layer*> unknownLayer;

	Engine::GameObject** currentEnemy{nullptr}; // fcs에게 줄거
};
