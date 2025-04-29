#include "pch.h"
#include "SU33Handler.h"

SU33Handler::SU33Handler(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : AircraftBoneHandler(dxDevice, dxDeviceContext)
{
}

SU33Handler::SU33Handler(const SU33Handler& other) : AircraftBoneHandler(other)
{
}

void SU33Handler::Free(void)
{
}

SU33Handler* SU33Handler::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, Engine::Model* body, Engine::Model* gear)
{
	SU33Handler* newInstance = new SU33Handler(dxDevice, dxDeviceContext);
	newInstance->model = body;
	newInstance->LinkModels(body, gear);

	if (FAILED(newInstance->Start()))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}

	return newInstance;
}

SU33Handler* SU33Handler::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext)
{
	SU33Handler* newInstance = new SU33Handler(dxDevice, dxDeviceContext);

	return newInstance;
}

Engine::Component* SU33Handler::Clone(void)
{
	return new SU33Handler(*this);
}

HRESULT SU33Handler::Start(void)
{
	boneSets.clear(); gearSets.clear();
	AircraftBoneHandler::CreateBoneSets(boneSets, body, body->DefaultAnimation());
	AircraftBoneHandler::CreateBoneSets(gearSets, landingGear, landingGear->DefaultAnimation());

	aileronLParts.push_back(&boneSets.find("L_Aileron")->second);
	aileronRParts.push_back(&boneSets.find("R_Aileron")->second);

	elevatorParts.push_back(&boneSets.find("L_Elevator")->second);
	elevatorParts.push_back(&boneSets.find("R_Elevator")->second);

	rudderParts.push_back(&boneSets.find("L_Rudder")->second);
	rudderParts.push_back(&boneSets.find("R_Rudder")->second);

	flapParts.push_back(&boneSets.find("Flap_01")->second);
	flapParts.push_back(&boneSets.find("Flap_02")->second);
	flapParts.push_back(&boneSets.find("Flap_03")->second);
	flapParts.push_back(&boneSets.find("Flap_04")->second);
	flapParts.push_back(&boneSets.find("Flap_05")->second);
	flapParts.push_back(&boneSets.find("Flap_06")->second);
	flapParts.push_back(&boneSets.find("Flap_07")->second);
	flapParts.push_back(&boneSets.find("Flap_08")->second);

	airbreakParts.push_back(&boneSets.find("AirBrake_01")->second);
	airbreakParts.push_back(&boneSets.find("AirBrake_02")->second);
	airbreakParts.push_back(&boneSets.find("AirBrake_03")->second);
	airbreakParts.push_back(&boneSets.find("LandingAirBrake")->second);

	airIntakeParts.push_back(&boneSets.find("AirIntake_01")->second);
	airIntakeParts.push_back(&boneSets.find("AirIntake_02")->second);
	airIntakeParts.push_back(&boneSets.find("AirIntake_03")->second);
	airIntakeParts.push_back(&boneSets.find("AirIntake_04")->second);



	wingParts.push_back(&boneSets.find("Wing_Fold_01")->second);
	wingParts.push_back(&boneSets.find("Wing_Fold_02")->second);
	wingParts.push_back(&boneSets.find("Wing_Fold_03")->second);
	wingParts.push_back(&boneSets.find("Wing_Fold_04")->second);
	wingParts.push_back(&boneSets.find("Wing_Fold_09")->second);
	wingParts.push_back(&boneSets.find("Wing_Fold_10")->second);
	//"Hook" 


	for (auto& gearSet : gearSets)
		landingGearParts.push_back(&gearSet.second);


	return S_OK;
}

