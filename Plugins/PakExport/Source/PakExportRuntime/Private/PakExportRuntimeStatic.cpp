// Fill out your copyright notice in the Description page of Project Settings.


#include "PakExportRuntimeStatic.h"
#include "PakExportUtilityRuntime.h"


FString UPakExportRuntimeStatic::GetPakExportName(const FString& PakFile)
{
	const auto Guid{UPakExportUtilityRuntime::GetPakGuid(PakFile)};
	
	return Guid.IsEmpty() ? PakExportName : Guid;
}
