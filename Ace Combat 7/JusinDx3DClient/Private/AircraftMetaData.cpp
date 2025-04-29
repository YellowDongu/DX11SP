#include "pch.h"
#include "AircraftMetaData.h"

F15EMetaData::F15EMetaData()
{
    modelFilePath = L"../Bin/f15e.model";
    modelFilePathA = "../Bin/f15e.fbx";
    gearModelfilePathA = "../Bin/f15eGear.fbx";
    gearModelfilePath = L"../Bin/f15eGear.model";
    flightSpec.airbreakPower = 0.0f;
    flightSpec.dragCoefficient = 0.05f;
    flightSpec.liftCoefficient = 0.05f;
    flightSpec.landingDragCoefficient = 1.0f;
    flightSpec.landingLiftCoefficient = 1.0f;
    flightSpec.maneuverSpeed = Vector3{ 1.0f,1.0f,1.5f };
    flightSpec.mass = 100.0f;
    flightSpec.EnginePower = 3000.0f;
    flightSpec.stallSpeed = 330.0f;
    flightSpec.criticalSpeed = 2000.0f;
    cameraTrdViewOffset = Vector3{ 0.0f ,0.4f, -1.75f };
    cameraFstViewOffset = Vector3{ 0.0f,0.12f, 0.88f };
}

F16CMetaData::F16CMetaData()
{
    //filePath = L"..\\Bin\\f16c\\f16c.fbx";
    //filePathA = "..\\Bin\\f16c\\f16c.fbx";
    modelFilePath = L"../Bin/f16c/f16c.model";
    modelFilePathA = "../Bin/f16c/f16c.fbx";
    gearModelfilePathA = "../Bin/f16c/f16cGear.fbx";
    gearModelfilePath = L"../Bin/f16c/f16cGear.model";
    flightSpec.airbreakPower = 0.0f;
    flightSpec.dragCoefficient = 0.05f;
    flightSpec.liftCoefficient = 0.05f;
    flightSpec.landingDragCoefficient = 1.0f;
    flightSpec.landingLiftCoefficient = 1.0f;
    flightSpec.maneuverSpeed = Vector3{ 1.0f,1.0f,1.75f };
    flightSpec.mass = 100.0f;
    flightSpec.EnginePower = 2250.0f;
    flightSpec.stallSpeed = 330.0f;
    flightSpec.criticalSpeed = 1800.0f;
    cameraTrdViewOffset = Vector3{ 0.0f ,0.4f, -1.5f };
    cameraFstViewOffset = Vector3{ 0.0f,0.09f, 0.53f };
}
