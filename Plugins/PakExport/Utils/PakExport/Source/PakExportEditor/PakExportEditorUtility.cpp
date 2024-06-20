// Fill out your copyright notice in the Description page of Project Settings.

#include "PakExportEditorUtility.h"

#include "AssetToolsModule.h"
#include "AssetViewUtils.h"
#include "EditorAssetLibrary.h"
#if ENGINE_MAJOR_VERSION > 4
#include "Misc/ScopedSlowTask.h"
#include "Internationalization/Internationalization.h"
#endif
#include "AssetRegistry/AssetRegistryModule.h"

#if ENGINE_MAJOR_VERSION > 4
#define LOCTEXT_NAMESPACE "PakExport"
#endif

int32 UPakExportEditorUtility::MoveAssets(const FString& Guid)
{
	const auto AllPackages{GetAllPackages()};

	for (const auto& Package : AllPackages)
	{
		TArray<FString> Array;
		Package.ParseIntoArray(Array, TEXT("/"));
		Array.RemoveAt(Array.Num() - 1);
		FString TmpPackage{"/"};
		for (const auto& Item : Array) TmpPackage += Item + "/";
		TmpPackage += FGuid::NewGuid().ToString(EGuidFormats::Base36Encoded);
		
		UEditorAssetLibrary::RenameAsset(Package, TmpPackage);
		FixUpRedirectors();
		UEditorAssetLibrary::RenameAsset(TmpPackage, Package);
		FixUpRedirectors();
	}
	
	TArray<FString> Folders;
	TArray<FString> Files;
	
	for (const auto& Package : AllPackages)
	{
		TArray<FString> Array;
		Package.ParseIntoArray(Array, TEXT("/"));

		const auto& Item{Array[1]};

		if (Array.Num() == 2 && Package != "/Game/Minimal_Default")
		{
			Files.AddUnique(Item);
		}
		else if (Array.Num() > 2)
		{
			Folders.AddUnique(Item);
		}
	}
    
	if (Files.Num() == 0 && Folders.Num() == 0) return 0;

	if (Folders.Num() > 0)
	{
		const auto& Folder{Folders[0]};
		const auto SourceFolder{"/Game/" + Folder};
		
		UEditorAssetLibrary::RenameDirectory(SourceFolder, "/" + Guid + "/" + Folder);

		FixUpRedirectors();

		UEditorAssetLibrary::DeleteDirectory(SourceFolder);
	}
	else if (Files.Num() > 0)
	{
		const auto File{Files[0]};

		UEditorAssetLibrary::RenameAsset("/Game/" + File, "/" + Guid + "/" + File);

		FixUpRedirectors();
	}
	
	return 1;
}

TArray<FString> UPakExportEditorUtility::GetAllPackages()
{
	TArray<FString> Packages;

	auto AllAssets{UEditorAssetLibrary::ListAssets("/Game/")};

	for (const auto& Asset : AllAssets)
	{
		TArray<FString> Array;
		Asset.ParseIntoArray(Array, TEXT("."));
		Packages.AddUnique(Array[0]);
	}

	return Packages;
}

void UPakExportEditorUtility::FixUpRedirectors(const TArray<FString>& SelectedPaths/* = {"/Game"}*/)
{
#if ENGINE_MAJOR_VERSION > 4
	TArray<FString> SelectedPackages;

	FAssetRegistryModule& AssetRegistryModule = FModuleManager::Get().LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
	
	// Form a filter from the paths
	FARFilter Filter;
	Filter.bRecursivePaths = true;
	
	Filter.PackagePaths.Reserve(SelectedPaths.Num());
	for (const FString& Path : SelectedPaths)
	{
	    Filter.PackagePaths.Emplace(*Path);
	}
	
	if (!SelectedPaths.IsEmpty())
	{
	    Filter.ClassPaths.Add(UObjectRedirector::StaticClass()->GetClassPathName());
	}
	    
	// Query for a list of assets in the selected paths
	TArray<FAssetData> AssetList;
	AssetRegistryModule.Get().GetAssets(Filter, AssetList);
	
	Filter.Clear();
	
	Filter.PackageNames.Reserve(SelectedPackages.Num());
	for (const FString& PackageName : SelectedPackages)
	{
	    Filter.PackageNames.Emplace(*PackageName);
	}
	
	if (!SelectedPackages.IsEmpty())
	{
	    Filter.ClassPaths.Add(UObjectRedirector::StaticClass()->GetClassPathName());
	}
	
	AssetRegistryModule.Get().GetAssets(Filter, AssetList);
	
	if (AssetList.Num() == 0)
	{
	    return;
	}
	    
	TArray<FString> ObjectPaths;
	for (const FAssetData& Asset : AssetList)
	{
	    ObjectPaths.Add(Asset.GetObjectPathString());
	}
	
	FScopedSlowTask SlowTask(3, LOCTEXT("FixupRedirectorsSlowTask", "Fixing up redirectors"));
	SlowTask.MakeDialog();
	
	TArray<UObject*> Objects;
	const bool bAllowedToPromptToLoadAssets = true;
	const bool bLoadRedirects = true;
	SlowTask.EnterProgressFrame(1, LOCTEXT("FixupRedirectors_LoadAssets", "Loading Assets..."));
	if (AssetViewUtils::LoadAssetsIfNeeded(ObjectPaths, Objects, bAllowedToPromptToLoadAssets, bLoadRedirects))
	{
	    // Transform Objects array to ObjectRedirectors array
	    TArray<UObjectRedirector*> Redirectors;
	    for (UObject* Object : Objects)
	    {
	        Redirectors.Add(CastChecked<UObjectRedirector>(Object));
	    }
	
	    SlowTask.EnterProgressFrame(1, LOCTEXT("FixupRedirectors_FixupReferencers", "Fixing up referencers..."));
	    // Load the asset tools module
	    FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>(TEXT("AssetTools"));
	    AssetToolsModule.Get().FixupReferencers(Redirectors);
	}
#else
	const FAssetRegistryModule& AssetRegistryModule = FModuleManager::Get().LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));

	// Form a filter from the paths
	FARFilter Filter;
	Filter.bRecursivePaths = true;
	for (const auto& Path : SelectedPaths)
	{
		Filter.PackagePaths.Emplace(*Path);
		Filter.ClassNames.Emplace(TEXT("ObjectRedirector"));
	}

	// Query for a list of assets in the selected paths
	TArray<FAssetData> AssetList;
	AssetRegistryModule.Get().GetAssets(Filter, AssetList);

	if (AssetList.Num() > 0)
	{
		TArray<FString> ObjectPaths;
		for (const auto& Asset : AssetList)
		{
			ObjectPaths.Add(Asset.ObjectPath.ToString());
		}

		TArray<UObject*> Objects;
		const bool bAllowedToPromptToLoadAssets = true;
		const bool bLoadRedirects = true;
		if (AssetViewUtils::LoadAssetsIfNeeded(ObjectPaths, Objects, bAllowedToPromptToLoadAssets, bLoadRedirects))
		{
			// Transform Objects array to ObjectRedirectors array
			TArray<UObjectRedirector*> Redirectors;
			for (auto Object : Objects)
			{
				auto Redirector = CastChecked<UObjectRedirector>(Object);
				Redirectors.Add(Redirector);
			}

			// Load the asset tools module
			FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>(TEXT("AssetTools"));
			AssetToolsModule.Get().FixupReferencers(Redirectors);
		}
	}
#endif
}
