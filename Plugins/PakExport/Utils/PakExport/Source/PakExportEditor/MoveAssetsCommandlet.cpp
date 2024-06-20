// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveAssetsCommandlet.h"
#include "PakExportEditorUtility.h"
#include "AssetRegistry/AssetRegistryModule.h"

int32 UMoveAssetsCommandlet::Main(const FString& Params)
{
    Super::Main(Params);

    const auto& AssetRegistryModule = FModuleManager::Get().LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
    auto& AssetRegistry = AssetRegistryModule.Get();
    AssetRegistry.SearchAllAssets(true);
    while (AssetRegistry.IsLoadingAssets())
    {
        AssetRegistry.Tick(1.0f);
    }

    TArray<FString> OutTokens;
    TArray<FString> OutSwitches;
    TMap<FString, FString> OutParams;
    ParseCommandLine(*Params, OutTokens, OutSwitches, OutParams);
    const auto Guid{OutParams["Guid"]};

    return UPakExportEditorUtility::MoveAssets(Guid);
}
