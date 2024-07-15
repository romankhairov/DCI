// Copyright 2024 3D Source, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PakExportRuntimeStatic.h"
#include "PayloadsBase.generated.h"

USTRUCT()
struct FFieldDescription
{
	GENERATED_BODY()
public:
	FFieldDescription();
	explicit FFieldDescription(const bool Valid) : Valid(Valid) {}
	bool Valid = false;
};

USTRUCT(BlueprintType)
struct FJson
{
	GENERATED_BODY()
public:
	bool IsValid() const;
	
	TMap<FName, FFieldDescription> FieldsDescriptions;
};

// ----------------------- VARIABLES JSON --------------------------------
#pragma region VariablesJsonRegion

/// { "Description" : "A vector in 2-D space composed of components (X, Y) with floating point precision." }
USTRUCT(BlueprintType)
struct PAKEXPORTRUNTIME_API FVector2DJson : public FJson
{
	GENERATED_BODY()
public:

	FVector2DJson();
	FVector2DJson(const FVector2D& Vector2D);
	FVector2DJson(float x, float y);

	FVector2D Vector2DJsonToVector2D();

	/// { "Description" : "Vector's X component." }
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Payload|Json") float x = 0.0f;
	/// { "Description" : "Vector's Y component." }
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Payload|Json") float y = 0.0f;
};

/// { "Description" : "Structure for integer points in 2-d space." }
USTRUCT(BlueprintType)
struct PAKEXPORTRUNTIME_API FIntPoint2DJson : public FJson
{
	GENERATED_BODY()
public:
	FIntPoint2DJson();
	FIntPoint2DJson(const FIntPoint& IntPoint);
	FIntPoint2DJson(int32 x, int32 y);

	FIntPoint IntPointJsonToIntPoint();

	/// { "Description" : "Holds the point's X-coordinate." }
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Payload|Json") int32 x = 0;
	/// { "Description" : "Holds the point's Y-coordinate." }
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Payload|Json") int32 y = 0;
};

/// { "Description" : "All rotation values are stored in degrees." }
USTRUCT(BlueprintType)
struct PAKEXPORTRUNTIME_API FRotatorJson : public FJson
{
	GENERATED_BODY()
public:
	FRotatorJson();
	FRotatorJson(const FRotator& Rotator);
	FRotatorJson(FRotator&& Rotator);
	FRotatorJson(float InPitch, float InYaw, float InRoll);

	FRotator RotatorJsonToRotator();

	/// { "Description" : "Rotation around the right axis (around Y axis), Looking up and down (0=Straight Ahead, +Up, -Down)" }
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Payload|Json") float Pitch = 0.0f;
	/// { "Description" : "Rotation around the up axis (around Z axis), Running in circles 0=East, +North, -South." }
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Payload|Json") float Yaw = 0.0f;
	/// { "Description" : "Rotation around the forward axis (around X axis), Tilting your head, 0=Straight, +Clockwise, -CCW." }
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Payload|Json") float Roll = 0.0f;
	
	/// {
	/// "DEPRECATED" : "Use Yaw, Pitch, Roll",
	/// "Optional" : true
	/// }
	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, Category = "Payload|Json") float x = 0.0f;
	/// {
	/// "DEPRECATED" : "Use Yaw, Pitch, Roll",
	/// "Optional" : true
	/// }
	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, Category = "Payload|Json") float y = 0.0f;
	/// {
	/// "DEPRECATED" : "Use Yaw, Pitch, Roll",
	/// "Optional" : true
	/// }
	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, Category = "Payload|Json") float z = 0.0f;
	/// {
	/// "DEPRECATED" : "Use Yaw, Pitch, Roll",
	/// "Optional" : true
	/// }
	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, Category = "Payload|Json") float w = 0.0f;
};

/// { "Description" : "A vector in 3-D space composed of components (X, Y, Z) with floating point precision." }
USTRUCT(BlueprintType)
struct PAKEXPORTRUNTIME_API FVectorJson : public FJson
{
	GENERATED_BODY()
public:
	FVectorJson();
	FVectorJson(const FVector& Vec);
	FVectorJson(float x, float y, float z);

