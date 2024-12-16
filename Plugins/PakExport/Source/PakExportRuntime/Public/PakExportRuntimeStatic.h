// Copyright 2024 3D Source, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PakExportRuntimeStatic.generated.h"

/**
 * 
 */
UCLASS()
class PAKEXPORTRUNTIME_API UPakExportRuntimeStatic : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, Category = PakExport) static FString GetPakExportName(const FString& PakFile);
	UFUNCTION(BlueprintPure, Category = PakExport) static FString GetPakExportMountPath() { return PakExportMountPath; }
	
	static constexpr auto PakExportName{"PakE"};
	static constexpr auto PakExportMountPath{"../../../PakExport/Plugins/"};
	
	
	
};
