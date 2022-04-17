// (c) 2021 MVFT. All Rights Reserved.


#include "Motor.h"

// Sets default values
AMotor::AMotor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMotor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMotor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMotor::PropagateSpeed(float power) {
	// TODO: get all child objects of type APropeller and call its SetRotationSpeed based on this motor's properties
}