	FVector VectorJsonToVector();
	
	/// { "Description" : "Vector's X component." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json") float x = 0.0f;
	/// { "Description" : "Vector's Y component." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json") float y = 0.0f;
	/// { "Description" : "Vector's Z component." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json") float z = 0.0f;
};

/// { "Description" : "Transform composed of Scale, Rotation (as a rotator), and Translation." }
USTRUCT(BlueprintType)
struct PAKEXPORTRUNTIME_API FTransformJson : public FJson
{
	GENERATED_BODY()
public:
	FTransformJson();
	FTransformJson(const FTransform& Transform);
	FTransformJson(FTransform&& Transform);

	FTransform TransformJsonToTransform();
	
	/// { "Description" : "Rotation of this transformation, as a rotator." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json") FRotatorJson rotation;
	/// { "Description" : "Translation of this transformation, as a vector." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json") FVectorJson translation;
	/// { "Description" : "3D scale (always applied in local space) as a vector." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json") FVectorJson Scale3D = FVectorJson(1.0f, 1.0f, 1.0f);
};

/// {
/// "Description" : "Scalar material parameter."
/// }
USTRUCT(BlueprintType)
struct PAKEXPORTRUNTIME_API FScalarJson : public FJson
{
	GENERATED_BODY()
public:
	/// { "Description" : "Scalar parameter name in material." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json") FString parameterName = "";
	/// {
	/// "Description" : "Value of the scalar material parameter."
	/// }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json") float value = 1.0f;
};

/// {
/// "Description" : "LinearColor RGBA(1.1.1.1)."
/// }
USTRUCT(BlueprintType)
struct PAKEXPORTRUNTIME_API FLinearColorJson : public FJson
{
	GENERATED_BODY()
public:
	FLinearColorJson();
	FLinearColorJson(const FLinearColor& Color);
	FLinearColorJson(float r, float g, float b, float a);

	FLinearColor LinearColorJsonToLinearColor();

	/// { "Description" : "Color's Red component." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json") float r = 1.0f;
	/// { "Description" : "Color's Green component." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json") float g = 1.0f;
	/// { "Description" : "Color's Blue component." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json") float b = 1.0f;
	/// {
	/// "Description" : "Color's Alpha component.",
	/// "Optional" : true
	/// }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "1.0"), Category = "Math|Color" ) float a = 1.0f;
};

/// { "Description" : "Material texture." }
USTRUCT(BlueprintType)
struct PAKEXPORTRUNTIME_API FTextureJson : public FJson
{
	GENERATED_BODY()
public:
	/// { "Description" : "Parameter for texture in material." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json") FString parameterName = "";
	/// { "Description" : "URL to download texture from." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json") FString url = "";
	/// {
	/// "Description" : "Enable canvas for texture or not.",
	///	"Optional" : true
	/// }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json") bool canvas = true;
	/// {
	/// "Description" : "Size of the texture.",
	///	"Optional" : true
	/// }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json") FVector2DJson size = {1024, 1024};
	/// {
	/// "Description" : "Position of the texture in relative space.",
	///	"Optional" : true
	/// }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json") FVector2DJson position = {0.5, 0.5};
	/// {
	/// "Description" : "Rotate angle in degrees.",
	///	"Optional" : true
	/// }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json") float Rotate = 0;
	/// {
	/// "Description" : "Pivot of the texture in relative space.",
	///	"Optional" : true
	/// }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json") FVector2DJson pivot = {0.5, 0.5};
	/// {
	/// "Description" : "Grid position.",
	///	"Optional" : true
	/// }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json") FVector2DJson coordinatePosition = {0, 0};
	/// {
	/// "Description" : "Grid size.",
	///	"Optional" : true
	/// }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json") FVector2DJson coordinateSize = {1, 1};
};

