#include "pch.h"
#include "AircraftMetaData.h"

F15EMetaData::F15EMetaData()
{
    AircraftModelName = L"F15E";
    modelFilePath = L"../Bin/Resources/Vehicles/Aircrafts/f15e/f15e.model";
    modelFilePathA = "../Bin/Resources/Vehicles/Aircrafts/f15e/f15e.fbx";
    gearModelfilePathA = "../Bin/Resources/Vehicles/Aircrafts/f15e/f15eGear.fbx";
    gearModelfilePath = L"../Bin/Resources/Vehicles/Aircrafts/f15e/f15eGear.model";
    BoneHandlerName = L"F15EHandler";
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


    // notYet
    colliderCenter = Vector3{ 0.0f, 0.1f, 0.25f };
    colliderExtent = Vector3{ 0.25f, 0.15f, 1.0f };


    WingTrailRightOffset = Vector3{ 0.5f, 0.0f, 0.0f };
    WingTrailLeftOffset = Vector3{ -0.5f, 0.0f, 0.0f };
    WingTrailWidthOffset = 1.0f;
}

F16CMetaData::F16CMetaData()
{
    AircraftModelName = L"F16C";
    modelFilePath = L"../Bin/Resources/Vehicles/Aircrafts/f16c/f16c.model";
    modelFilePathA = "../Bin/Resources/Vehicles/Aircrafts/f16c/f16c.fbx";
    gearModelfilePathA = "../Bin/Resources/Vehicles/Aircrafts/f16c/f16cGear.fbx";
    gearModelfilePath = L"../Bin/Resources/Vehicles/Aircrafts/f16c/f16cGear.model";
    BoneHandlerName = L"F16CHandler";
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


    // notYet
    colliderCenter = Vector3{ 0.0f, 0.1f, 0.25f };
    colliderExtent = Vector3{ 0.25f, 0.15f, 1.0f };


    WingTrailRightOffset = Vector3{ 0.5f, 0.0f, 0.0f };
    WingTrailLeftOffset = Vector3{ -0.5f, 0.0f, 0.0f };
    WingTrailWidthOffset = 0.0f;
}

F14DMetaData::F14DMetaData()
{
    AircraftModelName = L"F14D";
    modelFilePath = L"../Bin/Resources/Vehicles/Aircrafts/f14d/f14d.model";
    modelFilePathA = "../Bin/Resources/Vehicles/Aircrafts/f14d/f14d.fbx";
    gearModelfilePathA = "../Bin/Resources/Vehicles/Aircrafts/f14d/f14dGear.fbx";
    gearModelfilePath = L"../Bin/Resources/Vehicles/Aircrafts/f14d/f14dGear.model";
    BoneHandlerName = L"F14DHandler";
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
    cameraFstViewOffset = Vector3{ 0.0f,0.1f, 0.70f };


    // notYet
    colliderCenter = Vector3{ 0.0f, 0.1f, 0.25f };
    colliderExtent = Vector3{ 0.25f, 0.15f, 1.0f };


    WingTrailRightOffset = Vector3{ 0.5f, 0.0f, 0.0f };
    WingTrailLeftOffset = Vector3{ -0.5f, 0.0f, 0.0f };
    WingTrailWidthOffset = 1.0f;
}

