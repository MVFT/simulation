// (c) 2021 MVFT. All Rights Reserved.


#include "Drone.h"
#include "Kismet/KismetMathLibrary.h"
#include "GenericPlatform/GenericPlatformMath.h"

// Sets default values
ADrone::ADrone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	collision = CreateDefaultSubobject<UBoxComponent>("Collision");
	collision->SetSimulatePhysics(true);
	collision->SetEnableGravity(true);
	collision->SetCollisionProfileName(TEXT("PhysicsActor"));
	RootComponent = collision;
}

// Called when the game starts or when spawned
void ADrone::BeginPlay()
{
	Super::BeginPlay();
	startPos = GetActorLocation();
}

float ADrone::ThrustFrontRight(float inpower) {
	return  ForceScaleFactor* ThrustFactorFront* inpower;
}
float ADrone::ThrustFrontLeft(float inpower) {
	return  ForceScaleFactor* ThrustFactorFront* inpower;
}
float ADrone::ThrustBackRight(float inpower) {
	return  ForceScaleFactor* ThrustFactorBack* inpower;
}
float ADrone::ThrustBackLeft(float inpower) {
	return ForceScaleFactor* ThrustFactorBack* inpower;
}

FVector ADrone::TorqueFrontLeft(FVector inThrust)
{
	return FVector::CrossProduct(FrontLeftMotorPos, inThrust) * ForceScaleFactor;
}

FVector ADrone::TorqueFrontRight(FVector inThrust)
{
	return FVector::CrossProduct(FrontRightMotorPos, inThrust) * ForceScaleFactor;
}

FVector ADrone::TorqueBackLeft(FVector inThrust)
{
	return FVector::CrossProduct(BackLeftMotorPos, inThrust) * ForceScaleFactor;
}

FVector ADrone::TorqueBackRight(FVector inThrust)
{
	return FVector::CrossProduct(BackRightMotorPos, inThrust) * ForceScaleFactor;
}



FVector ADrone::TotalSpin(FVector FL, FVector FR, FVector BL, FVector BR) {
	constexpr float b = 1.0f;	// b is a constant we can make up later (we will get it from Propulsion)
	return b * (FL + BR - FR - BL) * ForceScaleFactor;
}


// Called every frame
void ADrone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	auto localVelVec = UKismetMathLibrary::InverseTransformDirection(GetTransform(), GetVelocity());
	if (isHovering) {
		// motor thrust
		auto FRThrust = GetActorUpVector() * ThrustFrontRight(power);
		auto FLThrust = GetActorUpVector() * ThrustFrontLeft(power);
		auto BLThrust = GetActorUpVector() * ThrustBackLeft(power);
		auto BRThrust = GetActorUpVector() * ThrustBackRight(power);
		collision->AddForce(FRThrust);
		collision->AddForce(FLThrust);
		collision->AddForce(BLThrust);
		collision->AddForce(BRThrust);

		// motor torques
		collision->AddTorque(TorqueFrontLeft(FLThrust));
		collision->AddTorque(TorqueFrontRight(FRThrust));
		collision->AddTorque(TorqueBackLeft(BLThrust));
		collision->AddTorque(TorqueBackRight(BRThrust));

		// motor spin torques
		collision->AddTorque(TotalSpin(FLThrust, FRThrust, BLThrust, BRThrust));
	
	}
	else {
		// behave like an airplane
		// power goes forward, and moveVec controls wings & rudder
		auto forwardForce = -GetActorForwardVector() * ForceScaleFactor * power;
		collision->AddForce(forwardForce);

		// innacurate lift 
		
		auto liftVec = -localVelVec.X * GlideLift;	// positive X is pointing aft
		collision->AddForce(FVector(0,0,liftVec));

		// even worse wing control
	}

	// Drag
	auto drag = FVector::DotProduct(localVelVec, FVector(0.0f, 0.0f, 1.0f)) * WingResistance;
	auto dragVec = drag * (-GetActorUpVector());
	collision->AddForce(dragVec);
	
	drag = FVector::DotProduct(localVelVec, FVector(1.0f, 0.0f, 0.0f)) * WingResistance;
	dragVec = drag * (-GetActorForwardVector());	// negative x is forward
	//UE_LOG(LogTemp, Warning, TEXT("%f"), drag);
	collision->AddForce(dragVec);

	// TODO: Lift

}

void ADrone::Power(float amt) {
	power = amt;
}

void ADrone::Forward(float amt) {
	moveVec.X = amt;
}

void ADrone::Right(float amt) {
	moveVec.Y = amt;
}

void ADrone::RotateZ(float amt) {
	moveVec.Z = amt;
}

void ADrone::Reset() {
	SetActorLocation(startPos);
	collision->SetAllPhysicsLinearVelocity(FVector(0, 0, 0));
	collision->SetAllPhysicsAngularVelocityInDegrees(FVector(0, 0, 0));

}