HRESULT SU33Handler::Awake(void)
{
	body = static_cast<Engine::Model*>(gameObject->GetComponent(L"FullModel"));
	landingGear = static_cast<Engine::Model*>(gameObject->GetComponent(L"GearModel"));
	model = body;

	if (body == nullptr || landingGear == nullptr)
		return E_FAIL;

	boneSets.clear(); gearSets.clear();
	AircraftBoneHandler::CreateBoneSets(boneSets, body, body->DefaultAnimation());
	AircraftBoneHandler::CreateBoneSets(gearSets, landingGear, landingGear->DefaultAnimation());

	aileronLParts.push_back(&boneSets.find("L_Aileron")->second);
	aileronRParts.push_back(&boneSets.find("R_Aileron")->second);

	elevatorParts.push_back(&boneSets.find("L_Elevator")->second);
	elevatorParts.push_back(&boneSets.find("R_Elevator")->second);

	rudderParts.push_back(&boneSets.find("L_Rudder")->second);
	rudderParts.push_back(&boneSets.find("R_Rudder")->second);

	flapParts.push_back(&boneSets.find("Flap_01")->second);
	flapParts.push_back(&boneSets.find("Flap_02")->second);
	flapParts.push_back(&boneSets.find("Flap_03")->second);
	flapParts.push_back(&boneSets.find("Flap_04")->second);
	flapParts.push_back(&boneSets.find("Flap_05")->second);
	flapParts.push_back(&boneSets.find("Flap_06")->second);
	flapParts.push_back(&boneSets.find("Flap_07")->second);
	flapParts.push_back(&boneSets.find("Flap_08")->second);

	airbreakParts.push_back(&boneSets.find("AirBrake_01")->second);
	airbreakParts.push_back(&boneSets.find("AirBrake_02")->second);
	airbreakParts.push_back(&boneSets.find("AirBrake_03")->second);
	airbreakParts.push_back(&boneSets.find("LandingAirBrake")->second);

	airIntakeParts.push_back(&boneSets.find("AirIntake_01")->second);
	airIntakeParts.push_back(&boneSets.find("AirIntake_02")->second);
	airIntakeParts.push_back(&boneSets.find("AirIntake_03")->second);
	airIntakeParts.push_back(&boneSets.find("AirIntake_04")->second);

	wingParts.push_back(&boneSets.find("Wing_Fold_01")->second);
	wingParts.push_back(&boneSets.find("Wing_Fold_02")->second);
	wingParts.push_back(&boneSets.find("Wing_Fold_03")->second);
	wingParts.push_back(&boneSets.find("Wing_Fold_04")->second);
	wingParts.push_back(&boneSets.find("Wing_Fold_09")->second);
	wingParts.push_back(&boneSets.find("Wing_Fold_10")->second);

	//"Hook" 
	for (auto& gearSet : gearSets)
		landingGearParts.push_back(&gearSet.second);

	return S_OK;
}

void SU33Handler::Update(void)
{
	if (yoke != nullptr)
	{
		Elevator(yoke->x * -1.0f); Rudder(yoke->y); Aileron(yoke->z);
	}
	else
	{
		Rudder(static_cast<FLOAT>(Input()->getButton(KeyType::Z)) - static_cast<FLOAT>(Input()->getButton(KeyType::X)));
		Aileron(static_cast<FLOAT>(Input()->getButton(KeyType::C)) - static_cast<FLOAT>(Input()->getButton(KeyType::B)));
		Elevator(static_cast<FLOAT>(Input()->getButton(KeyType::F)) - static_cast<FLOAT>(Input()->getButton(KeyType::V)));
	}

	Flap(static_cast<FLOAT>(Input()->getButton(KeyType::M)));
	AirBrake(static_cast<FLOAT>(Input()->getButton(KeyType::S)));
	AirIntake(static_cast<FLOAT>(Input()->getButton(KeyType::N)));
	if (Input()->getButtonDown(KeyType::G))
		landingGearApply = !landingGearApply;
	LandingGear();
}

void SU33Handler::FixedUpdate(void)
{
}

void SU33Handler::LateUpdate(void)
{
	UpdateBones(boneSets, body);
	UpdateBones(gearSets, landingGear);
}


