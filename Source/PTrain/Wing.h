// (c) 2021 MVFT. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StaticPart.h"
#include "Wing.generated.h"

/**
 * 
 */
UCLASS()
class PTRAIN_API AWing : public AStaticPart
{
	GENERATED_BODY()

public:
	virtual FVector CalcForces() override;
	virtual FVector CalcTorques() override;
	
};
