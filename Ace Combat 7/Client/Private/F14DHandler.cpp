#include "pch.h"
#include "F14DHandler.h"

F14DHandler::F14DHandler(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : AircraftBoneHandler(dxDevice, dxDeviceContext)
{
}

F14DHandler::F14DHandler(const F14DHandler& other) : AircraftBoneHandler(other)
{
}

void F14DHandler::Free(void)
{
}

F14DHandler* F14DHandler::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, Engine::Model* body, Engine::Model* gear)
{
	F14DHandler* newInstance = new F14DHandler(dxDevice, dxDeviceContext);
	newInstance->model = body;
	newInstance->LinkModels(body, gear);
	
	if (FAILED(newInstance->Start()))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}

	return newInstance;
}

F14DHandler* F14DHandler::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext)
{
	F14DHandler* newInstance = new F14DHandler(dxDevice, dxDeviceContext);
	return newInstance;
}

Engine::Component* F14DHandler::Clone(void)
{
	return new F14DHandler(*this);
}

HRESULT F14DHandler::Start(void)
{
	boneSets.clear(); gearSets.clear();
	AircraftBoneHandler::CreateBoneSets(boneSets, body, body->DefaultAnimation());
	AircraftBoneHandler::CreateBoneSets(gearSets, landingGear, landingGear->DefaultAnimation());


	aileronLParts.push_back(&boneSets.find("L_Spoiler")->second);
	aileronRParts.push_back(&boneSets.find("R_Spoiler")->second);

	elevatorParts.push_back(&boneSets.find("L_Elevator")->second);
	elevatorParts.push_back(&boneSets.find("R_Elevator")->second);
	taileronLParts.push_back(&boneSets.find("L_Elevator")->second);
	taileronRParts.push_back(&boneSets.find("R_Elevator")->second);

	rudderParts.push_back(&boneSets.find("Rudder_01")->second);
	rudderParts.push_back(&boneSets.find("Rudder_02")->second);
	rudderParts.push_back(&boneSets.find("Rudder_03")->second);
	rudderParts.push_back(&boneSets.find("Rudder_04")->second);

	flapParts.push_back(&boneSets.find("Flap_01")->second);
	flapParts.push_back(&boneSets.find("Flap_02")->second);
	flapParts.push_back(&boneSets.find("Flap_03")->second);
	flapParts.push_back(&boneSets.find("Flap_04")->second);
	flapParts.push_back(&boneSets.find("Flap_05")->second);
	flapParts.push_back(&boneSets.find("Flap_06")->second);
	flapParts.push_back(&boneSets.find("Front_Flap_01")->second);
	flapParts.push_back(&boneSets.find("Front_Flap_02")->second);

	airbreakParts.push_back(&boneSets.find("AirBrake_01")->second);
	airbreakParts.push_back(&boneSets.find("AirBrake_02")->second);
	airbreakParts.push_back(&boneSets.find("AirBrake_03")->second);
	airbreakParts.push_back(&boneSets.find("AirBrake_04")->second);
	airbreakParts.push_back(&boneSets.find("AirBrake_05")->second);

	airIntakeParts.push_back(&boneSets.find("AirIntake_01")->second);
	airIntakeParts.push_back(&boneSets.find("AirIntake_02")->second);
	airIntakeParts.push_back(&boneSets.find("AirIntake_03")->second);
	airIntakeParts.push_back(&boneSets.find("AirIntake_04")->second);
	airIntakeParts.push_back(&boneSets.find("AirIntake_05")->second);
	airIntakeParts.push_back(&boneSets.find("AirIntake_06")->second);
	airIntakeParts.push_back(&boneSets.find("AirIntake_07")->second);
	airIntakeParts.push_back(&boneSets.find("AirIntake_08")->second);

	wingParts.push_back(&boneSets.find("VariableWing_01")->second);
	wingParts.push_back(&boneSets.find("VariableWing_02")->second);

	landingGearParts.push_back(&boneSets.find("Hook")->second);


	for (auto& gearSet : gearSets)
		landingGearParts.push_back(&gearSet.second);

	return S_OK;
}

