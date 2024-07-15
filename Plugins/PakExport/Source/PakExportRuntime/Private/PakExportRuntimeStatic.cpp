// Copyright 2024 3D Source, Inc. All Rights Reserved.


#include "PakExportRuntimeStatic.h"
#include "PakExportUtilityRuntime.h"


FString UPakExportRuntimeStatic::GetPakExportName(const FString& PakFile)
{
	const auto Guid{UPakExportUtilityRuntime::GetPakGuid(PakFile)};
	
	return Guid.IsEmpty() ? PakExportName : Guid;
}
