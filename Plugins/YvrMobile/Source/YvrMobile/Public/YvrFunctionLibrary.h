#pragma once
#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "HeadMountedDisplayBase.h"

#include "IYvrMobilePlugin.h"
#include "YvrFunctionLibrary.generated.h"

UENUM(BlueprintType)
enum class EYvrTrackedDeviceType : uint8
{
	None UMETA(DisplayName = "No Devices"),
	HMD	UMETA(DisplayName = "HMD"),
	LTouch	UMETA(DisplayName = "Left Hand"),
	RTouch	UMETA(DisplayName = "Right Hand"),
	Touch	UMETA(DisplayName = "All Hands"),
	All	UMETA(DisplayName = "All Devices")
};

UENUM(BlueprintType)
enum class EYvrFixedFoveatedRenderingLevel : uint8
{
	FFR_Off = 0,
	FFR_Low = 1,
	FFR_Medium = 2,
	FFR_High = 3,
	FFR_HighTop = 4
};

UENUM(BlueprintType)
enum class EYvrColorSpace : uint8
{
	ColorSpace_Linear = 0,
	ColorSpace_SRGB = 1,
};

USTRUCT(BlueprintType)
struct FYvrGuardianTestResult
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boundary Test Result")
	bool IsTriggering;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boundary Test Result")
	float ClosestDistance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boundary Test Result")
	FVector ClosestPoint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boundary Test Result")
	FVector ClosestPointNormal;
};

UCLASS()
class YVRMOBILE_API UYvrFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintPure, Category = "YvrLibrary")
	static void GetPose(FRotator& DeviceRotation, FVector& DevicePosition, FVector& NeckPosition, bool bUseOrienationForPlayerCamera = false, bool bUsePositionForPlayerCamera = false, const FVector PositionScale = FVector::ZeroVector);

	UFUNCTION(BlueprintPure, Category = "YvrLibrary")
	static void GetRawSensorData(FVector& AngularAcceleration, FVector& LinearAcceleration, FVector& AngularVelocity, FVector& LinearVelocity, float& TimeInSeconds, EYvrTrackedDeviceType DeviceType = EYvrTrackedDeviceType::HMD);

	UFUNCTION(BlueprintPure, Category = "YvrLibrary")
	static bool IsDeviceTracked(EYvrTrackedDeviceType DeviceType);

	UFUNCTION(BlueprintCallable, Category = "YvrLibrary")
	static void SetCPUAndGPULevels(int CPULevel, int GPULevel);

	//UFUNCTION(BlueprintPure, Category = "YvrLibrary")
	//static bool GetUserProfile(FHmdUserProfile& Profile);

	UFUNCTION(BlueprintCallable, Category = "YvrLibrary")
	static void SetBaseRotationAndBaseOffsetInMeters(FRotator Rotation, FVector BaseOffsetInMeters, EOrientPositionSelector::Type Options);

	UFUNCTION(BlueprintPure, Category = "YvrLibrary")
	static void GetBaseRotationAndBaseOffsetInMeters(FRotator& OutRotation, FVector& OutBaseOffsetInMeters);

	UFUNCTION(BlueprintPure, Category = "YvrLibrary")
	static bool HasInputFocus();

	UFUNCTION(BlueprintPure, Category = "YvrLibrary")
	static bool HasSystemOverlayPresent();

	UFUNCTION(BlueprintPure, Category = "YvrLibrary")
	static void GetGPUUtilization(bool& IsGPUAvailable, float& GPUUtilization);

	//UFUNCTION(BlueprintPure, Category = "YvrLibrary")
	//static float GetGPUFrameTime();

	UFUNCTION(BlueprintPure, Category = "YvrLibrary")
	static EYvrFixedFoveatedRenderingLevel GetFixedFoveatedRenderingLevel();

	UFUNCTION(BlueprintCallable, Category = "YvrLibrary")
	static void SetFixedFoveatedRenderingLevel(EYvrFixedFoveatedRenderingLevel level);

	//UFUNCTION(BlueprintPure, Category = "YvrLibrary")
	//static EYvrDeviceType GetDeviceType();

	UFUNCTION(BlueprintPure, Category = "YvrLibrary")
	static TArray<float> GetAvailableDisplayFrequencies();

	UFUNCTION(BlueprintPure, Category = "YvrLibrary")
	static float GetCurrentDisplayFrequency();

	UFUNCTION(BlueprintCallable, Category = "YvrLibrary")
	static void SetDisplayFrequency(float RequestedFrequency);

	UFUNCTION(BlueprintCallable, Category = "YvrLibrary")
	static void SetExtraLatencyMode(int Mode);

	UFUNCTION(BlueprintCallable, Category = "YvrLibrary")
	static void SetColorScaleAndOffset(FLinearColor ColorScale, FLinearColor ColorOffset, bool bApplyToAllLayers = false);

	//UFUNCTION(BlueprintPure, Category = "YvrLibrary")
	//static EYvrColorSpace GetHmdColorDesc();

	//UFUNCTION(BlueprintCallable, Category = "YvrLibrary")
	//static void SetClientColorDesc(EYvrColorSpace ColorSpace);

	/* GUARDIAN API */
	UFUNCTION(BlueprintPure, Category = "YvrLibrary|Guardian")
	static bool IsGuardianDisplayed();

	UFUNCTION(BlueprintPure, Category = "YvrLibrary|Guardian")
	static bool IsGuardianConfigured();

	UFUNCTION(BlueprintPure, Category = "YvrLibrary|Guardian")
	static TArray<FVector> GetGuardianPoints();

	UFUNCTION(BlueprintPure, Category = "YvrLibrary|Guardian")
	static FVector GetGuardianDimensions();

	UFUNCTION(BlueprintPure, Category = "YvrLibrary|Guardian")
	static FTransform GetPlayAreaTransform();

	UFUNCTION(BlueprintCallable, Category = "YvrLibrary|Guardian")
	static FYvrGuardianTestResult GetPointGuardianIntersection(const FVector Point);

	UFUNCTION(BlueprintCallable, Category = "YvrLibrary|Guardian")
	static FYvrGuardianTestResult GetNodeGuardianIntersection(EYvrTrackedDeviceType DeviceType);

	UFUNCTION(BlueprintCallable, Category = "YvrLibrary|Guardian")
	static void SetGuardianVisibility(bool GuardianVisible);

protected:
	static class FYvrMobileHMD* GetYvrHMD();
};