HRESULT F14DHandler::Awake(void)
{
	body = static_cast<Engine::Model*>(gameObject->GetComponent(L"FullModel"));
	landingGear = static_cast<Engine::Model*>(gameObject->GetComponent(L"GearModel"));
	model = body;

	if (body == nullptr || landingGear == nullptr)
		return E_FAIL;

	boneSets.clear(); gearSets.clear();
	AircraftBoneHandler::CreateBoneSets(boneSets, body, body->DefaultAnimation());
	AircraftBoneHandler::CreateBoneSets(gearSets, landingGear, landingGear->DefaultAnimation());

	aileronLParts.push_back(&boneSets.find("L_Spoiler")->second);
	aileronRParts.push_back(&boneSets.find("R_Spoiler")->second);

	elevatorParts.push_back(&boneSets.find("L_Elevator")->second);
	elevatorParts.push_back(&boneSets.find("R_Elevator")->second);
	taileronLParts.push_back(&boneSets.find("L_Elevator")->second);
	taileronRParts.push_back(&boneSets.find("R_Elevator")->second);

	rudderParts.push_back(&boneSets.find("Rudder_01")->second);
	rudderParts.push_back(&boneSets.find("Rudder_02")->second);
	rudderParts.push_back(&boneSets.find("Rudder_03")->second);
	rudderParts.push_back(&boneSets.find("Rudder_04")->second);

	flapParts.push_back(&boneSets.find("Flap_01")->second);
	flapParts.push_back(&boneSets.find("Flap_02")->second);
	flapParts.push_back(&boneSets.find("Flap_03")->second);
	flapParts.push_back(&boneSets.find("Flap_04")->second);
	flapParts.push_back(&boneSets.find("Flap_05")->second);
	flapParts.push_back(&boneSets.find("Flap_06")->second);
	flapParts.push_back(&boneSets.find("Front_Flap_01")->second);
	flapParts.push_back(&boneSets.find("Front_Flap_02")->second);

	airbreakParts.push_back(&boneSets.find("AirBrake_01")->second);
	airbreakParts.push_back(&boneSets.find("AirBrake_02")->second);
	airbreakParts.push_back(&boneSets.find("AirBrake_03")->second);
	airbreakParts.push_back(&boneSets.find("AirBrake_04")->second);
	airbreakParts.push_back(&boneSets.find("AirBrake_05")->second);

	airIntakeParts.push_back(&boneSets.find("AirIntake_01")->second);
	airIntakeParts.push_back(&boneSets.find("AirIntake_02")->second);
	airIntakeParts.push_back(&boneSets.find("AirIntake_03")->second);
	airIntakeParts.push_back(&boneSets.find("AirIntake_04")->second);
	airIntakeParts.push_back(&boneSets.find("AirIntake_05")->second);
	airIntakeParts.push_back(&boneSets.find("AirIntake_06")->second);
	airIntakeParts.push_back(&boneSets.find("AirIntake_07")->second);
	airIntakeParts.push_back(&boneSets.find("AirIntake_08")->second);

	wingParts.push_back(&boneSets.find("VariableWing_01")->second);
	wingParts.push_back(&boneSets.find("VariableWing_02")->second);

	landingGearParts.push_back(&boneSets.find("Hook")->second);


	for (auto& gearSet : gearSets)
		landingGearParts.push_back(&gearSet.second);

	return S_OK;
}

