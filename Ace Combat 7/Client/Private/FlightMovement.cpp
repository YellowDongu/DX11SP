#include "pch.h"
#include "FlightMovement.h"


FlightMovement::FlightMovement(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, Engine::Transform* _transform, FlightSpec& flightSpec) : Engine::Component(dxDevice, dxDeviceContext), transform(_transform), airbreakActive(false), yoke(Vector3::zero()), idleThrottle(0.0f), throttle(0.0f), force(Vector3::zero()), maneuverSpeed(flightSpec.maneuverSpeed), EnginePower(flightSpec.EnginePower), airbreakPower(flightSpec.airbreakPower), mass(flightSpec.mass), liftCoefficient(flightSpec.liftCoefficient), dragCoefficient(flightSpec.dragCoefficient), landingLiftCoefficient(flightSpec.landingLiftCoefficient), landingDragCoefficient(flightSpec.landingDragCoefficient)
,stallSpeed(flightSpec.stallSpeed), criticalSpeed(flightSpec.criticalSpeed), manuverForce(Vector3::zero())
{}

FlightMovement::FlightMovement(const FlightMovement& other) : Engine::Component(other), transform(nullptr), airbreakActive(false), yoke(Vector3::zero()), idleThrottle(other.idleThrottle), throttle(0.0f), force(Vector3::zero()), maneuverSpeed(other.maneuverSpeed), EnginePower(other.EnginePower), airbreakPower(other.airbreakPower), mass(other.mass), liftCoefficient(other.liftCoefficient), dragCoefficient(other.dragCoefficient), landingLiftCoefficient(other.landingLiftCoefficient), landingDragCoefficient(other.landingDragCoefficient)
, stallSpeed(other.stallSpeed), criticalSpeed(other.criticalSpeed), manuverForce(Vector3::zero())
{}

void FlightMovement::Free(void)
{
}

FlightMovement* FlightMovement::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, Engine::Transform* transform, FlightSpec& flightSpec)
{
	FlightMovement* newInstance = new FlightMovement(dxDevice, dxDeviceContext, transform, flightSpec);
	if (FAILED(newInstance->Start()))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}
	return newInstance;
}
void FlightMovement::CalculateMovement(void)
{
	if (throttle > 1.0f) throttle = 1.0f;
	if (throttle < 0.0f) throttle = 0.0f;

	speedFactor = (velocity - stallSpeed) / (criticalSpeed - stallSpeed);
	float newVelocity = (EnginePower * throttle - velocity) - velocity * speedFactor - airbreakPower * static_cast<FLOAT>(airbreakActive);


	gravity = Vector3(0.0f, -1.0f, 0.0f) * 9.8f * (mass * 0.1f);
	Lift = transform->Up().normalize() * (1 + speedFactor) * 9.8f * (mass * 0.1f);
	Vector3 newDirection = gravity + Lift;
	float gravityFactor = newVelocity * (transform->Angle().x / -180.0f);
	velocity += ((newVelocity + gravityFactor) - 1000.0f * speedFactor * std::abs(finalMenuverForce.x)) * 0.00025f;
	//velocity -= velocity * airbreakPower;
	thrust = transform->Forward().normalize() * velocity;

	newDirection += thrust;
	force = newDirection;

	return;
	// Real AeroDynamics Test -> Failed :(

	if (throttle > 2.0f)
		throttle = 2.0f;
	if (throttle < 0.0f)
		throttle = 0.0f;

	float velocityTest = Vector3::dot(transform->Forward(), force);
	Vector3 forward = transform->Forward().normalize();
	//float velocity = force.magnitude();
	if (isnan(velocityTest))
		velocityTest = idleVelocity;


	if (velocityTest == 0.0f)
		velocityTest = idleVelocity;

	gravity = Vector3(0.0f, -1.0f, 0.0f) * 9.8f * mass;
	thrust = forward * EnginePower * throttle;
	Lift = transform->Up().normalize() * liftCoefficient * std::powf( velocityTest, 2.0f);
	Drag = forward * -1.0f * dragCoefficient * std::powf(velocityTest, 2.0f);
	//if (airbreakActive)
	//	Drag *= airbreakPower;

	force += (gravity + thrust + Lift + Drag) / mass;
}

