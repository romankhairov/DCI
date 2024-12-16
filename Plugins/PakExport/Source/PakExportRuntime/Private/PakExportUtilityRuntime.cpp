// Copyright 2024 3D Source, Inc. All Rights Reserved.


#include "PakExportUtilityRuntime.h"
#include "Runtime/Launch/Resources/Version.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "CineCameraComponent.h"
#include "PublicCineCameraComponent.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "JsonObjectConverter.h"
#include "CineCameraActor.h"
#include "LevelSequence.h"
#include "Engine/BlueprintGeneratedClass.h"
#include "Engine/SimpleConstructionScript.h"
#include "Engine/SCS_Node.h"
#include "PakExportRuntime.h"
#include "Animation/SkeletalMeshActor.h"
#include "Engine/StaticMesh.h"
#include "Engine/SkeletalMesh.h"
#if ENGINE_MAJOR_VERSION > 4
#include "Engine/SkinnedAssetCommon.h"
#endif
#include "PayloadFunctionLibrary.h"
#include "Engine/StaticMeshActor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "IPlatformFilePak.h"
#include "Misc/Filehelper.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimSequence.h"
#include "UObject/Package.h"
#include "AssetRegistry/AssetData.h"
#include "Materials/MaterialInterface.h"
#include "Engine/Level.h"
#include "Misc/Base64.h"

constexpr auto SlotDelimiter{"::"};

FString UPakExportUtilityRuntime::GetSlotDelimiter()
{
	return SlotDelimiter;
}

void UPakExportUtilityRuntime::GenerateJsonsForAssets(const FString& Guid, const TArray<FString>& InAssets, const FString& DestinationFile, FString& OutJsonString)
{
	TArray<FAssetData> Assets;
	const auto& AssetRegistryModule = FModuleManager::Get().LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry")).Get();
	for (const auto& Asset : InAssets)
	{
		//									backward compatibility
		const auto Dlc{Guid.IsEmpty() ? UPakExportRuntimeStatic::PakExportName : Guid};
		const auto PakName = "/" + Asset.Replace(TEXT("Content"), *Dlc, ESearchCase::CaseSensitive)
		.Replace(TEXT(".uasset"), TEXT(""), ESearchCase::CaseSensitive);
		const auto Obj = LoadObjectFromPak<UObject>(PakName);
		const auto PackageName = Obj->GetPackage()->GetName();
		TArray<FAssetData> A;
		AssetRegistryModule.GetAssetsByPackageName(*(PackageName), A);
		Assets.Append(A);
	}

	GenerateJsonsForAssets(Assets, DestinationFile, false, false, false, {}, &OutJsonString, Guid);
}

