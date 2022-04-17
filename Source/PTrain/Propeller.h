// (c) 2021 MVFT. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StaticPart.h"
#include "Propeller.generated.h"

/**
 * 
 */
UCLASS()
class PTRAIN_API APropeller : public AStaticPart
{
	GENERATED_BODY()

private:
	float rotationSpeed;
public:
	virtual FVector CalcForces() override;
	virtual FVector CalcTorques() override;
	
	// invoked by connected Motors
	void SetRotationSpeed(decltype(rotationSpeed) r) { rotationSpeed = r; }
};
