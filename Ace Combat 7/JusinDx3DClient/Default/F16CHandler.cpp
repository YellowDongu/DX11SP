#include "pch.h"
#include "F16CHandler.h"


F16CHandler::F16CHandler(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : AircraftBoneHandler(dxDevice, dxDeviceContext)
{
}

F16CHandler::F16CHandler(const F16CHandler& other) : AircraftBoneHandler(other)
{
}

void F16CHandler::Free(void)
{
}

F16CHandler* F16CHandler::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, Engine::Model* F16C, Engine::Model* F16CGear)
{
	F16CHandler* newInstance = new F16CHandler(dxDevice, dxDeviceContext);
	newInstance->model = F16C;
	newInstance->LinkModels(F16C, F16CGear);

	if (FAILED(newInstance->Start()))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}

	return newInstance;
}

F16CHandler* F16CHandler::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext)
{
	F16CHandler* newInstance = new F16CHandler(dxDevice, dxDeviceContext);

	return newInstance;
}

#define defineBoneSet(targetStruct, name, nodeIndex, boneIndexNumber)	\
targetStruct.SetName = name;											\
targetStruct.animationNodeIndex = nodeIndex;							\
targetStruct.boneIndex = boneIndexNumber;								\
boneSets[name] = targetStruct;											\


HRESULT F16CHandler::Start(void)
{
	boneSets.clear(); gearSets.clear();
	AircraftBoneHandler::CreateBoneSets(boneSets, body, body->DefaultAnimation());
	AircraftBoneHandler::CreateBoneSets(gearSets, landingGear, landingGear->DefaultAnimation());

	//Engine::BoneSet newStruct;
	//newStruct.animationTime = 0;
	//newStruct.minTime = 0;
	//newStruct.maxTime = 100;
	//newStruct.animationName = "Take001";
	//newStruct.currentPositionFrame = 0;
	//newStruct.currentRotationFrame = 0;
	//newStruct.currentScaleFrame = 0;
	//
	//defineBoneSet(newStruct, "Body", 16, 0);
	//defineBoneSet(newStruct, "AirBrake_01", 0, 1);
	//defineBoneSet(newStruct, "AirBrake_02", 1, 2);
	//defineBoneSet(newStruct, "AirBrake_03", 2, 3);
	//defineBoneSet(newStruct, "AirBrake_04", 3, 4);
	//defineBoneSet(newStruct, "AirBrake_05", 4, 5);
	//defineBoneSet(newStruct, "AirBrake_06", 5, 6);
	//defineBoneSet(newStruct, "AirBrake_07", 6, 7);
	//defineBoneSet(newStruct, "AirBrake_08", 7, 8);
	//defineBoneSet(newStruct, "AirBrake_09", 8, 9);
	//defineBoneSet(newStruct, "AirBrake_10", 9, 10);
	//defineBoneSet(newStruct, "AirBrake_11", 10, 11);
	//defineBoneSet(newStruct, "AirBrake_12", 11, 12);
	//defineBoneSet(newStruct, "AirBrake_13", 12, 13);
	//defineBoneSet(newStruct, "AirBrake_14", 13, 14);
	//defineBoneSet(newStruct, "AirBrake_15", 14, 15);
	//defineBoneSet(newStruct, "AirBrake_16", 15, 16);
	//
	//defineBoneSet(newStruct, "R_Tail", 22, 17);
	//defineBoneSet(newStruct, "L_Tail", 20, 18);
	//
	//defineBoneSet(newStruct, "Rudder", 23, 19);
	//
	//defineBoneSet(newStruct, "R_Aileron", 21, 20);
	//defineBoneSet(newStruct, "L_Aileron", 19, 21);
	//
	//defineBoneSet(newStruct, "Front_Flap_02", 18, 22);
	//defineBoneSet(newStruct, "Front_Flap_01", 17, 23);


	aileronLParts.push_back(&boneSets.find("L_Aileron")->second);
	aileronRParts.push_back(&boneSets.find("R_Aileron")->second);

	elevatorParts.push_back(&boneSets.find("L_Tail")->second);
	elevatorParts.push_back(&boneSets.find("R_Tail")->second);

	rudderParts.push_back(&boneSets.find("Rudder")->second);

	flapParts.push_back(&boneSets.find("Front_Flap_01")->second);
	flapParts.push_back(&boneSets.find("Front_Flap_02")->second);

	airbreakParts.push_back(&boneSets.find("AirBrake_01")->second);
	airbreakParts.push_back(&boneSets.find("AirBrake_02")->second);
	airbreakParts.push_back(&boneSets.find("AirBrake_03")->second);
	airbreakParts.push_back(&boneSets.find("AirBrake_04")->second);
	airbreakParts.push_back(&boneSets.find("AirBrake_05")->second);
	airbreakParts.push_back(&boneSets.find("AirBrake_06")->second);
	airbreakParts.push_back(&boneSets.find("AirBrake_07")->second);
	airbreakParts.push_back(&boneSets.find("AirBrake_08")->second);
	airbreakParts.push_back(&boneSets.find("AirBrake_09")->second);
	airbreakParts.push_back(&boneSets.find("AirBrake_10")->second);
	airbreakParts.push_back(&boneSets.find("AirBrake_11")->second);
	airbreakParts.push_back(&boneSets.find("AirBrake_12")->second);
	airbreakParts.push_back(&boneSets.find("AirBrake_13")->second);
	airbreakParts.push_back(&boneSets.find("AirBrake_14")->second);
	airbreakParts.push_back(&boneSets.find("AirBrake_15")->second);
	airbreakParts.push_back(&boneSets.find("AirBrake_16")->second);

	for (auto& gearSet : gearSets)
		landingGearParts.push_back(&gearSet.second);

	return S_OK;
}
HRESULT F16CHandler::Awake(void)
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

	elevatorParts.push_back(&boneSets.find("L_Tail")->second);
	elevatorParts.push_back(&boneSets.find("R_Tail")->second);

	rudderParts.push_back(&boneSets.find("Rudder")->second);

	flapParts.push_back(&boneSets.find("Front_Flap_01")->second);
	flapParts.push_back(&boneSets.find("Front_Flap_02")->second);

	airbreakParts.push_back(&boneSets.find("AirBrake_01")->second);
	airbreakParts.push_back(&boneSets.find("AirBrake_02")->second);
	airbreakParts.push_back(&boneSets.find("AirBrake_03")->second);
	airbreakParts.push_back(&boneSets.find("AirBrake_04")->second);
	airbreakParts.push_back(&boneSets.find("AirBrake_05")->second);
	airbreakParts.push_back(&boneSets.find("AirBrake_06")->second);
	airbreakParts.push_back(&boneSets.find("AirBrake_07")->second);
	airbreakParts.push_back(&boneSets.find("AirBrake_08")->second);
	airbreakParts.push_back(&boneSets.find("AirBrake_09")->second);
	airbreakParts.push_back(&boneSets.find("AirBrake_10")->second);
	airbreakParts.push_back(&boneSets.find("AirBrake_11")->second);
	airbreakParts.push_back(&boneSets.find("AirBrake_12")->second);
	airbreakParts.push_back(&boneSets.find("AirBrake_13")->second);
	airbreakParts.push_back(&boneSets.find("AirBrake_14")->second);
	airbreakParts.push_back(&boneSets.find("AirBrake_15")->second);
	airbreakParts.push_back(&boneSets.find("AirBrake_16")->second);

	for (auto& gearSet : gearSets)
		landingGearParts.push_back(&gearSet.second);

	return S_OK;
}
#undef defineBoneSet

