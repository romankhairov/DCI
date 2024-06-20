// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PakExportEditorUtility.generated.h"

/**
 * 
 */
UCLASS()
class UPakExportEditorUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "PakExport")
	static int32 MoveAssets(const FString& Guid);

private:
	static TArray<FString> GetAllPackages();
	static void FixUpRedirectors(const TArray<FString>& SelectedPaths = {"/Game"});
};