bool UPakExportUtilityRuntime::GenerateJsonsForAssets(const TArray<FAssetData>& InAssets,
                                                      const FString& DestinationFile,
                                                      bool MaterialsPakRequested/* = false*/,
                                                      bool CamerasPakRequested/* = false*/,
                                                      bool LevelSequencePakRequested/* = false*/,
                                                      const FString& ArchivedTempDir/* = {}*/,
                                                      FString* OutJsonString/* = {}*/,
                                                      const FString& Guid/* = {}*/)
{
	TArray<const FAssetData*> SelectedMaterialsAssets;
	TArray<FAssetPakJson> LevelSequencePakDatas;

	const auto AllAssetsJson {OutJsonString ? TSharedPtr<FJsonObject>(new FJsonObject) : nullptr};
	const auto AddAssetJson = [AllAssetsJson](TSharedPtr<FJsonValue> AssetJson)
	{
		if (AllAssetsJson)
		{
			constexpr auto AssetsArrayName{"assets"};
			
			auto AssetsArray = AllAssetsJson->HasTypedField<EJson::Array>(AssetsArrayName)
			? AllAssetsJson->GetArrayField(AssetsArrayName)
			: TArray<TSharedPtr<FJsonValue>>{};

			AssetsArray.Emplace(AssetJson);
			
			AllAssetsJson->SetArrayField(AssetsArrayName, AssetsArray);
		}
	};
	
	const TOptional<FString> DestinationDir = !DestinationFile.IsEmpty() ? FPaths::GetPath(DestinationFile) : TOptional<FString>();
	const auto PakFilePath = DestinationDir.IsSet() ? FPaths::GetCleanFilename(DestinationDir.GetValue()) : FString(); //TODO: make relative path instead simple filename
	const auto SelectedPakFileNameBase = DestinationDir.IsSet() ? FPaths::GetBaseFilename(DestinationDir.GetValue()) : FString();
	for (const auto& SelectedAsset : InAssets)
	{
		const auto Asset = SelectedAsset.GetAsset();
		const auto Name = Asset->GetName();
		const auto PackageName{SelectedAsset.PackageName.ToString().RightChop(6)};
		const auto AssetName =  PackageName;
		
		//material
		if (Asset->IsA(UMaterialInterface::StaticClass()))
		{
			if (!MaterialsPakRequested)
			{
				if (!ArchivedTempDir.IsEmpty())
				{
					FMaterialExportData MaterialExportData{};

					MaterialExportData.assetPath = AssetName;

					FString JsonString;
					if (FJsonObjectConverter::UStructToJsonObjectString(MaterialExportData, JsonString))
					{
						FFileHelper::SaveStringToFile(JsonString, *(ArchivedTempDir + "/config.json"));
					}
					else
					{
						UE_LOG(LogPakExportRuntime, Error, TEXT("Generate material pak json failed!"));
					}
				}
				else
				{
					FMaterialPayload MaterialPakData;
					MaterialPakData.materialName = Name;
					FMaterialJson MaterialJson;
					MaterialJson.name = AssetName;
					MaterialJson.pakFilePath = PakFilePath;
					MaterialPakData.materialPak.initialState.materials.Emplace(MoveTemp(MaterialJson));
					
					//save JSON to file near pak file
					if (DestinationDir.IsSet())
					{
						FString JsonString;
						if (FJsonObjectConverter::UStructToJsonObjectString(MaterialPakData, JsonString))
						{
							FFileHelper::SaveStringToFile(JsonString, *(DestinationDir.GetValue() + FString("/") + Name + ".json"));
						}
						else
						{
							UE_LOG(LogPakExportRuntime, Error, TEXT("Generate material pak json failed!"));
						}
					}
				}
			}
			else
			{
				SelectedMaterialsAssets.Add(&SelectedAsset);
			}
		}
		//level
		else if (Asset->IsA(UWorld::StaticClass()))
		{
			if (!CamerasPakRequested)
			{
				if (!ArchivedTempDir.IsEmpty())
				{
					FEnvironmentExportData EnvironmentExportData{};

					EnvironmentExportData.assetPath = AssetName;

					const auto World = Cast<UWorld>(Asset);
					for (int32 LevelIndex = 0; LevelIndex < World->GetNumLevels(); LevelIndex++)
					{
						const auto& Actors{World->GetLevel(LevelIndex)->Actors};
						for (const auto& Actor : Actors)
						{
							if (Actor->IsA<AStaticMeshActor>()
							|| Actor->IsA<ASkeletalMeshActor>()
							|| (((UObject*)Actor->GetClass())->IsA<UBlueprintGeneratedClass>()))
							{
								const auto ActorCounter{&Actor - &Actors[0]};
								Actor->ForEachComponent<UMeshComponent, TFunction<void(UMeshComponent*)>>(false
									, [&EnvironmentExportData, Actor, LevelIndex, ActorCounter](const auto MeshComponent)
									{
										//Levels slot names generation
										//Should be same as in PakExport project UPakExportEditorUtility::LevelsActorsUnification function
										for (const auto& MaterialName : MeshComponent->GetMaterialSlotNames())
											EnvironmentExportData.slots.Add(
												UKismetSystemLibrary::GetDisplayName(Actor) + FString::FromInt(LevelIndex) + FString::FromInt(ActorCounter) + SlotDelimiter +
												UKismetSystemLibrary::GetDisplayName(MeshComponent) + SlotDelimiter +
												MaterialName.ToString());
									});
							}
						}
					}

					FString JsonString;
					if (FJsonObjectConverter::UStructToJsonObjectString(EnvironmentExportData, JsonString))
					{
						FFileHelper::SaveStringToFile(JsonString, *(ArchivedTempDir + "/config.json"));
					}
					else
					{
						UE_LOG(LogPakExportRuntime, Error, TEXT("Generate level pak json failed!"));
					}
				}
				else
				{
					FLevelPayload LevelPakData;
					LevelPakData.levelName = Name;
					LevelPakData.levelPak.pakFilePath = PakFilePath;
					LevelPakData.levelPak.assetName = AssetName;

					const auto World = Cast<UWorld>(Asset);
					TArray<AActor*> Actors;
					for (int32 LevelIndex = 0; LevelIndex < World->GetNumLevels(); LevelIndex++)
						Actors.Append(World->GetLevel(LevelIndex)->Actors);
					for (const auto Actor : Actors)
					{
						if (Actor->IsA<AStaticMeshActor>()
							|| Actor->IsA<ASkeletalMeshActor>()
							|| (((UObject*)Actor->GetClass())->IsA<UBlueprintGeneratedClass>()))
						{
							Actor->ForEachComponent<UMeshComponent, TFunction<void(UMeshComponent*)>>(false
								, [&LevelPakData](const auto MeshComponent)
								{
									for (const auto& MaterialName : MeshComponent->GetMaterialSlotNames())
										LevelPakData.slots.Add(MaterialName.ToString());
								});
						}
					}
					if (DestinationDir.IsSet())
					{
						//save JSON to file near pak file
						FString JsonString;
						if (FJsonObjectConverter::UStructToJsonObjectString(LevelPakData, JsonString))
						{
							FFileHelper::SaveStringToFile(JsonString, *(DestinationDir.GetValue() + FString("/") + Name + ".json"));
						}
						else
						{
							UE_LOG(LogPakExportRuntime, Error, TEXT("Generate level pak json failed!"));
						}
					}
				}
			}
			else
			{
				const auto World = Cast<UWorld>(Asset);
				TArray<AActor*> Actors;
				for (int32 LevelIndex = 0; LevelIndex < World->GetNumLevels(); LevelIndex++)
					Actors.Append(World->GetLevel(LevelIndex)->Actors);
				for (const auto Actor : Actors)
					if (const auto CameraActor = Cast<ACineCameraActor>(Actor))
					{
						if (DestinationDir.IsSet())
						{
							FString JsonString;
							if (FJsonObjectConverter::UStructToJsonObjectString(GenerateCameraPresetPayload(CameraActor), JsonString))
							{
								FFileHelper::SaveStringToFile(JsonString, *(DestinationDir.GetValue() + FString("/")
																  + SelectedPakFileNameBase + "_" + Name + "_" + CameraActor
																  ->GetName() + ".json"));
							}
							else
							{
								UE_LOG(LogPakExportRuntime, Error, TEXT("Generate cameras pak json failed!"));
							}
						}
					}
			}
		}
		//product
		else if (Asset->IsA(UStaticMesh::StaticClass())
			|| Asset->IsA(UBlueprint::StaticClass())
			|| Asset->IsA(USkeletalMesh::StaticClass()))
		{
			if (!ArchivedTempDir.IsEmpty())
			{
				FProductExportData ProductExportData{};

				ProductExportData.assetPath = AssetName;

				if (const auto Mesh = Cast<UStaticMesh>(Asset))
					for (const auto& Material : Mesh->GetStaticMaterials())
						ProductExportData.slots.Add(Material.MaterialSlotName.ToString());

				if (const auto Mesh = Cast<USkeletalMesh>(Asset))
					for (const auto& Material : Mesh->GetMaterials())
						ProductExportData.slots.Add(Material.MaterialSlotName.ToString());

				if (const auto Blueprint = Cast<UBlueprint>(Asset))
				{
					for (const auto Node : Cast<UBlueprintGeneratedClass>(Blueprint->GeneratedClass)->
					                       SimpleConstructionScript->GetAllNodes())
					{
						if (const auto Mesh = Cast<UMeshComponent>(Node->ComponentTemplate))
							for (const auto& Slot : Mesh->GetMaterialSlotNames())
								ProductExportData.slots.Add(
									UKismetSystemLibrary::GetDisplayName(Mesh) + SlotDelimiter + Slot.ToString());
					}
				}

				for (const auto& A : InAssets)
				{
					if (A.GetAsset()->IsA(ULevelSequence::StaticClass()))
					{
						if (!ProductExportData.hasShowcase)
						{
							ProductExportData.hasShowcase = true;
						}
						else
						{
							UE_LOG(LogPakExportRuntime, Error, TEXT("Profuct has more than one showcase!"));
							return false;
						}
					}
				}
				
				FString JsonString;
				if (FJsonObjectConverter::UStructToJsonObjectString(ProductExportData, JsonString))
				{
					FFileHelper::SaveStringToFile(JsonString, *(ArchivedTempDir + "/config.json"));
				}
				else
				{
					UE_LOG(LogPakExportRuntime, Error, TEXT("Generate product pak json failed!"));
				}
			}
			else
			{
				FProductPayload ProductPakData;
				ProductPakData.productName = Name;
				ProductPakData.meshPak.pakFilePath = PakFilePath;
				ProductPakData.meshPak.initialState.meshName = AssetName;

				if (const auto Mesh = Cast<UStaticMesh>(Asset))
					for (const auto& Material : Mesh->GetStaticMaterials())
						ProductPakData.meshPak.initialState.slots.Add(Material.MaterialSlotName.ToString());

				if (const auto Mesh = Cast<USkeletalMesh>(Asset))
				{
					for (const auto& Material : Mesh->GetMaterials())
						ProductPakData.meshPak.initialState.slots.Add(Material.MaterialSlotName.ToString());

					for (const auto& Morph : Mesh->K2_GetAllMorphTargetNames())
						ProductPakData.meshPak.initialState.morphs.AddUnique(Morph);
				}

				//collect animations
				FPakJson AnimationsPakPathData;
				AnimationsPakPathData.pakFilePath = PakFilePath;
				if (const auto Blueprint = Cast<UBlueprint>(Asset))
				{
					TArray<USkeleton*> Skeletons;
					for (const auto Node : Cast<UBlueprintGeneratedClass>(Blueprint->GeneratedClass)->
					                       SimpleConstructionScript->GetAllNodes())
					{
						const auto ComponentTemplate = Node->ComponentTemplate;

						if (const auto Mesh = Cast<UMeshComponent>(ComponentTemplate))
							for (const auto& Slot : Mesh->GetMaterialSlotNames())
								ProductPakData.meshPak.initialState.slots.Add(Slot.ToString());

						if (const auto SkeletalMesh = Cast<USkeletalMeshComponent>(ComponentTemplate))
							Skeletons.Emplace(SkeletalMesh->SkeletalMesh->GetSkeleton());
					}

					const auto& AssetRegistryModule = FModuleManager::Get().LoadModuleChecked<FAssetRegistryModule>(
						TEXT("AssetRegistry")).Get();
					TArray<FName> Dependencies;
					AssetRegistryModule.GetDependencies(*Blueprint->GetPackage()->GetName(), Dependencies);
					for (const auto& Dependency : Dependencies)
					{
						TArray<FAssetData> Assets;
						AssetRegistryModule.GetAssetsByPackageName(Dependency, Assets);
						for (const auto& A : Assets)
							if (const auto AnimAsset = Cast<UAnimSequence>(A.GetAsset()))
								if (Skeletons.Contains(AnimAsset->GetSkeleton()))
									ProductPakData.meshPak.initialState.animations.Add(
										A.PackageName.ToString().RightChop(6));
					}
					ProductPakData.animationsPak = AnimationsPakPathData;
				}

				if (DestinationDir.IsSet())
				{
					FString JsonString;
					if (FJsonObjectConverter::UStructToJsonObjectString(ProductPakData, JsonString))
					{
						FFileHelper::SaveStringToFile(JsonString, *(DestinationDir.GetValue() + FString("/") + Name + ".json"));
					}
					else
					{
						UE_LOG(LogPakExportRuntime, Error, TEXT("Generate product pak json failed!"));
					}
				}
			}
		}
		//LevelSequence
		else if (Asset->IsA(ULevelSequence::StaticClass()))
		{
			FAssetPakJson LevelSequencePakData;
			LevelSequencePakData.pakFilePath = PakFilePath;
			LevelSequencePakData.assetName = AssetName;
			
			if (!LevelSequencePakRequested)
			{
				FInitSequencePayload InitSequencePayload;
				InitSequencePayload.paks.Add(LevelSequencePakData);
				InitSequencePayload.sequenceName = Name;

				if (const auto JsonObject{FJsonObjectConverter::UStructToJsonObject(InitSequencePayload)})
					AddAssetJson(MakeShared<FJsonValueObject>(JsonObject));
				else
					UE_LOG(LogPakExportRuntime, Error, TEXT("Generate Level Sequence pak json failed!"));

				if (DestinationDir.IsSet())
				{
					FString JsonString;
					if (FJsonObjectConverter::UStructToJsonObjectString(InitSequencePayload, JsonString))
					{
						FFileHelper::SaveStringToFile(JsonString, *(DestinationDir.GetValue() + FString("/") + Name + ".json"));
					}
					else
					{
						UE_LOG(LogPakExportRuntime, Error, TEXT("Generate Level Sequence pak json failed!"));
					}
				}
			}
			else
			{
				LevelSequencePakDatas.Emplace(MoveTemp(LevelSequencePakData));
			}
		}
		else
		{
			UE_LOG(LogPakExportRuntime, Error, TEXT("Unknow asset type for pak null!"));
		}
	}

	if (AllAssetsJson) *OutJsonString = UPayloadFunctionLibrary::JsonSerialize(AllAssetsJson.ToSharedRef());
	
	//make level sequence pak JSON
	if (LevelSequencePakRequested)
	{
		FInitSequencePayload InitSequencePayload;
		InitSequencePayload.paks = MoveTemp(LevelSequencePakDatas);

		if (DestinationDir.IsSet())
		{
			FString JsonString;
			if (FJsonObjectConverter::UStructToJsonObjectString(InitSequencePayload, JsonString))
			{
				FFileHelper::SaveStringToFile(JsonString, *(DestinationDir.GetValue() + FString("/") + SelectedPakFileNameBase + ".json"));
			}
			else
			{
				UE_LOG(LogPakExportRuntime, Error, TEXT("Generate Level Sequences pak json failed!"));
			}
		}
	}
	
	//make material pak JSON
	if (MaterialsPakRequested)
	{
		FMaterialsData MaterialsData;
		FMaterialsAssetData MaterialsAssetData;
		MaterialsAssetData.title = SelectedPakFileNameBase;
		MaterialsAssetData.sku = SelectedPakFileNameBase;
		MaterialsAssetData.pakFilePath = PakFilePath;
		MaterialsAssetData.id = ""; //TODO: fill
		MaterialsAssetData.previewPath = ""; //TODO: fill
		FMaterialPropertyData MaterialPropertyData;
		for (const auto SelectedMaterialsAssetPtr : SelectedMaterialsAssets)
		{
			const auto lable{ SelectedMaterialsAssetPtr->PackageName.ToString().RightChop(6) };
			MaterialPropertyData.variants.Emplace(lable, lable, FHex());
		}
		MaterialsAssetData.property = MoveTemp(MaterialPropertyData);
		MaterialsData.data = MoveTemp(MaterialsAssetData);

		if (DestinationDir.IsSet())
		{
			FString JsonString;
			if (FJsonObjectConverter::UStructToJsonObjectString(MaterialsData, JsonString))
			{
				FFileHelper::SaveStringToFile(JsonString, *(DestinationDir.GetValue() + FString("/") + SelectedPakFileNameBase + ".json"));
			}
			else
			{
				UE_LOG(LogPakExportRuntime, Error, TEXT("Generate material pak json failed!"));
			}
		}
	}

	return true;
}

