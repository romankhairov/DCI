// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PakExportUtilityRuntime.h"
#include "Engine/DeveloperSettings.h"
#include "PakExportUtility.generated.h"

struct ReportPackageData;
class UStaticMesh;
class UMeshComponent;
class UProceduralMeshComponent;
class AMeshOpsActor;

/**
 * 
 */
UCLASS()
class UPakExportUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "PakExport") static void AddAssets();
	UFUNCTION(BlueprintCallable, Category = "PakExport") static void CleanAssets();
	UFUNCTION(BlueprintCallable, Category = "PakExport") static void Export2Pak();
	UFUNCTION(BlueprintCallable, Category = "PakExport") static void Export2Paks();
	UFUNCTION(BlueprintCallable, Category = "PakExport") static void Export2MaterialsPak();
	UFUNCTION(BlueprintCallable, Category = "PakExport") static void Export2CamerasPak();
	UFUNCTION(BlueprintCallable, Category = "PakExport") static void MakeBunchFromMorphs(FString SourcePakageName, FString MaterialPakageName, FString EdgeMaterialPakageName, FString SliceMaterialPakageName, FString SubstrateMaterialPakageName, FString DestinationPakageName, TArray<FMorphTargetData> MorphTargets, FVector2D Size, float Gap, FString Styling, bool OnlyCut);

private:
	static bool SetStarted(bool Val);
	static bool Prepare();
	static bool MigratePackages(const TArray<FAssetData>& Assets = {});
	static void MigratePackages_ReportConfirmed(TSharedPtr<TArray<ReportPackageData>> PackageDataToMigrate);
	static bool CookPak(const FString& DestinationFile = {});
	static bool RunBat(const FString& BatFileAndParams);
	static void EnableSimpleCollisions(const TArray<FAssetData>& Assets);
	static void RecursiveGetDependencies(const FName& PackageName, TSet<FName>& AllDependencies, TSet<FName>& Dependencies, const FString& OriginalRoot);
	static UStaticMesh* ConvertMeshesToStaticMesh(const TArray<UMeshComponent*>& InMeshComponents, const FTransform& InRootTransform, const FString& InPackageName);
	static void ConvertActorsToStaticMesh(const TArray<AActor*> InActors, FString DestinationPakageName);
	static void ProceduralToStaticMesh(UProceduralMeshComponent* ProceduralMeshComponent, const FString& InPackageName);
	static void SetAllowCPUAccess(UObject* WorldContextObject, UStaticMesh* StaticMesh);
	static TArray<UMaterialInterface*> GetBunchMaterials(const FString& MaterialPackageName);
	static void Export2Pak_Internal(const TArray<FAssetData>& Assets = {}, const FString& DestinationDir = {});
};

UCLASS(config = Debug, defaultconfig)
class UPakExportDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()
public:
	UPROPERTY(config, EditAnywhere) TSoftClassPtr<AMeshOpsActor> MeshOpsActorClass;

	static UClass* GetMeshOpsActorClass();
};
