#pragma once

struct FlightSpec
{
    Vector3 maneuverSpeed;
    float EnginePower, airbreakPower, mass;

    float liftCoefficient, dragCoefficient;
    float landingLiftCoefficient, landingDragCoefficient;
    float criticalSpeed; // max ground speed
    float stallSpeed; // stall speed(ground speed)
};

struct AircraftMetaData
{
    std::wstring AircraftModelName;
    std::string modelFilePathA;
    std::wstring modelFilePath;
    std::string gearModelfilePathA;
    std::wstring gearModelfilePath;
    std::wstring BoneHandlerName;
    FlightSpec flightSpec;
    Vector3 cameraTrdViewOffset;
    Vector3 cameraFstViewOffset;

    Vector3 colliderCenter;
    Vector3 colliderExtent;
};

struct PilotInfomation
{
    AircraftMetaData aircraftInfomation;
    UINT allyInfo;//player = 0, ally = 1, enemy = 2
    Vector3 startPosition;
    Vector3 startDirection;
    std::wstring gameObejctName;
};

struct F15EMetaData final : public AircraftMetaData { F15EMetaData(); };
struct F16CMetaData final : public AircraftMetaData { F16CMetaData(); };
struct F14DMetaData final : public AircraftMetaData { F14DMetaData(); };
struct SU33MetaData final : public AircraftMetaData { SU33MetaData(); };
struct TU160MetaData final : public AircraftMetaData { TU160MetaData(); };
struct TU95MetaData final : public AircraftMetaData { TU95MetaData(); };