void F14DHandler::Update(void)
{
	if (yoke != nullptr)
	{
		//Elevator(yoke->x * -1.0f); Rudder(yoke->y); Aileron(yoke->z);
		Taileron(yoke->x * -1.0f, yoke->z); Rudder(yoke->y); if(!wingFold) Aileron(yoke->z);
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
	if (Input()->getButtonDown(KeyType::H))
		wingFold = !wingFold;
	LandingGear();
	VariableWing();
	//std::cout << transformComponent->Position().x << " :   " << transformComponent->Position().y << " :   " << transformComponent->Position().z << std::endl;
}

void F14DHandler::FixedUpdate(void)
{
}

void F14DHandler::LateUpdate(void)
{
	UpdateBones(boneSets, body);
	UpdateBones(gearSets, landingGear);
}


void F14DHandler::Taileron(float elevatorInputValue, float aileronInputValue)
{
	#define CheckMinMax(boneSet)						\
if (boneSet->animationTime < boneSet->minTime)		\
	boneSet->animationTime = boneSet->minTime;		\
if (boneSet->animationTime > boneSet->maxTime)		\
	boneSet->animationTime = boneSet->maxTime;		\

	if (abs(elevatorInputValue) > 1.0f)
		elevatorInputValue /= abs(elevatorInputValue);
	if (abs(aileronInputValue) > 1.0f)
		aileronInputValue /= abs(aileronInputValue);

	if (taileronRParts.size() == 0 || taileronLParts.size() == 0)
		return;

	float value;
	float middleValue = (taileronLParts[0]->maxTime - taileronLParts[0]->minTime) / 2.0f;
	if (elevatorInputValue == 0 && aileronInputValue == 0)
	{
		if (taileronLParts[0]->animationTime != middleValue)
		{
			value = middleValue - taileronLParts[0]->animationTime;
			value /= abs(value);

			taileronLParts[0]->animationTime += value;
		}
		if (taileronRParts[0]->animationTime != middleValue)
		{
			value = middleValue - taileronRParts[0]->animationTime;
			value /= abs(value);

			taileronRParts[0]->animationTime += value;
		}
	}
	else if (aileronInputValue == 0)
	{
		if (taileronLParts.size() == 0)
			return;

		if (abs(elevatorInputValue) > 1.0f)
			elevatorInputValue /= abs(elevatorInputValue);

		if (elevatorInputValue == 0)
		{
			if (taileronLParts[0]->animationTime != middleValue)
			{
				value = middleValue - taileronLParts[0]->animationTime;
				value /= abs(value);

				taileronLParts[0]->animationTime += value;

				for (size_t i = 1; i < taileronLParts.size(); i++)
					taileronLParts[i]->animationTime = taileronLParts[0]->animationTime;
			}
			if (taileronRParts[0]->animationTime != middleValue)
			{
				value = middleValue - taileronRParts[0]->animationTime;
				value /= abs(value);

				taileronRParts[0]->animationTime += value;

				for (size_t i = 1; i < taileronRParts.size(); i++)
					taileronRParts[i]->animationTime = taileronRParts[0]->animationTime;
			}
		}
		else
		{
			taileronLParts[0]->animationTime += elevatorInputValue;
			CheckMinMax(taileronLParts[0]);
			for (size_t i = 1; i < taileronLParts.size(); i++)
				taileronLParts[i]->animationTime = taileronLParts[0]->animationTime;

			taileronRParts[0]->animationTime += elevatorInputValue;
			CheckMinMax(taileronRParts[0]);
			for (size_t i = 1; i < taileronRParts.size(); i++)
				taileronRParts[i]->animationTime = taileronRParts[0]->animationTime;
		}
	}
	else
	{
		float max, min;
		if (elevatorInputValue == 0)
		{
			min = taileronLParts[0]->minTime;
			max = taileronLParts[0]->maxTime;
		}
		else if (elevatorInputValue > 0.0f)
		{
			min = middleValue;
			max = taileronLParts[0]->maxTime;
		}
		else
		{
			min = taileronLParts[0]->minTime;
			max = middleValue;
		}

		if (taileronLParts[0]->animationTime > max)
			//taileronLParts[0]->animationTime += aileronInputValue;
			taileronLParts[0]->animationTime -= 1.0f;
		else if(taileronLParts[0]->animationTime < min)
			taileronLParts[0]->animationTime += 1.0f;
		else
			taileronLParts[0]->animationTime += aileronInputValue;

		if (taileronLParts[0]->animationTime > taileronLParts[0]->maxTime)
			taileronLParts[0]->animationTime = taileronLParts[0]->maxTime;
		if (taileronLParts[0]->animationTime < taileronLParts[0]->minTime)
			taileronLParts[0]->animationTime = taileronLParts[0]->minTime;

		for (size_t i = 0; i < taileronLParts.size(); i++)
			taileronLParts[i]->animationTime = taileronLParts[0]->animationTime;




		if (taileronRParts[0]->animationTime > max)
			//taileronLParts[0]->animationTime += aileronInputValue;
			taileronRParts[0]->animationTime -= 1.0f;
		else if (taileronRParts[0]->animationTime < min)
			taileronRParts[0]->animationTime += 1.0f;
		else
			taileronRParts[0]->animationTime -= aileronInputValue;

		if (taileronRParts[0]->animationTime > taileronRParts[0]->maxTime)
			taileronRParts[0]->animationTime = taileronRParts[0]->maxTime;
		if (taileronRParts[0]->animationTime < taileronRParts[0]->minTime)
			taileronRParts[0]->animationTime = taileronRParts[0]->minTime;

		for (size_t i = 0; i < taileronRParts.size(); i++)
			taileronRParts[i]->animationTime = taileronRParts[0]->animationTime;
	}
	#undef CheckMinMax
}

void F14DHandler::Flap(float inputValue)
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

void F14DHandler::AirBrake(float inputValue)
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

void F14DHandler::AirIntake(float inputValue)
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

void F14DHandler::LandingGear(void)
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


void F14DHandler::VariableWing(void)
{
	if (wingParts.empty())
		return;

	wingParts[0]->minTime = 50.0f;
	if (wingFold)
	{
		if (wingParts[0]->animationTime != wingParts[0]->maxTime)
		{
			wingParts[0]->animationTime += 0.05f;
			if (wingParts[0]->animationTime > wingParts[0]->maxTime)
				wingParts[0]->animationTime = wingParts[0]->maxTime;

			for (size_t i = 1; i < wingParts.size(); i++)
				wingParts[i]->animationTime = wingParts[0]->animationTime;
		}
	}
	else
	{
		if (wingParts[0]->animationTime != wingParts[0]->minTime)
		{
			wingParts[0]->animationTime -= 0.05f;
			if (wingParts[0]->animationTime < wingParts[0]->minTime)
				wingParts[0]->animationTime = wingParts[0]->minTime;

			for (size_t i = 1; i < wingParts.size(); i++)
				wingParts[i]->animationTime = wingParts[0]->animationTime;
		}
	}
}