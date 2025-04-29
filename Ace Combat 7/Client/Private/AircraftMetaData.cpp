#include "pch.h"
#include "AircraftMetaData.h"

F15EMetaData::F15EMetaData()
{
    AircraftModelName = L"F15E";
    modelFilePath = L"../Bin/f15e.model";
    modelFilePathA = "../Bin/f15e.fbx";
    gearModelfilePathA = "../Bin/f15eGear.fbx";
    gearModelfilePath = L"../Bin/f15eGear.model";
    flightSpec.airbreakPower = 500.0f;
    flightSpec.dragCoefficient = 0.05f;
    flightSpec.liftCoefficient = 0.05f;
    flightSpec.landingDragCoefficient = 1.0f;
    flightSpec.landingLiftCoefficient = 1.0f;
    flightSpec.maneuverSpeed = Vector3{ 1.0f,0.1f,1.5f };
    flightSpec.mass = 100.0f;
    flightSpec.EnginePower = 3000.0f;
    flightSpec.stallSpeed = 330.0f;
    flightSpec.criticalSpeed = 2000.0f;
    cameraTrdViewOffset = Vector3{ 0.0f ,0.4f, -1.75f };
    cameraFstViewOffset = Vector3{ 0.0f,0.12f, 0.88f };
    colliderCenter = Vector3{ 0.0f, 0.1f, 0.25f };
    colliderExtent = Vector3{ 0.25f, 0.15f, 1.0f };
}

F16CMetaData::F16CMetaData()
{
    AircraftModelName = L"F16C";
    modelFilePath = L"../Bin/f16c/f16c.model";
    modelFilePathA = "../Bin/f16c/f16c.fbx";
    gearModelfilePathA = "../Bin/f15eGear.fbx";
    gearModelfilePath = L"../Bin/f15eGear.model";
    flightSpec.airbreakPower = 500.0f;
    flightSpec.dragCoefficient = 0.05f;
    flightSpec.liftCoefficient = 0.05f;
    flightSpec.landingDragCoefficient = 1.0f;
    flightSpec.landingLiftCoefficient = 1.0f;
    flightSpec.maneuverSpeed = Vector3{ 1.0f,0.1f,1.75f };
    flightSpec.mass = 100.0f;
    flightSpec.EnginePower = 2250.0f;
    flightSpec.stallSpeed = 330.0f;
    flightSpec.criticalSpeed = 1800.0f;
    cameraTrdViewOffset = Vector3{ 0.0f ,0.4f, -1.5f };
    cameraFstViewOffset = Vector3{ 0.0f,0.09f, 0.53f };
    colliderCenter = Vector3{ 0.0f, 0.1f, 0.25f };
    colliderExtent = Vector3{ 0.25f, 0.15f, 1.0f };
}

F14DMetaData::F14DMetaData()
{
    AircraftModelName = L"F14D";
    modelFilePath = L"../Bin/Resources/Vehicles/Aircrafts/f14d/f14d.model";
    modelFilePathA = "../Bin/Resources/Vehicles/Aircrafts/f14d/f14d.fbx";
    gearModelfilePathA = "../Bin/Resources/Vehicles/Aircrafts/f14d/f14dGear.fbx";
    gearModelfilePath = L"../Bin/Resources/Vehicles/Aircrafts/f14d/f14dGear.model";
    flightSpec.airbreakPower = 500.0f;
    flightSpec.dragCoefficient = 0.05f;
    flightSpec.liftCoefficient = 0.05f;
    flightSpec.landingDragCoefficient = 1.0f;
    flightSpec.landingLiftCoefficient = 1.0f;
    flightSpec.maneuverSpeed = Vector3{ 1.0f,0.1f,1.5f };
    flightSpec.mass = 100.0f;
    flightSpec.EnginePower = 3000.0f;
    flightSpec.stallSpeed = 330.0f;
    flightSpec.criticalSpeed = 2000.0f;
    cameraTrdViewOffset = Vector3{ 0.0f ,0.4f, -1.75f };
    cameraFstViewOffset = Vector3{ 0.0f,0.1f, 0.84f };
    colliderCenter = Vector3{ 0.0f, 0.1f, 0.25f };
    colliderExtent = Vector3{ 0.25f, 0.15f, 1.0f };
}

