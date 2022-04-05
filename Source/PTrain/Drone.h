// (c) 2021 MVFT. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Drone.generated.h"

#define BP_CAT "Aircraft Parameters"

UCLASS()
class PTRAIN_API ADrone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADrone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	float power = 0;
	FVector moveVec;
	bool isHovering = true;	// begin in hover mode 

	FVector startPos;
	float currentBank = 0;
	float currentPitch = 0;
	
	// thrust functions per motor
	float ThrustFrontRight(float);
	float ThrustFrontLeft(float);
	float ThrustBackRight(float);
	float ThrustBackLeft(float);

	// torque functions per motor
	FVector TorqueFrontLeft(FVector);
	FVector TorqueFrontRight(FVector);
	FVector TorqueBackLeft(FVector);
	FVector TorqueBackRight(FVector);
	FVector TotalSpin(FVector, FVector, FVector, FVector);



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Power(float percent);
	void Forward(float amt);
	void Right(float amt);
	void RotateZ(float amt);
	void SetBank(float amt) {	// see APilot::SetBank for amounts
		currentBank = amt;
	}
	void SetPitch(float amt) {
		currentPitch = amt;
	}
	void Switch() {
		isHovering = !isHovering;
	}
	void Reset();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* collision;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Aircraft Parameters")
		float ForceScaleFactor = 10000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aircraft Parameters")
		// thrust provided by 1 motor in newtons
		float ThrustFactorFront = 66.0/4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aircraft Parameters")
		// thrust provided by 1 motor in newtons
		float ThrustFactorBack = 66.0 / 4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aircraft Parameters")
		float GlideLift = 50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aircraft Parameters")
		FVector FrontLeftMotorPos = FVector(-30,-30,0);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aircraft Parameters")
		FVector FrontRightMotorPos = FVector(-30,30,0);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aircraft Parameters")
		FVector BackLeftMotorPos = FVector(30,-30,0);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aircraft Parameters")
		FVector BackRightMotorPos = FVector(30,30,0);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aircraft Parameters")
		float WingResistance = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aircraft Parameters")
		float LiftScaleFactor = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aircraft Parameters")
		FVector RuddervatorAnglesDegrees = FVector(45,0,0);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aircraft Parameters")
		float RuddervatorScaleFactor = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aircraft Parameters")
		float MaxAileronAngle = 45;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aircraft Parameters")
		float RollScaleFactor = 20;
};