/// { "Description" : "Material." }
USTRUCT(BlueprintType)
struct PAKEXPORTRUNTIME_API FMaterialJson : public FJson
{
	GENERATED_BODY()
public:
	/// { "Description" : "Name of the asset relative to Content folder in project." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json") FString name = "";
	/// { "Description" : "Material name applying material to." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json") FString slot_name = "";
	/// {
	/// "Description" : "List of textures in the material.",
	///	"Optional" : true
	/// }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json") TArray<FTextureJson> textures;
	/// {
	/// "Description" : "List of scalars in the material.",
	///	"Optional" : true
	/// }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json") TArray<FScalarJson> scalars;
	/// { "Description" : "Relative path to material pak file." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json") FString pakFilePath = "";
	/// {
	/// "Description" : "Color",
	/// "Optional" : true
	///}
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json") FLinearColorJson color;
	/// {
	/// "Description" : "renderTargetAspectRatio.",
	///	"Optional" : true
	/// }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json") FVector2DJson renderTargetAspectRatio;
};

/// { "Description" : "Material initial state." }
USTRUCT(BlueprintType)
struct PAKEXPORTRUNTIME_API FAnimationInitialStateJson : public FJson
{
	GENERATED_BODY()
public:
	/// { "Description" : "Product animations." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json") TArray<FString> animations;
};

/// { "Description" : "Material initial state." }
USTRUCT(BlueprintType)
struct PAKEXPORTRUNTIME_API FMaterialInitialStateJson : public FJson
{
	GENERATED_BODY()
public:
	/// { "Description" : "List of materials applying to product." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json") TArray<FMaterialJson> materials;
};

/// { "Description" : "Mesh initial state." }
USTRUCT(BlueprintType)
struct PAKEXPORTRUNTIME_API FMeshInitialStateJson : public FMaterialInitialStateJson
{
	GENERATED_BODY()
public:
	/// { "Description" : "Name of the asset relative to Content folder in project." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json") FString meshName = "";
	/// {
	/// "Description" : "Product spawn transform.",
	///	"Optional" : true
	/// }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json") FTransformJson transform;
	/// {
	/// "Description" : "Product animations.",
	///	"Optional" : true
	/// }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json") TArray<FString> animations;
	/// {
	/// "Description" : "Materials slots.",
	///	"Optional" : true
	/// }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json") TArray<FString> slots;
	/// {
	/// "Description" : "Founded in product morphs.",
	///	"Optional" : true
	/// }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json") TArray<FString> morphs;
};

/// { "Description" : "Object describes path to pak/use file." }
USTRUCT(BlueprintType)
struct PAKEXPORTRUNTIME_API FPakJson : public FJson
{
	GENERATED_BODY()
public:
	/// { "Description" : "Relative path to pak file." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json|PAK") FString pakFilePath = "";
	/// {
	/// "Description" : "Path to usd/usdz file overrides pak file path.",
	///	"Optional" : true
	/// }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json|PAK") FString usd_path = "";
};

/// { "Description" : "Asset pak file." }
USTRUCT(BlueprintType)
struct PAKEXPORTRUNTIME_API FAssetPakJson : public FPakJson
{
	GENERATED_BODY()
public:
	/// { "Description" : "Asset name." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json|PAK") FString assetName;
};

/// { "Description" : "Animation pak file." }
USTRUCT(BlueprintType)
struct PAKEXPORTRUNTIME_API FAnimationPakJson : public FPakJson
{
	GENERATED_BODY()
public:
	/// { "Description" : "Animation initial state." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json|PAK") FAnimationInitialStateJson initialState;
};

/// { "Description" : "Material pak file." }
USTRUCT(BlueprintType)
struct PAKEXPORTRUNTIME_API FMaterialPakJson : public FJson
{
	GENERATED_BODY()
public:
	/// { "Description" : "Material initial state." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json|PAK") FMaterialInitialStateJson initialState;
};

/// { "Description" : "Mesh pak file." }
USTRUCT(BlueprintType)
struct PAKEXPORTRUNTIME_API FMeshPakJson : public FPakJson
{
	GENERATED_BODY()
public:
	/// { "Description" : "Mesh initial state." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json|PAK") FMeshInitialStateJson initialState;
};

