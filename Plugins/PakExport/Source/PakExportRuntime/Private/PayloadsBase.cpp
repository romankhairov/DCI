#include "PayloadsBase.h"

// ----------------------- VARIABLES JSON --------------------------------
#pragma region VariablesJsonRegion

FVector2DJson::FVector2DJson()
{
}

FVector2DJson::FVector2DJson(const FVector2D& Vector2D) : x(Vector2D.X), y(Vector2D.Y)
{
}

FVector2DJson::FVector2DJson(float x, float y) : x(x), y(y)
{
}

FVector2D FVector2DJson::Vector2DJsonToVector2D()
{
	return FVector2D(x, y);
}

FIntPoint FIntPoint2DJson::IntPointJsonToIntPoint()
{
	return FIntPoint(x,y);
}

FIntPoint2DJson::FIntPoint2DJson()
{
}

FIntPoint2DJson::FIntPoint2DJson(const FIntPoint& IntPoint) : x(IntPoint.X), y(IntPoint.Y)
{
}

FIntPoint2DJson::FIntPoint2DJson(int32 x, int32 y)	: x(x), y(y)
{
}

FRotatorJson::FRotatorJson()
{
	
}

FRotatorJson::FRotatorJson(const FRotator& Rotator)
	: Pitch(Rotator.Pitch), Yaw(Rotator.Yaw), Roll(Rotator.Roll)
{
	
}

FRotatorJson::FRotatorJson(FRotator&& Rotator)
	: Pitch(Rotator.Pitch), Yaw(Rotator.Yaw), Roll(Rotator.Roll)
{
	
}

FRotatorJson::FRotatorJson(float InPitch, float InYaw, float InRoll)
	: Pitch(InPitch), Yaw(InYaw), Roll(InRoll)
{

}

FRotator FRotatorJson::RotatorJsonToRotator()
{
	return FRotator(Pitch, Yaw, Roll);
}

FVector FVectorJson::VectorJsonToVector()
{
	return FVector(x, y, z);
}

FVectorJson::FVectorJson()
{
}

FVectorJson::FVectorJson(const FVector& Vec) : x(Vec.X), y(Vec.Y), z(Vec.Z)
{
}

FVectorJson::FVectorJson(float x, float y, float z)
	: x(x), y(y), z(z)
{
}

FTransformJson::FTransformJson()
{
}

FTransformJson::FTransformJson(const FTransform& Transform)
	: rotation(Transform.Rotator()), translation(Transform.GetTranslation()), Scale3D(Transform.GetScale3D())
{
	
}

FTransformJson::FTransformJson(FTransform&& Transform)
: rotation(Transform.Rotator()), translation(Transform.GetTranslation()), Scale3D(Transform.GetScale3D())
{
	
}

FTransform FTransformJson::TransformJsonToTransform()
{
	return FTransform(rotation.RotatorJsonToRotator(), translation.VectorJsonToVector(), Scale3D.VectorJsonToVector());
}

FLinearColor FLinearColorJson::LinearColorJsonToLinearColor()
{
	return FLinearColor(r,g,b,a);
}

FLinearColorJson::FLinearColorJson()
{
}

FLinearColorJson::FLinearColorJson(const FLinearColor& Color) : r(Color.R), g(Color.G), b(Color.B), a(Color.A)
{
}

FLinearColorJson::FLinearColorJson(float r, float g, float b, float a) : r(r), g(g), b(b), a(a)
{
}

#pragma endregion

// ----------------------- BASE PAYLOADS --------------------------------
#pragma region BasePayloadsRegion

FFieldDescription::FFieldDescription()
{
}

bool FJson::IsValid() const
{
	return true;
}

FPayloadBase::FPayloadBase()
{
	switch (FEngineVersion::Current().GetMajor())
	{
	case 4: unrealVersion = "V_4"; break;
	case 5: unrealVersion = "V_5"; break;
	default: break;
	}
}

FBoolPayload::FBoolPayload()
{
}

FFloatPayload::FFloatPayload()
{
}

FFVectorPayload::FFVectorPayload()
{
}

FFStringPayload::FFStringPayload()
{
}

#pragma endregion

// ----------------------- ASSETS --------------------------------
#pragma region AssetsRegion

FProductPayload::FProductPayload()
{
	type = EAssetType::product;
}

bool FProductPayload::IsValid() const
{
	return (!meshPak.initialState.meshName.IsEmpty() && !meshPak.pakFilePath.IsEmpty()) ||
		!datasmithFilePath.IsEmpty() || !meshPak.usd_path.IsEmpty() ||
		(FParse::Param(FCommandLine::Get(), TEXT("Debug")) && !objectName.IsEmpty());
}

bool FProductPayload::operator==(const FProductPayload& Other) const
{
	ensureMsgf(!Other.objectName.IsEmpty() && !objectName.IsEmpty(), TEXT("objectName must be not empty!"));
	return Other.objectName == objectName;
}

bool FProductPayload::operator==(const FString& ObjectName) const
{
	ensureMsgf(!ObjectName.IsEmpty() && !objectName.IsEmpty(), TEXT("objectName must be not empty!"));
	return ObjectName == objectName;
}

FLevelPayload::FLevelPayload()
{
	type = EAssetType::environment;
}

bool FLevelPayload::operator==(const FLevelPayload& Other) const
{
	return Other.levelPak.assetName == levelPak.assetName;
}

bool FLevelPayload::operator==(const FString& Name) const
{
	return Name == levelPak.assetName;
}

FMaterialPayload::FMaterialPayload()
{
	type = EAssetType::material;
}

#pragma endregion

// ----------------------- CONTROLS CAMERA --------------------------------
#pragma region ControlsCameraRegion

FCameraSettingsPayload::FCameraSettingsPayload()
{
	type = EAssetType::camera;
}

FCameraPresetBasePayload::FCameraPresetBasePayload()
{
	type = EAssetType::camera;
}

#pragma endregion

FInitSequencePayload::FInitSequencePayload()
{
	type = EAssetType::level_sequence;
}

FTempCommandPayload::FTempCommandPayload()
{
}
