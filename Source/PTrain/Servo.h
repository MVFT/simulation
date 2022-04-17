// (c) 2021 MVFT. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Servo.generated.h"

UCLASS()
class PTRAIN_API AServo : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AServo();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void AssumePose(float bounds);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Servo Parameters")
		FVector2D AngleBounds = FVector2D(-90,90);
};
