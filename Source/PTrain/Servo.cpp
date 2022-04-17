// (c) 2021 MVFT. All Rights Reserved.


#include "Servo.h"

// Sets default values
AServo::AServo()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AServo::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AServo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AServo::AssumePose(float pos) {
	//TODO: set actor local rotation based on lerp(pos, angle)
}

