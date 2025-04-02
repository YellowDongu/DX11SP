#include "pch.h"
#include "F15EHandler.h"

F15EHandler::F15EHandler(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : AircraftBoneHandler(dxDevice, dxDeviceContext)
{
}

F15EHandler::F15EHandler(const F15EHandler& other) : AircraftBoneHandler(other)
{
}

void F15EHandler::Free(void)
{
}

F15EHandler* F15EHandler::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, Engine::Model* F15E, Engine::Model* F15ELandingGear)
{
	F15EHandler* newInstance = new F15EHandler(dxDevice, dxDeviceContext);
	newInstance->model = F15E;
	newInstance->LinkModels(F15E, F15ELandingGear);

	if (FAILED(newInstance->Start()))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}

	return newInstance;
}

#define defineBoneSet(boneSets, targetStruct, name, nodeIndex, boneIndexNumber)	\
targetStruct.SetName = name;											\
targetStruct.animationNodeIndex = nodeIndex;							\
targetStruct.boneIndex = boneIndexNumber;								\
boneSets[name] = targetStruct;											\

HRESULT F15EHandler::Start(void)
{
	boneSets.clear(); gearSets.clear();
	AircraftBoneHandler::CreateBoneSets(boneSets, body, body->DefaultAnimation());
	AircraftBoneHandler::CreateBoneSets(gearSets, landingGear, landingGear->DefaultAnimation());

	#pragma region boneInfomation
	//Engine::BoneSet newStruct;
	//newStruct.animationTime = 0;
	//newStruct.minTime = 0;
	//newStruct.maxTime = 100;
	//newStruct.animationName = "Take001";
	//newStruct.currentPositionFrame = 0;
	//newStruct.currentRotationFrame = 0;
	//newStruct.currentScaleFrame = 0;
	// 
	//defineBoneSet(boneSets, newStruct, "AirBrake_01", 0, 8);
	//defineBoneSet(boneSets, newStruct, "AirBrake_02", 1, 3);
	//defineBoneSet(boneSets, newStruct, "AirBrake_03", 2, 19);
	//
	//defineBoneSet(boneSets, newStruct, "AirIntake_01", 3, 9);
	//defineBoneSet(boneSets, newStruct, "AirIntake_02", 4, 13);
	//defineBoneSet(boneSets, newStruct, "AirIntake_03", 5, 10);
	//defineBoneSet(boneSets, newStruct, "AirIntake_04", 6, 11);
	//defineBoneSet(boneSets, newStruct, "AirIntake_05", 7, 12);
	//defineBoneSet(boneSets, newStruct, "AirIntake_06", 8, 14);
	//defineBoneSet(boneSets, newStruct, "AirIntake_07", 9, 15);
	//defineBoneSet(boneSets, newStruct, "AirIntake_08", 10, 16);
	//defineBoneSet(boneSets, newStruct, "AirIntake_09", 11, 17);
	//
	//defineBoneSet(boneSets, newStruct, "Body", 12, 0);
	//
	//defineBoneSet(boneSets, newStruct, "Flap_01", 13, 4);
	//defineBoneSet(boneSets, newStruct, "Flap_02", 14, 5);
	//
	//defineBoneSet(boneSets, newStruct, "L_Aileron", 15, 1);
	//defineBoneSet(boneSets, newStruct, "L_Elevator", 16, 18);
	//defineBoneSet(boneSets, newStruct, "R_Aileron", 17, 2);
	//defineBoneSet(boneSets, newStruct, "R_Elevator", 18, 20);
	//defineBoneSet(boneSets, newStruct, "Rudder_01", 19, 6);
	//defineBoneSet(boneSets, newStruct, "Rudder_02", 20, 7);



	//defineBoneSet(gearSets, newStruct, "Tire_01", 32, 0);
	//defineBoneSet(gearSets, newStruct, "LandingGear_02", 6, 1);
	//defineBoneSet(gearSets, newStruct, "LandingGear_03", 7, 2);
	//defineBoneSet(gearSets, newStruct, "LandingGear_04", 8, 3);
	//defineBoneSet(gearSets, newStruct, "LandingGear_05", 9, 4);
	//defineBoneSet(gearSets, newStruct, "LandingGear_07", 11, 5);
	//defineBoneSet(gearSets, newStruct, "L_Suspension_02", 3, 6);
	//defineBoneSet(gearSets, newStruct, "L_Suspension_03", 4, 7);
	//defineBoneSet(gearSets, newStruct, "L_Suspension_01", 2, 8);
	//defineBoneSet(gearSets, newStruct, "LandingGear_08", 12, 9);
	//defineBoneSet(gearSets, newStruct, "LandingGear_09", 13, 10);
	//defineBoneSet(gearSets, newStruct, "LandingGear_10", 14, 11);
	//defineBoneSet(gearSets, newStruct, "Tire_02", 33, 12);
	//defineBoneSet(gearSets, newStruct, "LandingGear_11", 15, 13);
	//defineBoneSet(gearSets, newStruct, "LandingGear_12", 16, 14);
	//defineBoneSet(gearSets, newStruct, "R_Suspension_02", 30, 15);
	//defineBoneSet(gearSets, newStruct, "R_Suspension_03", 31, 16);
	//defineBoneSet(gearSets, newStruct, "R_Suspension_01", 29, 17);
	//defineBoneSet(gearSets, newStruct, "LandingGear_13", 17, 18);
	//defineBoneSet(gearSets, newStruct, "LandingGear_14", 18, 19);
	//defineBoneSet(gearSets, newStruct, "LandingGear_15", 19, 20);
	//defineBoneSet(gearSets, newStruct, "Tire_03", 34, 21);
	//defineBoneSet(gearSets, newStruct, "LandingGear_16", 20, 22);
	//defineBoneSet(gearSets, newStruct, "LandingGear_01", 5, 23);
	//defineBoneSet(gearSets, newStruct, "F_Suspension", 1, 24);
	//defineBoneSet(gearSets, newStruct, "LandingGear_06", 10, 25);
	//defineBoneSet(gearSets, newStruct, "LandingGear_17", 21, 26);
	//defineBoneSet(gearSets, newStruct, "LandingGear_18", 22, 27);
	//defineBoneSet(gearSets, newStruct, "LandingGear_19", 23, 28);
	//defineBoneSet(gearSets, newStruct, "LandingGear_20", 24, 29);
	//defineBoneSet(gearSets, newStruct, "LandingGear_21", 25, 30);
	//defineBoneSet(gearSets, newStruct, "LandingGear_22", 26, 31);
	//defineBoneSet(gearSets, newStruct, "LandingGear_23", 27, 32);
	//defineBoneSet(gearSets, newStruct, "LandingGear_24", 28, 33);

	//landingGearParts.push_back(&gearSets.find("Tire_01")->second);
	//landingGearParts.push_back(&gearSets.find("LandingGear_02")->second);
	//landingGearParts.push_back(&gearSets.find("LandingGear_03")->second);
	//landingGearParts.push_back(&gearSets.find("LandingGear_04")->second);
	//landingGearParts.push_back(&gearSets.find("LandingGear_05")->second);
	//landingGearParts.push_back(&gearSets.find("LandingGear_07")->second);
	//landingGearParts.push_back(&gearSets.find("L_Suspension_02")->second);
	//landingGearParts.push_back(&gearSets.find("L_Suspension_03")->second);
	//landingGearParts.push_back(&gearSets.find("L_Suspension_01")->second);
	//landingGearParts.push_back(&gearSets.find("LandingGear_08")->second);
	//landingGearParts.push_back(&gearSets.find("LandingGear_09")->second);
	//landingGearParts.push_back(&gearSets.find("LandingGear_10")->second);
	//landingGearParts.push_back(&gearSets.find("Tire_02")->second);
	//landingGearParts.push_back(&gearSets.find("LandingGear_11")->second);
	//landingGearParts.push_back(&gearSets.find("LandingGear_12")->second);
	//landingGearParts.push_back(&gearSets.find("R_Suspension_02")->second);
	//landingGearParts.push_back(&gearSets.find("R_Suspension_03")->second);
	//landingGearParts.push_back(&gearSets.find("R_Suspension_01")->second);
	//landingGearParts.push_back(&gearSets.find("LandingGear_13")->second);
	//landingGearParts.push_back(&gearSets.find("LandingGear_14")->second);
	//landingGearParts.push_back(&gearSets.find("LandingGear_15")->second);
	//landingGearParts.push_back(&gearSets.find("Tire_03")->second);
	//landingGearParts.push_back(&gearSets.find("LandingGear_16")->second);
	//landingGearParts.push_back(&gearSets.find("LandingGear_01")->second);
	//landingGearParts.push_back(&gearSets.find("F_Suspension")->second);
	//landingGearParts.push_back(&gearSets.find("LandingGear_06")->second);
	//landingGearParts.push_back(&gearSets.find("LandingGear_17")->second);
	//landingGearParts.push_back(&gearSets.find("LandingGear_18")->second);
	//landingGearParts.push_back(&gearSets.find("LandingGear_19")->second);
	//landingGearParts.push_back(&gearSets.find("LandingGear_20")->second);
	//landingGearParts.push_back(&gearSets.find("LandingGear_21")->second);
	//landingGearParts.push_back(&gearSets.find("LandingGear_22")->second);
	//landingGearParts.push_back(&gearSets.find("LandingGear_23")->second);
	//landingGearParts.push_back(&gearSets.find("LandingGear_24")->second);

	#pragma endregion


	aileronLParts.push_back(&boneSets.find("L_Aileron")->second);
	aileronRParts.push_back(&boneSets.find("R_Aileron")->second);

	elevatorParts.push_back(&boneSets.find("L_Elevator")->second);
	elevatorParts.push_back(&boneSets.find("R_Elevator")->second);

	rudderParts.push_back(&boneSets.find("Rudder_01")->second);
	rudderParts.push_back(&boneSets.find("Rudder_02")->second);

	flapParts.push_back(&boneSets.find("Flap_01")->second);
	flapParts.push_back(&boneSets.find("Flap_02")->second);

	airbreakParts.push_back(&boneSets.find("AirBrake_01")->second);
	airbreakParts.push_back(&boneSets.find("AirBrake_02")->second);
	airbreakParts.push_back(&boneSets.find("AirBrake_03")->second);

	airIntakeParts.push_back(&boneSets.find("AirIntake_01")->second);
	airIntakeParts.push_back(&boneSets.find("AirIntake_02")->second);
	airIntakeParts.push_back(&boneSets.find("AirIntake_03")->second);
	airIntakeParts.push_back(&boneSets.find("AirIntake_04")->second);
	airIntakeParts.push_back(&boneSets.find("AirIntake_05")->second);
	airIntakeParts.push_back(&boneSets.find("AirIntake_06")->second);
	airIntakeParts.push_back(&boneSets.find("AirIntake_07")->second);
	airIntakeParts.push_back(&boneSets.find("AirIntake_08")->second);
	airIntakeParts.push_back(&boneSets.find("AirIntake_09")->second);

	for (auto& gearSet : gearSets)
		landingGearParts.push_back(&gearSet.second);


	return S_OK;
}
#undef defineBoneSet