/// { "Description" : "Attachment settings." }
USTRUCT(BlueprintType)
struct PAKEXPORTRUNTIME_API FAttachment : public FJson
{
	GENERATED_BODY()
public:
	/// { "Description" : "Find object in scene to what attach." }
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Payload|Json") FString attachToObject;
	/// { "Description" : "Find socket in object in scene to what attach." }
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Payload|Json") FString attachToSocket;
	/// {
	/// "Description" : "Use socket as a pivot of attached object.",
	///	"Optional" : true
	/// }
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Payload|Json") FString pivotSocket;
	/// {
	/// "Description" : "Local distance offset. Orientation of attachToSocket is main for this local offset. Socket's forward vector is multiplied on this.",
	///	"Optional" : true
	/// }
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Payload|Json") FVectorJson offset = { 0 , 0 , 0};
};

/// { "Description" : "Sequence section." }
USTRUCT(BlueprintType)
struct PAKEXPORTRUNTIME_API FSequenceSection : public FJson
{
	GENERATED_BODY()
public:
	/// { "Description" : "Id of section." }
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Payload|Json") int32 id = -1;
	/// { "Description" : "Start time of section." }
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Payload|Json") float startTime = 0;
	/// { "Description" : "End time of section." }
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Payload|Json") float endTime = 0;
};

#pragma endregion

// ----------------------- BASE PAYLOADS --------------------------------
#pragma region BasePayloadsRegion

/// { "Description" : "Type of Asset for all PAKs." }
UENUM(BlueprintType)
enum class EAssetType : uint8
{
	none,
	product,
	material,
	environment,
	level, //Deprecated, need for json strict
	camera,
	level_sequence,
	screenshot,
	light
};

/// { "Description" : "NULL payload." }
USTRUCT(BlueprintType)
struct PAKEXPORTRUNTIME_API FNullJson : public FJson
{
	GENERATED_BODY()
};

/// { "Description" : "Base payload structure." }
USTRUCT(BlueprintType)
struct PAKEXPORTRUNTIME_API FPayloadBase : public FJson
{
	GENERATED_BODY()
public:
	FPayloadBase();

protected:
	/// {
	/// "Description" : "Asset id.",
	///	"Optional" : true
	/// }
	UPROPERTY(EditAnywhere, Category = "Payload") FString assetId;
	/// {
	/// "DEPRECATED" : "Use correlationId.",
	/// "Description" : "Command id.",
	/// "Optional" : true
	/// }
	UPROPERTY(EditAnywhere, Category = "Payload") FString commandId;
	/// {
	/// "Description" : "Correlation id.",
	///	"Optional" : true
	/// }
	UPROPERTY(EditAnywhere, Category = "Payload") FString correlationId;
	UPROPERTY(EditAnywhere, Category = "Payload") FString unrealVersion;
public:
	UPROPERTY(EditAnywhere, Category = "Payload") EAssetType type = EAssetType::none;
};

/// { "Description" : "Base payload with bool to enable, click, drag and etc." }
USTRUCT(BlueprintType)
struct PAKEXPORTRUNTIME_API FBoolPayload : public FPayloadBase
{
	GENERATED_BODY()
public:
	FBoolPayload();
	/// { "Description" : "This value means enable, click, drag, etc." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload") bool value = false;
};

/// { "Description" : "Base payload for float." }
USTRUCT(BlueprintType)
struct PAKEXPORTRUNTIME_API FFloatPayload : public FPayloadBase
{
	GENERATED_BODY()
public:
	FFloatPayload();
	/// { "Description" : "This value is for changing float values." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload") float value = 0;
};

/// { "Description" : "Base payload for vector." }
USTRUCT(BlueprintType)
struct PAKEXPORTRUNTIME_API FFVectorPayload : public FPayloadBase
{
	GENERATED_BODY()
public:
	FFVectorPayload();
	/// { "Description" : "This value is for changing vector values." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload") FVectorJson value;
};

/// { "Description" : "Base payload for rotator." }
USTRUCT(BlueprintType)
struct PAKEXPORTRUNTIME_API FFRotatorPayload : public FPayloadBase
{
	GENERATED_BODY()
public:
	/// { "Description" : "This value is for changing rotator values." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload") FRotatorJson value;
};

