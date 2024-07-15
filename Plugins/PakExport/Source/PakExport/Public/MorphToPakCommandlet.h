// Copyright 2024 3D Source, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Commandlets/Commandlet.h"
#include "MorphToPakCommandlet.generated.h"

/**
 * 
 */
UCLASS()
class PAKEXPORT_API UMorphToPakCommandlet : public UCommandlet
{
	GENERATED_BODY()
	
	virtual int32 Main(const FString& Params) override;
};