SU33MetaData::SU33MetaData()
{
    AircraftModelName = L"SU33";
    modelFilePath = L"../Bin/Resources/Vehicles/Aircrafts/su33/su33.model";
    modelFilePathA = "../Bin/Resources/Vehicles/Aircrafts/su33/su33.fbx";
    gearModelfilePathA = "../Bin/Resources/Vehicles/Aircrafts/su33/su33Gear.fbx";
    gearModelfilePath = L"../Bin/Resources/Vehicles/Aircrafts/su33/su33Gear.model";
    flightSpec.airbreakPower = 500.0f;
    flightSpec.dragCoefficient = 0.05f;
    flightSpec.liftCoefficient = 0.05f;
    flightSpec.landingDragCoefficient = 1.0f;
    flightSpec.landingLiftCoefficient = 1.0f;
    flightSpec.maneuverSpeed = Vector3{ 1.0f,0.1f,1.5f };
    flightSpec.mass = 100.0f;
    flightSpec.EnginePower = 3000.0f;
    flightSpec.stallSpeed = 330.0f;
    flightSpec.criticalSpeed = 2000.0f;
    cameraTrdViewOffset = Vector3{ 0.0f ,0.4f, -1.75f };
    cameraFstViewOffset = Vector3{ 0.0f,0.1f, 0.84f };
    colliderCenter = Vector3{ 0.0f, 0.1f, 0.25f };
    colliderExtent = Vector3{ 0.25f, 0.15f, 1.0f };
}

TU160MetaData::TU160MetaData()
{
    AircraftModelName = L"TU160";
    modelFilePath = L"../Bin/Resources/Vehicles/Aircrafts/t160/t160.model";
    modelFilePathA = "../Bin/Resources/Vehicles/Aircrafts/t160/t160.fbx";
    gearModelfilePathA = "";
    gearModelfilePath = L"";
    flightSpec.airbreakPower = 500.0f;
    flightSpec.dragCoefficient = 0.05f;
    flightSpec.liftCoefficient = 0.05f;
    flightSpec.landingDragCoefficient = 1.0f;
    flightSpec.landingLiftCoefficient = 1.0f;
    flightSpec.maneuverSpeed = Vector3{ 1.0f,0.1f,1.5f };
    flightSpec.mass = 100.0f;
    flightSpec.EnginePower = 3000.0f;
    flightSpec.stallSpeed = 330.0f;
    flightSpec.criticalSpeed = 2000.0f;
    cameraTrdViewOffset = Vector3{ 0.0f ,0.4f, -1.75f };
    cameraFstViewOffset = Vector3{ 0.0f,0.1f, 0.84f };
    colliderCenter = Vector3{ 0.0f, 0.1f, 0.25f };
    colliderExtent = Vector3{ 0.25f, 0.15f, 1.0f };
}

TU95MetaData::TU95MetaData()
{
    AircraftModelName = L"TU95";
    modelFilePath = L"../Bin/Resources/Vehicles/Aircrafts/t095/t095.model";
    modelFilePathA = "../Bin/Resources/Vehicles/Aircrafts/t095/t095.fbx";
    gearModelfilePathA = "";
    gearModelfilePath = L"";
    flightSpec.airbreakPower = 500.0f;
    flightSpec.dragCoefficient = 0.05f;
    flightSpec.liftCoefficient = 0.05f;
    flightSpec.landingDragCoefficient = 1.0f;
    flightSpec.landingLiftCoefficient = 1.0f;
    flightSpec.maneuverSpeed = Vector3{ 1.0f,0.1f,1.5f };
    flightSpec.mass = 100.0f;
    flightSpec.EnginePower = 3000.0f;
    flightSpec.stallSpeed = 330.0f;
    flightSpec.criticalSpeed = 2000.0f;
    cameraTrdViewOffset = Vector3{ 0.0f ,0.4f, -1.75f };
    cameraFstViewOffset = Vector3{ 0.0f,0.1f, 0.84f };
    colliderCenter = Vector3{ 0.0f, 0.1f, 0.25f };
    colliderExtent = Vector3{ 0.25f, 0.15f, 1.0f };
}
