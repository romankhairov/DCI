#include "CommandsHelpers.h"

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

FVectorJson::FVectorJson()
{
}

FVectorJson::FVectorJson(const FVector Vec) : x(Vec.X), y(Vec.Y), z(Vec.Z)
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

//Payloads

FPayloadJson::FPayloadJson()
{
	switch (FEngineVersion::Current().GetMajor())
	{
	case 4: unrealVersion = "V_4"; break;
	case 5: unrealVersion = "V_5"; break;
	default: break;
	}
}

FSelectProductPayloadJson::FSelectProductPayloadJson()
{
	type = EAssetType::PRODUCT;
}

FLoadLevelPayloadJson::FLoadLevelPayloadJson()
{
	type = EAssetType::LEVEL;
}

FSetMaterialPayloadJson::FSetMaterialPayloadJson()
{
	type = EAssetType::MATERIAL;
}

FInitSequencePayloadJson::FInitSequencePayloadJson()
{
	type = EAssetType::LEVEL_SEQUENCE;
}

FApplyCameraPresetPayloadJson::FApplyCameraPresetPayloadJson()
{
	type = EAssetType::CAMERA;
}