/// { "Description" : "Base payload for string." }
USTRUCT(BlueprintType)
struct PAKEXPORTRUNTIME_API FFStringPayload : public FPayloadBase
{
	GENERATED_BODY()
public:
	FFStringPayload();
	/// { "Description" : "This value could be used for ObjectName, URL and other strings." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload") FString value;
};

/// { "Description" : "Base payload for strings." }
USTRUCT(BlueprintType)
struct PAKEXPORTRUNTIME_API FFStringsPayload : public FPayloadBase
{
	GENERATED_BODY()
public:
	/// { "Description" : "This value could be used for ObjectName, URL and other strings." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload") TArray<FString> values;
};

#pragma endregion

// ----------------------- ASSETS --------------------------------
#pragma region AssetsRegion

/// { "Description" : "Add mesh pak pak" }
USTRUCT(BlueprintType)
struct FAddMeshJson : public FPakJson
{
	GENERATED_BODY()
public:
	/// { "Description" : "Mesh name in pak." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Product") FString meshName;
};

/// { "Description" : "Unwrap/Wrap mesh pairs params" }
USTRUCT(BlueprintType)
struct FUWMeshPairParamsJson : public FJson
{
	GENERATED_BODY()
public:
	/// { "Description" : "Size." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Product") FVectorJson size;
	/// {
	/// "Description" : "Tile.",
	///	"Optional" : true
	/// }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Product") FVector2DJson UVTile = {1.f, 1.f};
	/// { "Description" : "position." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Product") FVectorJson position;
	/// {
	/// "Description" : "scale.",
	///	"Optional" : true
	/// }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Product") FVectorJson scale = {1.f, 1.f, 1.f};
};

/// { "Description" : "Unwrap/Wrap mesh pairs pak" }
USTRUCT(BlueprintType)
struct FUWMeshPairPakJson : public FAddMeshJson
{
	GENERATED_BODY()
public:
	/// { "Description" : "UWMeshPair params." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Product") FUWMeshPairParamsJson params;
};

/// { "Description" : "Unwrap/Wrap mesh pairs" }
USTRUCT(BlueprintType)
struct FUWMeshPairJson : public FJson
{
	GENERATED_BODY()
public:
	/// { "Description" : "Unwrap pak info." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Product") FUWMeshPairPakJson unwrap;
	/// { "Description" : "Unwrap pak info." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Product") FUWMeshPairPakJson wrap;
};

/// { "Description" : "Material Base Payload for PakExport." }
USTRUCT(BlueprintType)
struct PAKEXPORTRUNTIME_API FMaterialBasePayload : public FPayloadBase
{
	GENERATED_BODY()
public:
	/// { "Description" : "Material PAK." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Matrial") FMaterialPakJson materialPak;
};

/// { "Description" : "Material Base Payload for PakExport." }
USTRUCT(BlueprintType)
struct PAKEXPORTRUNTIME_API FMaterialPayload : public FMaterialBasePayload
{
	GENERATED_BODY()
public:
	FMaterialPayload();
	/// {
	/// "Description" : "Material name.",
	///	"Optional" : true
	/// }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Matrial") FString materialName;
};

/// { "Description" : "This payload is used in command to load/spawn and unload/destroy products." }
USTRUCT(BlueprintType)
struct PAKEXPORTRUNTIME_API FProductPayload : public FMaterialBasePayload
{
	GENERATED_BODY()
public:
	FProductPayload();
	bool IsValid() const;
	bool operator==(const FProductPayload& Other) const;
	bool operator==(const FString& ObjectName) const;
	
