#pragma once
#include "FlightMovement.h"


struct AircraftMetaData
{
    std::string modelFilePathA;
    std::wstring modelFilePath;
    std::string gearModelfilePathA;
    std::wstring gearModelfilePath;
    struct FlightMovement::FlightSpec flightSpec;
    Vector3 cameraTrdViewOffset;
    Vector3 cameraFstViewOffset;

};


struct F15EMetaData final : public AircraftMetaData
{
    F15EMetaData();
};

struct F16CMetaData final : public AircraftMetaData
{
    F16CMetaData();
};
/*
struct F15EMetaData
{
    std::string filePathA;
    std::wstring filePath;
    FlightMovement::FlightSpec flightSpec;

    F15EMetaData()
    {
        filePath = L"D:\\PJ\\C++\\Class\\JusinDx3D\\JusinDx3DClient\\Bin\\f15e.model";
        filePathA = "D:\\PJ\\C++\\Class\\JusinDx3D\\JusinDx3DClient\\Bin\\f15e.model";
        flightSpec.airbreakPower = 0.0f;
        flightSpec.dragCoefficient = 0.05f;
        flightSpec.liftCoefficient = 0.05f;
        flightSpec.landingDragCoefficient = 1.0f;
        flightSpec.landingLiftCoefficient = 1.0f;
        flightSpec.maneuverSpeed = Vector3{ 1.0f,1.0f,1.0f };
        flightSpec.mass = 100.0f;
        flightSpec.EnginePower = 3000.0f;
        flightSpec.stallSpeed = 330.0f;
        flightSpec.criticalSpeed = 2000.0f;
    }

};
*/