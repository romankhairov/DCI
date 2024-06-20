// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Commandlets/Commandlet.h"
#include "MoveAssetsCommandlet.generated.h"

/**
 * 
 */
UCLASS()
class PAKEXPORTEDITOR_API UMoveAssetsCommandlet : public UCommandlet
{
	GENERATED_BODY()
	
	virtual int32 Main(const FString& Params) override;
};