	/// {
	/// "Description" : "Global(static) product ID in scene (not per instance).",
	///	"Optional" : true
	/// }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Product") FString productName;
	/// { "Description" : "Mesh pak file." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Product") FMeshPakJson meshPak;
	/// {
	/// "Description" : "Animations pak file.",
	///	"Optional" : true
	/// }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Product") FPakJson animationsPak;
	/// {
	/// "Description" : "Object name used by frontend.",
	///	"Optional" : true
	/// }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Product") FString objectName;
	/// {
	/// "Description" : "Material slots.",
	///	"Optional" : true
	/// }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Product") TArray<FString> material_slots;
	/// {
	/// "Description" : "Product transform in the world.",
	///	"Optional" : true
	/// }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Product") FTransformJson transform;
	/// {
	/// "Description" : "Vector parameters of product materials.",
	///	"Optional" : true
	/// }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Product") TArray<FString> material_vector_params;
	/// {
	/// "Description" : "Datasmith format file path.",
	///	"Optional" : true
	/// }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Product") FString datasmithFilePath;
	/// {
	/// "Description" : "Attachment settings.",
	///	"Optional" : true
	/// }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Product") FAttachment attachment;
	/// {
	/// "Description" : "Unwrap/Wrap mesh pairs",
	///	"Optional" : true
	/// }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Product") TArray<FUWMeshPairJson> UWMeshPairs;
	/// {
	/// "Description" : "Additional meshes for product",
	///	"Optional" : true
	/// }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Product") TArray<FAddMeshJson> addMeshes;
	/// {
	/// "Description" : "Unwrap mesh pak",
	///	"Optional" : true
	/// }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Product") FUWMeshPairPakJson unwrapPak;
	/// {
	/// "Description" : "Make product like static mesh actor(no select)",
	///	"Optional" : true
	/// }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Product") bool staticMeshActor = false;
	/// {
	/// "Description" : "Can product collision with levels.",
	///	"Optional" : true
	/// }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Product") bool collisionLevel = false;
	/// {
	/// "Description" : "Can product collision with other products.",
	///	"Optional" : true
	/// }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Product") bool collisionProduct = false;
	/// {
	/// "Description" : "Show only meshes.",
	///	"Optional" : true
	/// }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Product") TArray<FString> cutMeshes;
	/// {
	/// "Description" : "Meshes to hide from product.",
	///	"Optional" : true
	/// }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Product") TArray<FString> hideMeshes;
	/// {
	/// "Description" : "Make pivot in center of bounds.",
	///	"Optional" : true
	/// }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Product") bool centerPivot = false;
	/// {
	/// "Description" : "Show unwrap for debug.",
	///	"Optional" : true
	/// }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Product") bool showUnwrap = false;
	/// {
	/// "Description" : "Layers to add product to.",
	///	"Optional" : true
	/// }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Product") TArray<FString> layers;
	/// {
	/// "Description" : "Set product hidden in scene.",
	///	"Optional" : true
	/// }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Product") bool hide = false;
	/// {
	/// "Description" : "Is product selected.",
	///	"Optional" : true
	/// }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Product") bool selected = false;
};

/// {
/// "Description" : "Load sublevel."
/// }
USTRUCT(BlueprintType)
struct PAKEXPORTRUNTIME_API FLevelPayload : public FPayloadBase
{
	GENERATED_BODY()
public:
	FLevelPayload();

	bool operator==(const FLevelPayload& Other) const;
	bool operator==(const FString& Name) const;
	
	/// {
	/// "Description" : "Level name.",
	///	"Optional" : true
	/// }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Level") FString levelName;
	/// {
	/// "Description" : "Bool that hide all levels, after that it loads level with levelName.",
	///	"Optional" : true
	/// }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Level") bool hideAllLevels = false;
	/// {
	/// "Description" : "Hide last level.",
	///	"Optional" : true
	/// }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Level") bool hideLastLevel = false;
	/// {
	/// "Description" : "This means that parts of level with tag 'Selectable' can be selected.",
	///	"Optional" : true
	/// }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Level") bool selectable = false;
	/// {
	/// "Description" : "Location of this sublevel.",
	///	"Optional" : true
	/// }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Level") FVectorJson location;
	/// {
	/// "Description" : "Rotation of this sublevel.",
	///	"Optional" : true
	/// }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Level") FRotatorJson rotation;
	/// {
	/// "Description" : "Level type.",
	///	"Optional" : true
	/// }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Level") TArray<FString> levelType;
	/// { "Description" : "Level PAK." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Level") FAssetPakJson levelPak;
	/// {
	/// "Description" : "Material slots of all actors in the world.",
	///	"Optional" : true
	/// }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Level") TArray<FString> slots;
	/// {
	/// "Description" : "Clear products during level change.",
	///	"Optional" : true
	/// }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Level") bool clearProducts = false;
};

