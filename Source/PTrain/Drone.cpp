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
	auto forwardVel = -localVelVec.X;

	if (isHovering) {

		// control system for hover
		//UE_LOG(LogTemp, Warning, TEXT("%f"), power);
		float FLPower = power, FRPower = power, BLPower = power, BRPower = power;
		
		FLPower += YawHoverScaleFactor * moveVec.Z;
		FRPower -= YawHoverScaleFactor * moveVec.Z;
		BRPower += YawHoverScaleFactor * moveVec.Z;
		BLPower -= YawHoverScaleFactor * moveVec.Z;

		// motor thrust
		auto FRThrust = ThrustFrontRight(FRPower);
		auto FLThrust = ThrustFrontLeft(FLPower);
		auto BLThrust = GetActorUpVector() * ThrustBackLeft(BLPower);
		auto BRThrust = GetActorUpVector() * ThrustBackRight(BRPower);

		collision->AddForce(GetActorUpVector() * FRThrust);
		collision->AddForce(GetActorUpVector() * FLThrust);
		collision->AddForce(BLThrust);
		collision->AddForce(BRThrust);

		// motor torques
		collision->AddTorque(TorqueFrontLeft(GetActorUpVector() * FLThrust));
		collision->AddTorque(TorqueFrontRight(GetActorUpVector() * FRThrust));
		collision->AddTorque(TorqueBackLeft(BLThrust));
		collision->AddTorque(TorqueBackRight(BRThrust));

		// motor spin torques
		collision->AddTorque(TotalSpin(GetActorUpVector() * FLThrust, GetActorUpVector() * FRThrust, BLThrust, BRThrust));
	
	}
	else {
		// glide

		auto FRThrust = ThrustFrontRight(power);
		auto FLThrust = ThrustFrontLeft(power);

		auto FrontVec = -GetActorForwardVector();
		collision->AddForce(FrontVec * FRThrust);
		collision->AddForce(FrontVec * FLThrust);

		// Ruddervator positions
		float LRVAngle = MaxRuddervatorAngle * (currentYaw + currentPitch) / 2, 
			RRVAngle = MaxRuddervatorAngle * (currentYaw + currentPitch) / 2;

		// Ruddervators on V-tail
		//collision->AddTorque(UKismetMathLibrary::TransformDirection(GetTransform(), FVector(forwardVel * LRVAngle * RollScaleFactor, 0, 0)));
		//collision->AddTorque(-UKismetMathLibrary::TransformDirection(GetTransform(), FVector(forwardVel * RRVAngle * RollScaleFactor, 0, 0)));

		// aileron positions
		float LAileronAngle = 0, RAileronAngle = 0;
		LAileronAngle = MaxAileronAngle * -currentBank;	
		RAileronAngle = MaxAileronAngle * currentBank;

		// aileron roll torque
		collision->AddTorque(UKismetMathLibrary::TransformDirection(GetTransform(), FVector(forwardVel * LAileronAngle * RollScaleFactor,0,0)));
		collision->AddTorque(-UKismetMathLibrary::TransformDirection(GetTransform(), FVector(forwardVel * RAileronAngle * RollScaleFactor,0,0)));

		// aileron pitch torque
		//collision->AddTorque(UKismetMathLibrary::TransformDirection(GetTransform(), FVector(0, forwardVel * LAileronAngle * RollScaleFactor, 0)));
		//collision->AddTorque(UKismetMathLibrary::TransformDirection(GetTransform(), FVector(0, forwardVel * RAileronAngle * RollScaleFactor, 0)));
	}	

	// Lift
	auto normalizedUp = GetActorUpVector();
	normalizedUp.Normalize();
	auto cosTheta = FVector::DotProduct( normalizedUp,FVector(0, 0, 1));
	forwardVel = UKismetMathLibrary::Abs(localVelVec.X);
	collision->AddForce((cosTheta * forwardVel) * normalizedUp * LiftScaleFactor);

	// Drag - perpendicular
	auto drag = FVector::DotProduct(localVelVec, FVector(0.0f, 0.0f, 1.0f)) * WingResistance;
	auto dragVec = drag * (-GetActorUpVector());
	collision->AddForce(dragVec);
	
	// drag - parallel
	drag = FVector::DotProduct(localVelVec, FVector(1.0f, 0.0f, 0.0f)) * ForwardResistance;
	dragVec = drag * (-GetActorForwardVector());	// negative x is forward
	//UE_LOG(LogTemp, Warning, TEXT("%f"), drag);
	collision->AddForce(dragVec);

	// angular drag - yaw
	collision->AddTorque(-collision->GetPhysicsAngularVelocityInRadians() * AngularDrag * ForceScaleFactor);

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
	currentYaw = amt;	// slightly redundant...
}

void ADrone::Reset() {
	SetActorLocation(startPos);
	collision->SetAllPhysicsLinearVelocity(FVector(0, 0, 0));
	collision->SetAllPhysicsAngularVelocityInDegrees(FVector(0, 0, 0));

}