FCameraPresetBasePayload UPakExportUtilityRuntime::GenerateCameraPresetPayload(const AActor* const CameraActor, float FocusOffset/* = 0.f*/)
{
	if (!IsValid(CameraActor)) return {};

	const auto CameraComp = CameraActor->FindComponentByClass<UCineCameraComponent>();

	if (!IsValid(CameraComp)) return {};
	
	FCameraPresetBasePayload CameraData;

	CameraData.location = CameraActor->GetActorLocation();
	CameraData.rotation = CameraComp->GetComponentRotation();
	CameraData.aperture = CameraComp->CurrentAperture;
	CameraData.focalLength = CameraComp->CurrentFocalLength;
	CameraData.focusOffset = FocusOffset;
	CameraData.fov = CameraComp->FieldOfView;

	const auto DuplicateCameraActor{DuplicateObject<AActor>(CameraActor, CameraActor->GetOuter())};
	const auto DuplicateCameraComp = NewObject<UPublicCineCameraComponent>(DuplicateCameraActor->GetOuter());

	TArray<uint8> Bytes;
	FMemoryWriter MemoryWriter(Bytes, true);
	FObjectAndNameAsStringProxyArchive Ar(MemoryWriter, false);
	UEngine::FCopyPropertiesForUnrelatedObjectsParams CopyPropertiesForUnrelatedObjectsParams;
	CopyPropertiesForUnrelatedObjectsParams.bDoDelta = false;
	UEngine::CopyPropertiesForUnrelatedObjects(CameraComp, DuplicateCameraComp, CopyPropertiesForUnrelatedObjectsParams);
	DuplicateCameraComp->Serialize(Ar);
	
	CameraData.object = FBase64::Encode(Bytes);

	return CameraData;
}