SU33MetaData::SU33MetaData()
{
    AircraftModelName = L"SU33";
    modelFilePath = L"../Bin/Resources/Vehicles/Aircrafts/su33/su33.model";
    modelFilePathA = "../Bin/Resources/Vehicles/Aircrafts/su33/su33.fbx";
    gearModelfilePathA = "../Bin/Resources/Vehicles/Aircrafts/su33/su33Gear.fbx";
    gearModelfilePath = L"../Bin/Resources/Vehicles/Aircrafts/su33/su33Gear.model";
    BoneHandlerName = L"SU33Handler";
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
    cameraTrdViewOffset = Vector3::zero();
    cameraFstViewOffset = Vector3::zero();


    // notYet
    colliderCenter = Vector3{ 0.0f, 0.1f, 0.25f };
    colliderExtent = Vector3{ 0.25f, 0.15f, 1.0f };


    WingTrailRightOffset = Vector3{ 0.5f, 0.0f, 0.0f };
    WingTrailLeftOffset = Vector3{ -0.5f, 0.0f, 0.0f };
    WingTrailWidthOffset = 1.0f;
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
    cameraTrdViewOffset = Vector3::zero();
    cameraFstViewOffset = Vector3::zero();

    // notYet
    colliderCenter = Vector3{ 0.0f, 0.1f, 0.25f };
    colliderExtent = Vector3{ 0.25f, 0.15f, 1.0f };


    WingTrailRightOffset = Vector3{ 0.5f, 0.0f, 0.0f };
    WingTrailLeftOffset = Vector3{ -0.5f, 0.0f, 0.0f };
    WingTrailWidthOffset = 1.0f;
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
    cameraTrdViewOffset = Vector3::zero();
    cameraFstViewOffset = Vector3::zero();

    // notYet
    colliderCenter = Vector3{ 0.0f, 0.1f, 0.25f };
    colliderExtent = Vector3{ 0.25f, 0.15f, 1.0f };


    WingTrailRightOffset = Vector3{ 0.5f, 0.0f, 0.0f };
    WingTrailLeftOffset = Vector3{ -0.5f, 0.0f, 0.0f };
    WingTrailWidthOffset = 1.0f;
}

SAMMetaData::SAMMetaData()
{
    AircraftModelName = L"SAM";
    modelFilePath = L"../Bin/Resources/Vehicles/Ground/geck/geck.model";
    modelFilePathA = "../Bin/Resources/Vehicles/Ground/geck/geck.FBX";
    gearModelfilePathA = "";
    gearModelfilePath = L"";
    flightSpec.airbreakPower = 0.0f;
    flightSpec.dragCoefficient = 0.0f;
    flightSpec.liftCoefficient = 0.0f;
    flightSpec.landingDragCoefficient = 0.0f;
    flightSpec.landingLiftCoefficient = 0.0f;
    flightSpec.maneuverSpeed = Vector3::zero();
    flightSpec.mass = 0.0f;
    flightSpec.EnginePower = 0.0f;
    flightSpec.stallSpeed = 0.0f;
    flightSpec.criticalSpeed = 0.0f;
    cameraTrdViewOffset = Vector3::zero();
    cameraFstViewOffset = Vector3::zero();

    // notYet
    colliderCenter = Vector3::zero();
    colliderExtent = Vector3::zero();


    WingTrailRightOffset = Vector3::zero();
    WingTrailLeftOffset = Vector3::zero();
    WingTrailWidthOffset = 0.0f;
}

AAMetaData::AAMetaData()
{
    AircraftModelName = L"AAGUN";
    modelFilePath = L"../Bin/Resources/Vehicles/Ground/gepa/gepa.model";
    modelFilePathA = "../Bin/Resources/Vehicles/Ground/gepa/gepa.FBX";
    gearModelfilePathA = "";
    gearModelfilePath = L"";
    flightSpec.airbreakPower = 0.0f;
    flightSpec.dragCoefficient = 0.0f;
    flightSpec.liftCoefficient = 0.0f;
    flightSpec.landingDragCoefficient = 0.0f;
    flightSpec.landingLiftCoefficient = 0.0f;
    flightSpec.maneuverSpeed = Vector3::zero();
    flightSpec.mass = 0.0f;
    flightSpec.EnginePower = 0.0f;
    flightSpec.stallSpeed = 0.0f;
    flightSpec.criticalSpeed = 0.0f;
    cameraTrdViewOffset = Vector3::zero();
    cameraFstViewOffset = Vector3::zero();

    // notYet
    colliderCenter = Vector3::zero();
    colliderExtent = Vector3::zero();


    WingTrailRightOffset = Vector3::zero();
    WingTrailLeftOffset = Vector3::zero();
    WingTrailWidthOffset = 0.0f;
}