#pragma endregion

// ----------------------- CONTROLS CAMERA --------------------------------
#pragma region ControlsCameraRegion

/// { "Description" : "Camera settings." }
USTRUCT(BlueprintType)
struct PAKEXPORTRUNTIME_API FCameraSettingsPayload : public FPayloadBase
{
	GENERATED_BODY()
public:
	FCameraSettingsPayload();
	/// {
	/// "Description" : "Camera aperture.",
	///	"Optional" : true
	/// }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Camera|Settings") float aperture = 0;
	/// {
	/// "Description" : "Camera FocusOffset.",
	///	"Optional" : true
	/// }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Camera|Settings") float focusOffset = 0;
	/// {
	/// "Description" : "Camera FocalLength.",
	///	"Optional" : true
	/// }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Camera|Settings") float focalLength = 0;
	/// {
	/// "Description" : "Camera FOV.",
	///	"Optional" : true
	/// }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Camera|Settings") float fov = 0;
	/// {
	/// "Description" : "Turns off focusing completely.",
	///	"Optional" : true
	/// }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Camera|Settings") bool enableFocus = false;
};

/// { "Description" : "CameraPreset with Location and Rotation. It Used in PakExportRuntime." }
USTRUCT(BlueprintType)
struct PAKEXPORTRUNTIME_API FCameraPresetBasePayload : public FCameraSettingsPayload
{
	GENERATED_BODY()
public:
	FCameraPresetBasePayload();
	/// { "Description" : "Location." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|PAK") FVectorJson location;
	/// { "Description" : "Rotation." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|PAK") FRotatorJson rotation;
	/// {
	/// "Description" : "Camera object.",
	///	"Optional" : true
	/// }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|PAK") FString object;
};

#pragma endregion

// ----------------------- SEQUENCE --------------------------------
#pragma region SequenceRegion

/// { "Description" : "SequenceLoopBehavior." }
UENUM(BlueprintType)
enum class ESequenceLoopBehavior : uint8
{
	none,
	loop,
	pingPong,
	loopSection,
	pingPongSection
};

/// { "Description" : "Settings of sequence." }
USTRUCT(BlueprintType)
struct PAKEXPORTRUNTIME_API FSettingsSequencePayload : public FPayloadBase
{
	GENERATED_BODY()
public:
	/// { 
	/// "Optional" : true,
	/// "Description" : "Loop behavior of sequence playing." 
	/// }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Sequence") ESequenceLoopBehavior loopBehavior = ESequenceLoopBehavior::none;
	/// {
	/// "TODO" : "Make play rate. It could be negative, so remove bool reverse.",
	/// "Optional" : true,
	/// "Description" : "Rate-speed of sequence playing"
	/// }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Sequence") float playRate = 1.0f;
	/// { 
	/// "Optional" : true,
	/// "Description" : "Is reverse playing of sequence." 
	/// }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Sequence") bool reverse = false;
	/// { 
	/// "Optional" : true,
	/// "Description" : "Time period of responding changes for callbacks." 
	/// }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Sequence") float timeResponseChangeCallBack = 0.05f;
	/// { 
	/// "Optional" : true,
	/// "Description" : "Sections of sequence. It is used for callback." 
	/// }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Callback") TArray<FSequenceSection> sections;
};

/// {
/// "Description" : "Init sequence."
/// }
USTRUCT(BlueprintType)
struct PAKEXPORTRUNTIME_API FInitSequencePayload : public FSettingsSequencePayload
{
	GENERATED_BODY()
public:
	FInitSequencePayload();
	
	/// { 
	/// "Optional" : true,
	/// "Description" : "Sequence to init if app uses CameraCuts as sections, not LevelSequences as sections. If invalid, then 0 index from paks." 
	/// }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|PAK") FString sequenceName;
	/// { "Description" : "PAKs of sequences." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|PAK") TArray<FAssetPakJson> paks;
};

#pragma endregion

USTRUCT()
struct PAKEXPORTRUNTIME_API FTempCommandPayload
{
	GENERATED_BODY()
public:
	FTempCommandPayload();
};