//static float testvalue = 0.0f;
void F15EHandler::Update(void)
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
	if(Input()->getButtonDown(KeyType::G))
		landingGearApply = !landingGearApply;
	LandingGear();
}

void F15EHandler::FixedUpdate(void)
{
}

void F15EHandler::LateUpdate(void)
{
	UpdateBones(boneSets, body);
	UpdateBones(gearSets, landingGear);
}

void F15EHandler::Flap(float inputValue)
{
	if(flapParts.empty())
		return;

	if (abs(inputValue) > 1.0f)
		inputValue /= abs(inputValue);
	else if(inputValue < 0.0f)
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

void F15EHandler::AirBrake(float inputValue)
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

void F15EHandler::AirIntake(float inputValue)
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
	else if(airIntakeParts[0]->animationTime != airIntakeParts[0]->minTime)
	{
		inputValue = 1.0f;
		airIntakeParts[0]->animationTime -= inputValue;
		if (airIntakeParts[0]->animationTime < airIntakeParts[0]->minTime)
			airIntakeParts[0]->animationTime = airIntakeParts[0]->minTime;
	}

	for (size_t i = 1; i < airIntakeParts.size(); i++)
		airIntakeParts[i]->animationTime = airIntakeParts[0]->animationTime;
}

void F15EHandler::LandingGear(void)
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

Engine::Component* F15EHandler::Clone(void)
{
	return new F15EHandler(*this);
}