//static float testvalue = 0.0f;
void F16CHandler::Update(void)
{
	if (yoke != nullptr)
	{
		Elevator(yoke->x);
		Rudder(yoke->y);
		Aileron(yoke->z);
	}
	else
	{
		#define buttonInput(key) Input()->getButton(KeyType::key)
		Rudder(static_cast<FLOAT>(buttonInput(Z)) - static_cast<FLOAT>(buttonInput(X)));
		Aileron(static_cast<FLOAT>(buttonInput(C)) - static_cast<FLOAT>(buttonInput(B)));
		Elevator(static_cast<FLOAT>(buttonInput(F)) - static_cast<FLOAT>(buttonInput(V)));
		#undef buttonInput
	}

	Flap(static_cast<FLOAT>(Input()->getButton(KeyType::M)));
	AirBrake(static_cast<FLOAT>(Input()->getButton(KeyType::S)));
	AirIntake(static_cast<FLOAT>(Input()->getButton(KeyType::N)));
	if (Input()->getButtonDown(KeyType::G))
		landingGearApply = !landingGearApply;
	LandingGear();
}

void F16CHandler::FixedUpdate(void)
{
}

void F16CHandler::LateUpdate(void)
{
	UpdateBones(boneSets, body);
	UpdateBones(gearSets, landingGear);
}


void F16CHandler::Flap(float inputValue)
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
		flapParts[0]->animationTime = inputValue;
		if (flapParts[0]->animationTime < flapParts[0]->minTime)
			flapParts[0]->animationTime = flapParts[0]->minTime;
	}
	else if (flapParts[0]->animationTime != flapParts[0]->minTime)
	{
		inputValue = 1.0f;
		flapParts[0]->animationTime -= inputValue;
		if (flapParts[0]->animationTime > flapParts[0]->maxTime)
			flapParts[0]->animationTime = flapParts[0]->maxTime;
	}

	for (size_t i = 1; i < flapParts.size(); i++)
		flapParts[i]->animationTime = flapParts[0]->animationTime;
}

void F16CHandler::AirBrake(float inputValue)
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

void F16CHandler::AirIntake(float inputValue)
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

void F16CHandler::LandingGear(void)
{
	if (landingGearParts.empty())
		return;

	landingGearParts[0]->maxTime = 75.0f;
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

Engine::Component* F16CHandler::Clone(void)
{
	return new F16CHandler(*this);
}