// (c) 2021 MVFT. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StaticPart.h"
#include "Flap.generated.h"

/**
 * 
 */
UCLASS()
class PTRAIN_API AFlap : public AStaticPart
{
	GENERATED_BODY()
public:
	virtual FVector CalcForces() override;
	virtual FVector CalcTorques() override;
};
