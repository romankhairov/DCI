// Copyright 2024 3D Source, Inc. All Rights Reserved.


#include "PayloadFunctionLibrary.h"
#include "JsonObjectConverter.h"
#include "Policies/CondensedJsonPrintPolicy.h"

// ----------------------- Payload  --------------------------------
#pragma region PayloadRegion

bool UPayloadFunctionLibrary::GetPayloadStringFromDescriptor(const FString Descriptor, FString& PayloadString)
{
	TSharedPtr<FJsonObject> JsonObject;
	if (!FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(Descriptor), JsonObject) || !JsonObject.IsValid())
	{
		return false;
	}

	if (!JsonObject->HasTypedField<EJson::Object>(FString{ "payload" }))
	{
		return false;
	}

	const auto PayloadJsonObject{ JsonObject->GetObjectField(FString{ "payload" }) };

	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> JsonWriter = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&PayloadString, 0);
	FJsonSerializer::Serialize(PayloadJsonObject.ToSharedRef(), JsonWriter);

	return !PayloadString.IsEmpty() && PayloadString != FString{ "{}" };
}

bool UPayloadFunctionLibrary::GetCommandNameFromDescriptor(const FString Descriptor, FString& CommandName)
{
	TSharedPtr<FJsonObject> JsonObject;
	if (!FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(Descriptor), JsonObject) || !JsonObject.IsValid())
	{
		return false;
	}

	if (!JsonObject->HasTypedField<EJson::String>(FString{ "command" }))
	{
		return false;
	}
	const TSharedPtr<FJsonObject>* CommandNameJsonObjectPtr = &JsonObject;
	const auto CommandNameJsonObject{ JsonObject->TryGetStringField(FString{ "command" }, CommandName) };

	return !CommandName.IsEmpty() && CommandName != FString{ "{}" };
}

void UPayloadFunctionLibrary::IsJsonFieldValidPure(const UField*& Field, bool& Result, const UStruct*& JsonStruct)
{
	// We should never hit these!  They're stubs to avoid NoExport on the class.  Call the Generic* equivalent instead
	check(0);
}

DEFINE_FUNCTION(UPayloadFunctionLibrary::execIsJsonFieldValidPure)
{
	Stack.StepCompiledIn<FProperty>({});
	const auto Field{Stack.MostRecentProperty};
	const auto FieldPtr{Stack.MostRecentPropertyAddress};
	
	P_GET_UBOOL_REF(Result)

	Stack.Step(Stack.Object, nullptr);

	const auto StructProp{CastField<FStructProperty>(Stack.MostRecentProperty)};
	ensureAlwaysMsgf(StructProp && StructProp->Struct->IsChildOf(FJson::StaticStruct()), TEXT("Input is not FJson! Object: %s"), *Stack.Object->GetName());
	
	const auto JsonStructDef = StructProp ? StructProp->Struct : nullptr;
	const auto JsonStruct = StructProp ? (FJson*)Stack.MostRecentPropertyAddress : nullptr;

	P_FINISH;

	P_NATIVE_BEGIN

	if (StructProp) GenericIsJsonFieldValidPure(*JsonStruct, JsonStructDef, Result, FieldPtr, Field);

	P_NATIVE_END
}

void UPayloadFunctionLibrary::IsJsonFieldValid(const UField*& Field, EValidation& Validation, const UStruct*& JsonStruct)
{
	// We should never hit these!  They're stubs to avoid NoExport on the class.  Call the Generic* equivalent instead
	check(0);
}

DEFINE_FUNCTION(UPayloadFunctionLibrary::execIsJsonFieldValid)
{
	Stack.StepCompiledIn<FProperty>({});
	const auto Field{Stack.MostRecentProperty};
	
	P_GET_ENUM_REF(EValidation, Validation)

	Stack.Step(Stack.Object, nullptr);

	const auto StructProp{CastField<FStructProperty>(Stack.MostRecentProperty)};
	ensureAlwaysMsgf(StructProp && StructProp->Struct->IsChildOf(FJson::StaticStruct()), TEXT("Input is not FJson! Object: %s"), *Stack.Object->GetName());
	
	const auto JsonStruct = StructProp ? (FJson*)Stack.MostRecentPropertyAddress : nullptr;

	P_FINISH;

	P_NATIVE_BEGIN

	if (StructProp) GenericIsJsonFieldValid(*JsonStruct, Validation, Field);

	P_NATIVE_END
}

bool UPayloadFunctionLibrary::IsEqualJsonStruct(const int32& JsonStructA, const int32& JsonStructB)
{
	// We should never hit these!  They're stubs to avoid NoExport on the class.  Call the Generic* equivalent instead
	check(0);
	return false;
}

DEFINE_FUNCTION(UPayloadFunctionLibrary::execIsEqualJsonStruct)
{
	Stack.StepCompiledIn<FProperty>(nullptr);
	const void* JsonStructA = Stack.MostRecentPropertyAddress;
	Stack.StepCompiledIn<FProperty>(nullptr);
	const void* JsonStructB = Stack.MostRecentPropertyAddress;
	FProperty* ValueProperty = Stack.MostRecentProperty;
	const auto StructProperty = CastField<FStructProperty>(ValueProperty);

	P_FINISH;

	bool bResult;

	P_NATIVE_BEGIN
	bResult = StructProperty->Struct->CompareScriptStruct(JsonStructA, JsonStructB, 0);
	P_NATIVE_END
	*StaticCast<bool*>(RESULT_PARAM) = bResult;
}