void SU33Handler::Flap(float inputValue)
{
	if (flapParts.empty())
		return;

	if (abs(inputValue) > 1.0f)
		inputValue /= abs(inputValue);
	else if (inputValue < 0.0f)
		inputValue = 0.0f;

	flapParts[0]->minTime = 0.0f;
	flapParts[0]->maxTime = 50.0f;

	if (inputValue != 0)
	{
		flapParts[0]->animationTime -= inputValue;
		if (flapParts[0]->animationTime < flapParts[0]->minTime)
			flapParts[0]->animationTime = flapParts[0]->minTime;

	}
	else if (flapParts[0]->animationTime != flapParts[0]->maxTime)
	{
		inputValue = 1.0f;
		flapParts[0]->animationTime += inputValue;
		if (flapParts[0]->animationTime > flapParts[0]->maxTime)
			flapParts[0]->animationTime = flapParts[0]->maxTime;
	}

	for (size_t i = 0; i < flapParts.size(); i++)
		flapParts[i]->animationTime = flapParts[0]->animationTime;
}

void SU33Handler::AirBrake(float inputValue)
{
	if (airbreakParts.empty())
		return;

	if (abs(inputValue) > 1.0f)
		inputValue /= abs(inputValue);
	else if (inputValue < 0.0f)
		inputValue = 0.0f;
	airbreakParts[0]->minTime = 50.0f;

	if (inputValue == 0 && airbreakParts[0]->animationTime != airbreakParts[0]->minTime)
	{
		inputValue = 1.0f;
		airbreakParts[0]->animationTime -= inputValue;
		if (airbreakParts[0]->animationTime < airbreakParts[0]->minTime)
			airbreakParts[0]->animationTime = airbreakParts[0]->minTime;
	}
	else
	{
		airbreakParts[0]->animationTime += inputValue;
		if (airbreakParts[0]->animationTime > airbreakParts[0]->maxTime)
			airbreakParts[0]->animationTime = airbreakParts[0]->maxTime;
	}

	for (size_t i = 1; i < airbreakParts.size(); i++)
		airbreakParts[i]->animationTime = airbreakParts[0]->animationTime;
}

void SU33Handler::AirIntake(float inputValue)
{
	if (airIntakeParts.empty())
		return;

	if (abs(inputValue) > 1.0f)
		inputValue /= abs(inputValue);
	else if (inputValue < 0.0f)
		inputValue = 0.0f;
	airIntakeParts[0]->minTime = 50.0f;
	if (inputValue != 0)
	{
		airIntakeParts[0]->animationTime += inputValue;
		if (airIntakeParts[0]->animationTime > airIntakeParts[0]->maxTime)
			airIntakeParts[0]->animationTime = airIntakeParts[0]->maxTime;
	}
	else if (airIntakeParts[0]->animationTime != airIntakeParts[0]->minTime)
	{
		inputValue = 1.0f;
		airIntakeParts[0]->animationTime -= inputValue;
		if (airIntakeParts[0]->animationTime < airIntakeParts[0]->minTime)
			airIntakeParts[0]->animationTime = airIntakeParts[0]->minTime;
	}

	for (size_t i = 1; i < airIntakeParts.size(); i++)
		airIntakeParts[i]->animationTime = airIntakeParts[0]->animationTime;
}

void SU33Handler::LandingGear(void)
{
	if (landingGearParts.empty())
		return;

	landingGearParts[0]->maxTime = 50.0f;
	if (landingGearApply)
	{
		if (landingGearParts[0]->animationTime != landingGearParts[0]->maxTime)
		{
			landingGearParts[0]->animationTime += 0.05f;
			if (landingGearParts[0]->animationTime > landingGearParts[0]->maxTime)
				landingGearParts[0]->animationTime = landingGearParts[0]->maxTime;

			for (size_t i = 1; i < landingGearParts.size(); i++)
				landingGearParts[i]->animationTime = landingGearParts[0]->animationTime;
		}
	}
	else
	{
		if (landingGearParts[0]->animationTime != landingGearParts[0]->minTime)
		{
			landingGearParts[0]->animationTime -= 0.05f;
			if (landingGearParts[0]->animationTime < landingGearParts[0]->minTime)
				landingGearParts[0]->animationTime = landingGearParts[0]->minTime;

			for (size_t i = 1; i < landingGearParts.size(); i++)
				landingGearParts[i]->animationTime = landingGearParts[0]->animationTime;
		}
	}

}


