// Copyright 2024 3D Source, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PayloadsBase.h"
#include "Misc/EngineVersion.h"
#include "Misc/PackageName.h"
#include "PakExportUtilityRuntime.generated.h"

//JSON data structs

class FPakPlatformFile;

USTRUCT()
struct FHex
{
	GENERATED_BODY()
public:
	FHex() {}
	FHex(int32 R, int32 G, int32 B) : R(R), G(G), B(B) {}
	int32 R{0};
	int32 G{0};
	int32 B{0};
};

USTRUCT()
struct FMaterialVariantData
{
	GENERATED_BODY()
public:
	FMaterialVariantData() {}
	FMaterialVariantData(const FString& Lable, const FString& Name, const FHex& Hex)
	: label(Lable), name(Name) { SetHex(Hex); }
	void SetHex(const FHex& Hex)
	{
		hex = "rgb("
		+ FString::FromInt(Hex.R) + ","
		+ FString::FromInt(Hex.G) + ","
		+ FString::FromInt(Hex.B) + ")";
	}

	UPROPERTY() FString label;
	UPROPERTY() FString name;
	
protected:
	UPROPERTY() FString hex { "rgb(0,0,0)" };
};

USTRUCT()
struct FMaterialPropertyData
{
	GENERATED_BODY()
public:
	UPROPERTY() TArray<FMaterialVariantData> variants;
};

USTRUCT()
struct FMaterialsAssetData
{
	GENERATED_BODY()
public:
	UPROPERTY() FString title;
	UPROPERTY() FString sku;
	UPROPERTY() FString pakFilePath;
	UPROPERTY() FString id;
	UPROPERTY() FString previewPath;
	UPROPERTY() FMaterialPropertyData property;
	UPROPERTY() EAssetType type{EAssetType::material};
};

USTRUCT()
struct FMaterialsData
{
	GENERATED_BODY()
public:
	UPROPERTY() FMaterialsAssetData data;
};

USTRUCT(BlueprintType)
struct FMorphTargetData
{
	GENERATED_BODY()
public:
	FMorphTargetData() {}
	FMorphTargetData(FName Name, float Value) : Name(Name), Value(Value) {}
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PakExport") FName	Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PakExport") float Value{};
};

USTRUCT()
struct FExportData
{
	GENERATED_BODY()
public:
	FExportData() { UpdateEngineVersion(); }

	EAssetType GetType() const { return type; }
	
	void UpdateEngineVersion()
	{
		const auto EngineVersion = FEngineVersion::Current();
		unrealVersion = FString::FromInt(EngineVersion.GetMajor())
		+ "." + FString::FromInt(EngineVersion.GetMinor())
		+ "." + FString::FromInt(EngineVersion.GetPatch());
	}
	
protected:
	UPROPERTY() EAssetType type{};
	UPROPERTY() FString unrealVersion{};

public:
	UPROPERTY() FString assetPath{};
};

USTRUCT()
struct FMaterialableExportData : public FExportData
{
	GENERATED_BODY()
public:
	UPROPERTY() TArray<FString> slots{};
};

USTRUCT()
struct FProductExportData : public FMaterialableExportData
{
	GENERATED_BODY()
public:
	FProductExportData()
	{
		type = EAssetType::product;
	}
	
	UPROPERTY() bool hasShowcase{};
};

USTRUCT()
struct FEnvironmentExportData : public FMaterialableExportData
{
	GENERATED_BODY()
public:
	FEnvironmentExportData()
	{
		type = EAssetType::environment;
	}
};

USTRUCT()
struct FMaterialExportData : public FExportData
{
	GENERATED_BODY()
public:
	FMaterialExportData()
	{
		type = EAssetType::material;
	}
};

/**
 * 
 */
UCLASS()
class PAKEXPORTRUNTIME_API UPakExportUtilityRuntime : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, Category = "PakExport") static FString GetSlotDelimiter();

	UFUNCTION(BlueprintCallable, Category = "PakExport")
	static void GenerateJsonsForAssets(const FString& Guid, const TArray<FString>& InAssets, const FString& DestinationFile, FString& OutJsonString);

	static bool GenerateJsonsForAssets(const TArray<FAssetData>& InAssets, const FString& DestinationFile,
	                                   bool MaterialsPakRequested = false, bool CamerasPakRequested = false,
	                                   bool LevelSequencePakRequested = false, const FString& Archived = {}
	                                   , FString* OutJsonString = {}, const FString& Guid = {});

	UFUNCTION(BlueprintCallable, Category = "PakExport")
	static FCameraPresetBasePayload GenerateCameraPresetPayload(const AActor* const CameraActor, float FocusOffset = 0.f);

	UFUNCTION(BlueprintCallable, Category = "PakExport")
	static FString GetPakMountPoint(const FString& PakFilename);

	UFUNCTION(BlueprintCallable, Category = "PakExport")
	static FString GetPakGuid(const FString& PakFilename);
	
private:
	template<class T>
	static T *LoadObjectFromPak(const FString &Filename)
	{
		const FString Name = T::StaticClass()->GetName() + TEXT("'") + Filename + TEXT(".") + FPackageName::GetShortName(Filename) + TEXT("'");
		return Cast<T>(StaticLoadObject(T::StaticClass(), nullptr, *Name));
	}

	static FPakPlatformFile* GetPakPlatformFile();
};