FString UPakExportUtilityRuntime::GetPakMountPoint(const FString& PakFilename)
{
	if (PakFilename.IsEmpty()) return {};

	const TRefCountPtr PakFile{new FPakFile(GetPakPlatformFile(), *PakFilename, false)};

	if (const FPakFile* Pak = PakFile.GetReference(); Pak->IsValid())
	{
		auto MountPoint{Pak->GetMountPoint()};

		if (TArray<FString> Array; MountPoint.ParseIntoArray(Array, TEXT("/")) == 3)
		{
			for (FPakFile::FFileIterator It(*Pak); It; ++It)
			{
				const auto FolderPath{FPaths::GetPath(It.Filename())};
				FolderPath.ParseIntoArray(Array, TEXT("/"));
				if (Array.Num() == 3 && Array[0] == "PakExport" && Array[1] == "Plugins")
				{
					for (const auto& Folder : Array)
						MountPoint += Folder + TEXT("/");
					break;
				}
			}
		}
		
		return MountPoint;
	}
	
	return {};
}

FString UPakExportUtilityRuntime::GetPakGuid(const FString& PakFilename)
{
	if (PakFilename.IsEmpty()) return {};
	
	const auto MountPoint{GetPakMountPoint(PakFilename)};
	TArray<FString> Array;
	if (MountPoint.ParseIntoArray(Array, TEXT("/")) == 0) return {};

	constexpr auto GuidFormat{EGuidFormats::Base36Encoded};
	FGuid Guid;
	if (!FGuid::ParseExact(Array.Last(), GuidFormat, Guid)) return {};

	return Guid.ToString(GuidFormat);
}

FPakPlatformFile* UPakExportUtilityRuntime::GetPakPlatformFile()
{
	FPakPlatformFile* PakPlatformFile;

	/*
		Packaged shipping builds will have a PakFile platform.
		For other build types a new pak platform file will be created.
	*/
	if (const auto CurrentPlatformFile = FPlatformFileManager::Get().FindPlatformFile(TEXT("PakFile")))
	{
		PakPlatformFile = static_cast<FPakPlatformFile*>(CurrentPlatformFile);
	}
	else
	{
		PakPlatformFile = new FPakPlatformFile();

		ensure(PakPlatformFile != nullptr);

		IPlatformFile &PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

		if (PakPlatformFile->Initialize(&PlatformFile, TEXT("")))
		{
			FPlatformFileManager::Get().SetPlatformFile(*PakPlatformFile);
		}
	}

	ensure(PakPlatformFile != nullptr);
	return PakPlatformFile;
}