FString UPayloadFunctionLibrary::JsonSerialize(const TSharedRef<FJsonObject>& Json)
{
	FString String;
	const auto Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&String);
	FJsonSerializer::Serialize(Json, Writer);
	return String;
}

TSharedPtr<FJsonObject> UPayloadFunctionLibrary::JsonDeserialize(const FString& String)
{
	TSharedPtr<FJsonObject> Json;
	FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(String), Json);
	return Json;
}

bool UPayloadFunctionLibrary::UStructToJsonObject(const UStruct* InStructDefinition, const void* InStructPtr, const TSharedRef<FJsonObject>& OutJsonObject)
{
	if (!InStructDefinition->IsChildOf(FJson::StaticStruct())) return false;

	if (!FJsonObjectConverter::UStructToJsonObject(InStructDefinition, InStructPtr, OutJsonObject)) return false;

	TFunction<void(const UStruct*, const void*, const TSharedRef<FJsonObject>&)> FillNullJsonObjects
		= [&FillNullJsonObjects](const UStruct* StructDefinition, const void* StructPtr, const TSharedRef<FJsonObject>& JsonObject)
	{
		const auto Json{(FJson*)StructPtr};

		for (TFieldIterator<FProperty> PropIt(StructDefinition); PropIt; ++PropIt)
		{
			const auto Property{*PropIt};
			const auto PropertyName{Property->GetName()};

			if (const auto Found{JsonObject->Values.Find(PropertyName)})
			{
				if (const TSharedPtr<FJsonObject>* SubJsonObject{}; Found->Get()->TryGetObject(SubJsonObject))
				{
					if (const auto Struct{CastField<FStructProperty>(Property)};
						ensureMsgf(Struct, TEXT("Field %s is not a struct!"), *PropertyName) &&
						ensureMsgf(Struct->Struct->IsChildOf(FJson::StaticStruct()), TEXT("Struct %s is not a FJson!"), *PropertyName))
					{
						if (const auto PropertyValue{Property->ContainerPtrToValuePtr<FJson>(StructPtr)}; !PropertyValue->IsValid())
							JsonObject->SetField(PropertyName, MakeShared<FJsonValueNull>());
						else
							FillNullJsonObjects(Struct->Struct, PropertyValue, SubJsonObject->ToSharedRef());
					}
				}
				else if (const TArray<TSharedPtr<FJsonValue>>* JsonArray{}; Found->Get()->TryGetArray(JsonArray) && JsonArray->Num() > 0)
				{
					if (const TSharedPtr<FJsonObject>* TestObject{}; (*JsonArray)[0]->TryGetObject(TestObject))
						if (const auto Array{CastField<FArrayProperty>(Property)}; ensureMsgf(Array, TEXT("Field %s is not a array!"), *PropertyName) && ensureMsgf(Array->Inner->IsA(FStructProperty::StaticClass()), TEXT("Array %s elements are not structs!"), *PropertyName))
							if (FScriptArrayHelper ArrayHelper(Array, Property->ContainerPtrToValuePtr<void>(StructPtr)); ensureMsgf(JsonArray->Num() == ArrayHelper.Num(), TEXT("Json and struct arrays are not same!")))
								for (const auto& E : *JsonArray)
									if (const TSharedPtr<FJsonObject>* Object{}; E->TryGetObject(Object))
										if (const auto StrProp{CastField<FStructProperty>(Array->Inner)})
											FillNullJsonObjects(StrProp->Struct, ArrayHelper.GetRawPtr(&E - &(*JsonArray)[0]), Object->ToSharedRef());
				}
			}
		}
	};

	FillNullJsonObjects(InStructDefinition, InStructPtr, OutJsonObject);

	return true;
}

bool UPayloadFunctionLibrary::UStructToJsonObjectString(const UStruct* InStructDefinition, const void* InStructPtr, FString& OutJsonString)
{
	const auto JsonObject{MakeShared<FJsonObject>()};
	if (!UStructToJsonObject(InStructDefinition, InStructPtr, JsonObject)) return false;

	OutJsonString = JsonSerialize(JsonObject);

	return true;
}

void UPayloadFunctionLibrary::GenericIsJsonFieldValidPure(const FJson& JsonStruct, UStruct* JsonStructDef, bool& Result, void* PropertyPtr, const FProperty* Property)
{
	//if (JsonStruct.NoValidate)
	//{
	//	Result = true;
	//	return;
	//}

	if (const auto Found{JsonStruct.FieldsDescriptions.Find(Property->GetFName())})
		Result = Found->Valid;
	else
		Result = true;
}

void UPayloadFunctionLibrary::GenericIsJsonFieldValid(const FJson& JsonStruct, EValidation& Validation,	const FProperty* Property)
{
	//if (JsonStruct.NoValidate)
	//{
	//	Validation = EValidation::IsValid;
	//	return;
	//}

	Validation = EValidation::IsNotValid;
	if (const auto Found{JsonStruct.FieldsDescriptions.Find(Property->GetFName())})
	{
		Validation = Found->Valid ? EValidation::IsValid : EValidation::IsNotValid;
	}
	else
	{
		Validation = EValidation::IsValid;
	}
}

#pragma endregion
