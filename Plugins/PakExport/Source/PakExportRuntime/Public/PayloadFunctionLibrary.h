// Copyright 2024 3D Source, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PayloadsBase.h"
#include "Dom/JsonObject.h"
#include "PayloadFunctionLibrary.generated.h"


/**
 * 
 */

UENUM(BlueprintType)
enum class EValidation : uint8
{
	IsValid,
	IsNotValid
};

UCLASS()
class PAKEXPORTRUNTIME_API UPayloadFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

// ----------------------- Convertation  --------------------------------
#pragma region ConvertationRegion
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Convert Vector2DJson to Vector2D", CompactNodeTitle = "->", BlueprintAutocast), Category = Payload)
	static FVector2D Vector2DJsonToVector2D(FVector2DJson Vector2DJson) { return Vector2DJson.Vector2DJsonToVector2D(); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Convert Vector2D to Vector2DJson", CompactNodeTitle = "->", BlueprintAutocast), Category = Payload)
	static FVector2DJson Vector2DToVector2DJson(FVector2D Vector2D) { return FVector2DJson(Vector2D); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Convert IntPointJson to IntPoint", CompactNodeTitle = "->", BlueprintAutocast), Category = Payload)
	static FIntPoint IntPointJsonToIntPoint(FIntPoint2DJson IntPoint2DJson) { return IntPoint2DJson.IntPointJsonToIntPoint(); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Convert IntPoint to IntPointJson", CompactNodeTitle = "->", BlueprintAutocast), Category = Payload)
	static FIntPoint2DJson IntPointToIntPoint2DJson(FIntPoint IntPoint) { return FIntPoint2DJson(IntPoint); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Convert RotatorJson to Rotator", CompactNodeTitle = "->", BlueprintAutocast), Category = Payload)
	static FRotator RotatorJsonToRotator(FRotatorJson RotatorJson) { return RotatorJson.RotatorJsonToRotator(); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Convert Rotator to RotatorJson", CompactNodeTitle = "->", BlueprintAutocast), Category = Payload)
	static FRotatorJson RotatorToRotatorJson(FRotator Rotator) { return FRotatorJson(Rotator); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Convert VectorJson to Vector", CompactNodeTitle = "->", BlueprintAutocast), Category = Payload)
	static FVector VectorJsonToVector(FVectorJson VectorJson) { return VectorJson.VectorJsonToVector(); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Convert Vector to VectorJson", CompactNodeTitle = "->", BlueprintAutocast), Category = Payload)
	static FVectorJson VectorToVectorJson(FVector Vector) { return FVectorJson(Vector); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Convert TransformJson to Tranform", CompactNodeTitle = "->", BlueprintAutocast), Category = Payload)
	static FTransform TransformJsonToTransform(FTransformJson TransformJson) { return TransformJson.TransformJsonToTransform(); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Convert Transform to TranformJson", CompactNodeTitle = "->", BlueprintAutocast), Category = Payload)
	static FTransformJson TransformToTransformJson(FTransform Transform) { return FTransformJson(Transform); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Convert LinearColorJson to LineraColor", CompactNodeTitle = "->", BlueprintAutocast), Category = Payload)
	static FLinearColor LinearColorJsonToLinearColor(FLinearColorJson LinearColorJson) { return LinearColorJson.LinearColorJsonToLinearColor(); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Convert LinearColor to LineraColorJson", CompactNodeTitle = "->", BlueprintAutocast), Category = Payload)
	static FLinearColorJson LinearColorToLinearColorJson(FLinearColor LinearColor) { return FLinearColor(LinearColor); }

#pragma endregion	

// ----------------------- Payload  --------------------------------
#pragma region PayloadRegion

public:
	//DeprecatedFunction, DeprecationMessage="We don't want to operate raw json strings!"
	UFUNCTION(BlueprintPure, Category = "Configurator", meta = (DisplayName = "GetPayloadStringFromDescriptor"))
	static UPARAM(DisplayName = "Success") bool GetPayloadStringFromDescriptor(const FString Descriptor, FString& PayloadString);

	//DeprecatedFunction, DeprecationMessage="We don't want to operate raw json strings!"
	UFUNCTION(BlueprintPure, Category = "Configurator", meta = (DisplayName = "GetCommandNameFromDescriptor"))
	static UPARAM(DisplayName = "Success") bool GetCommandNameFromDescriptor(const FString Descriptor, FString& CommandName);

	UFUNCTION(BlueprintPure, Category = "Configurator", CustomThunk, meta = (CustomStructureParam = "JsonStruct,Field", DisplayName = "IsJsonFieldValid", UnsafeDuringActorConstruction = "true"))
	static void IsJsonFieldValidPure(const UField*& Field, bool& Result, const UStruct*& JsonStruct);
	DECLARE_FUNCTION(execIsJsonFieldValidPure);

	UFUNCTION(BlueprintCallable, Category = "Configurator", CustomThunk, meta = (ExpandEnumAsExecs = "Validation", CustomStructureParam = "JsonStruct,Field", DisplayName = "IsJsonFieldValid", UnsafeDuringActorConstruction = "true"))
	static void IsJsonFieldValid(const UField*& Field, EValidation& Validation, const UStruct*& JsonStruct);
	DECLARE_FUNCTION(execIsJsonFieldValid);

	UFUNCTION(BlueprintPure, Category = "Configurator", CustomThunk, meta = (CustomStructureParam = "JsonStructA,JsonStructB", DisplayName = "IsEqualJsonStruct", UnsafeDuringActorConstruction = "true", CompactNodeTitle = "=="))
	static bool IsEqualJsonStruct(const int32& JsonStructA, const int32& JsonStructB);
	DECLARE_FUNCTION(execIsEqualJsonStruct);

	static FString JsonSerialize(const TSharedRef<FJsonObject>& Json);
	static TSharedPtr<FJsonObject> JsonDeserialize(const FString& String);
	
	static bool UStructToJsonObject(const UStruct* InStructDefinition, const void* InStructPtr, const TSharedRef<FJsonObject>& OutJsonObject);
	static bool UStructToJsonObjectString(const UStruct* InStructDefinition, const void* InStructPtr, FString& OutJsonString);
	
private:
	static void GenericIsJsonFieldValidPure(const FJson& JsonStruct, UStruct* JsonStructDef, bool& Result, void* PropertyPtr, const FProperty* Property);
	static void GenericIsJsonFieldValid(const FJson& JsonStruct, EValidation& Validation, const FProperty* Property);

#pragma endregion
};