void FlightMovement::CalculatePrimarySurfaceForce(void)
{
	if(yoke.x == 0.0f && manuverForce.x != 0.0f)
		manuverForce.x += manuverForce.x * -0.05f/*deltatime * 10.0f*/;
	else
	{
		manuverForce.x += yoke.x * 0.01f/*deltatime*/;
		if (std::abs(manuverForce.x) > 1.0f)
			manuverForce.x /= std::abs(manuverForce.x);
	}

	if (yoke.y == 0.0f && manuverForce.y != 0.0f)
		manuverForce.y += manuverForce.y * -0.05f/*deltatime * 10.0f*/;
	else
	{
		manuverForce.y += yoke.y * 0.01f/*deltatime*/ * -1.0f;
		if (std::abs(manuverForce.y) > 1.0f)
			manuverForce.y /= std::abs(manuverForce.y);
	}

	if (yoke.z == 0.0f && manuverForce.z != 0.0f)
		manuverForce.z += manuverForce.z * -0.25f/*deltatime * 25.0f*/;
	else
	{
		manuverForce.z += yoke.z * 0.01f/*deltatime*/;
		if (std::abs(manuverForce.z) > 1.0f)
			manuverForce.z /= std::abs(manuverForce.z);
	}

	finalMenuverForce.x = maneuverSpeed.x * manuverForce.x * 0.5f;
	finalMenuverForce.y = maneuverSpeed.y * manuverForce.y * 0.5f;
	finalMenuverForce.z = maneuverSpeed.z * manuverForce.z * 0.75f;

	if(isSuper)
		transform->Rotate(Vector3{ 0.0f, sinf((transform->Angle().z / 2.0f) * (3.14159265357989f / 90.0f)) * -0.01f/*deltatime*/, 0.0f } * 5.0f);
	else
		transform->Rotate(Vector3{ 0.0f, sinf((transform->Angle().z / 2.0f) * (3.14159265357989f / 90.0f)) * -0.01f/*deltatime*/, 0.0f });
	transform->RelativeRotate(finalMenuverForce * 0.01f/*deltatime*/);
	//transform->RelativeRotate(manuverForce * DeltaTime());
}

float FlightMovement::IdlePower(float EnginePower, float stallSpeed)
{
	/*
	minspeed = force.magnitude()
	force = thrust + Drag + gravity + lift
	thrust = EnginePower * throttle
	Drag = -EnginePower * throttle * ((velocity - minspeed) / (maxSpeed - minspeed)) * ((velocity - minspeed) / (maxSpeed - minspeed))
	Drag = -EnginePower * throttle * ((minspeed - minspeed) / (maxSpeed - minspeed)) * ((minspeed - minspeed) / (maxSpeed - minspeed))
	Drag = -EnginePower * throttle * (0 / (maxSpeed - minspeed)) * (0 / (maxSpeed - minspeed))
	Drag = -EnginePower * throttle * 0
	Drag = 0
	thrust + Drag = minspeed
	thrust = minspeed
	EnginePower * throttle = minspeed
	throttle(min) = minspeed / EnginePower
	*/
	return stallSpeed / EnginePower;
}

float FlightMovement::IdlePower(float EnginePower, float mass, float idleVelocity, float dragCoefficient)
{
	// Real AeroDynamics Test -> Failed :(
	float velocity = stallSpeed;

	Vector3 forward = transform->Forward().normalize();

	speedFactor = 2 * ((velocity - stallSpeed) / (criticalSpeed - stallSpeed));

	thrust = forward * EnginePower * throttle;
	Drag = forward * -1.0f * thrust.magnitude() * (speedFactor / 2) * (speedFactor / 2);
	gravity = Vector3(0.0f, (-1.0f * 9.8f * mass), 0.0f);
	Lift = transform->Up().normalize() * ((1 + speedFactor) * (9.8f * mass));

	Vector3 newDirection = (gravity + Lift);
	newDirection.x *= 0.001f;
	newDirection.z *= 0.001f;
	newDirection += thrust + Drag;
	force = newDirection;

	return (dragCoefficient * idleVelocity * idleVelocity + idleVelocity) / EnginePower;
}

float FlightMovement::IdleVelocity(float mass, float liftCoefficient)
{
	return std::sqrt((mass * 9.8f) / liftCoefficient);
}

Engine::Component* FlightMovement::Clone(void)
{
	return new FlightMovement(*this);
}

HRESULT FlightMovement::Start(void)
{
	velocity = stallSpeed;
	throttle = idleThrottle = IdlePower(EnginePower, stallSpeed);
	DirectX::XMStoreFloat3(&force, DirectX::XMVector3Rotate(DirectX::XMVectorSet(0.0f, 0.0f, stallSpeed, 1.0f), DirectX::XMLoadFloat4(&transform->Quaternion())));

	// Real AeroDynamics Test -> Failed :(
	//throttle = idleThrottle = IdlePower(EnginePower, mass, idleVelocity, dragCoefficient);
	return S_OK;
}

// For Test !!!!!
void FlightMovement::Render(void)
{
}
// For Test !!!!!

void FlightMovement::Update(void)
{
	if (force.magnitude() == 0.0f) // safety
		Start();
	CalculateMovement();
	CalculatePrimarySurfaceForce();

	if (force.magnitude() >= (criticalSpeed * 1.5f)) // safety
		force = force.normalize() * (criticalSpeed * 1.5f);

	//transform->Translate(force * 0.0005f/*deltatime*/ * 50.0f);
	transform->Translate(force * 0.0005f/*deltatime*/);
	//transform->Translate(force * 0.0125f/*deltatime*/);
	//transform->Translate(force * DeltaTime() * 100.0f);
	//transform->Translate(force * DeltaTime() * 2.0f);
}

void FlightMovement::LateUpdate(void)
{

}


