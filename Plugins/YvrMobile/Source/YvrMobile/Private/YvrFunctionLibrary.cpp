#include "YvrFunctionLibrary.h"
#include "YvrMobile.h"

UYvrFunctionLibrary::UYvrFunctionLibrary(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

FYvrMobileHMD* UYvrFunctionLibrary::GetYvrHMD()
{
	if (GEngine && GEngine->XRSystem.IsValid())
	{
		if (GEngine->XRSystem->GetSystemName() == FName("YvrHMD"))
		{
			return static_cast<FYvrMobileHMD*>(GEngine->XRSystem.Get());
		}
	}
	return nullptr;
}

void UYvrFunctionLibrary::GetPose(FRotator& DeviceRotation, FVector& DevicePosition, FVector& NeckPosition, bool bUseOrienationForPlayerCamera, bool bUsePositionForPlayerCamera, const FVector PositionScale)
{
	FYvrMobileHMD* YvrHMD = GetYvrHMD();
	if (YvrHMD != nullptr)
	{
		FQuat HeadOrientation = FQuat::Identity;
		FVector HeadPosition = FVector::ZeroVector;

		YvrHMD->GetCurrentPose(YvrHMD->HMDDeviceId, HeadOrientation, HeadPosition);

		DeviceRotation = HeadOrientation.Rotator();
		DevicePosition = HeadPosition;
		NeckPosition = FVector::ZeroVector;
	}
	else
	{
		DeviceRotation = FRotator::ZeroRotator;
		DevicePosition = FVector::ZeroVector;
		NeckPosition = FVector::ZeroVector;
	}
}

void UYvrFunctionLibrary::GetRawSensorData(FVector& AngularAcceleration, FVector& LinearAcceleration, FVector& AngularVelocity, FVector& LinearVelocity, float& TimeInSeconds, EYvrTrackedDeviceType DeviceType)
{
#if PLATFORM_ANDROID
	FYvrMobileHMD* YvrHMD = GetYvrHMD();
	if (YvrHMD != nullptr)
	{
		yvrPoseState State;
		FYvrMobilePlugin::GetPluginWrapper().GetNodePose(FYvrMobileHMD::ToYvrNode(DeviceType), 0.0f, State);

		AngularAcceleration = FYvrMobileHMD::ToFVector(State.angularAcceleration);
		LinearAcceleration = FYvrMobileHMD::ToFVector(State.linearAcceleration);
		AngularVelocity = FYvrMobileHMD::ToFVector(State.angularVelocity);
		LinearVelocity = FYvrMobileHMD::ToFVector(State.linearVelocity);
		TimeInSeconds = State.poseTimeStampNs;
	}
#endif
}

bool UYvrFunctionLibrary::IsDeviceTracked(EYvrTrackedDeviceType DeviceType)
{
#if PLATFORM_ANDROID
	FYvrMobileHMD* YvrHMD = GetYvrHMD();
	if (YvrHMD != nullptr)
	{
		yvrControllerConnectedInfo info;
		FYvrMobilePlugin::GetPluginWrapper().GetControllerConnectedInfo(&info);
		switch (DeviceType)
		{
		case EYvrTrackedDeviceType::None:
			return true;
		case EYvrTrackedDeviceType::HMD:
			return true;
		case EYvrTrackedDeviceType::LTouch:
			return info.left_connected;
		case EYvrTrackedDeviceType::RTouch:
			return info.right_connected;
		case EYvrTrackedDeviceType::Touch:
			return info.left_connected || info.right_connected;
		case EYvrTrackedDeviceType::All:
			return info.left_connected && info.right_connected;
		default:
			break;
		}
	}
#endif
	return false;
}

void UYvrFunctionLibrary::SetCPUAndGPULevels(int CPULevel, int GPULevel)
{
	FYvrMobileHMD* YvrHMD = GetYvrHMD();
	if (YvrHMD != nullptr)
	{
		FYvrMobilePlugin::GetPluginWrapper().SetCpuLevel(CPULevel);
		FYvrMobilePlugin::GetPluginWrapper().SetGpuLevel(GPULevel);
	}
}

void UYvrFunctionLibrary::SetBaseRotationAndBaseOffsetInMeters(FRotator Rotation, FVector BaseOffsetInMeters, EOrientPositionSelector::Type Options)
{
	FYvrMobileHMD* YvrHMD = GetYvrHMD();
	if (YvrHMD != nullptr)
	{
		if ((Options == EOrientPositionSelector::Orientation) || (Options == EOrientPositionSelector::OrientationAndPosition))
		{
			YvrHMD->SetBaseRotation(Rotation);
		}
		if ((Options == EOrientPositionSelector::Position) || (Options == EOrientPositionSelector::OrientationAndPosition))
		{
			YvrHMD->SetBasePosition(BaseOffsetInMeters);
		}
	}
}

void UYvrFunctionLibrary::GetBaseRotationAndBaseOffsetInMeters(FRotator& OutRotation, FVector& OutBaseOffsetInMeters)
{
	FYvrMobileHMD* YvrHMD = GetYvrHMD();
	if (YvrHMD != nullptr)
	{
		OutRotation = YvrHMD->GetBaseRotation();
		OutBaseOffsetInMeters = YvrHMD->GetBasePosition();
	}
	else
	{
		OutRotation = FRotator::ZeroRotator;
		OutBaseOffsetInMeters = FVector::ZeroVector;
	}
}

bool UYvrFunctionLibrary::HasInputFocus()
{
	FYvrMobileHMD* YvrHMD = GetYvrHMD();
	if (YvrHMD != nullptr)
	{
		return YvrHMD->IsAppFocused;
	}
	return false;
}

bool UYvrFunctionLibrary::HasSystemOverlayPresent()
{
	FYvrMobileHMD* YvrHMD = GetYvrHMD();
	if (YvrHMD != nullptr)
	{
		return !YvrHMD->IsAppFocused;
	}
	return false;
}

void UYvrFunctionLibrary::GetGPUUtilization(bool& IsGPUAvailable, float& GPUUtilization)
{
	FYvrMobileHMD* YvrHMD = GetYvrHMD();
	if (YvrHMD != nullptr)
	{
		IsGPUAvailable = true;
		GPUUtilization = FYvrMobilePlugin::GetPluginWrapper().GetGpuUtilization();
	}
}

EYvrFixedFoveatedRenderingLevel UYvrFunctionLibrary::GetFixedFoveatedRenderingLevel()
{
	FYvrMobileHMD* YvrHMD = GetYvrHMD();
	if (YvrHMD != nullptr)
	{
		return YvrHMD->GetFFRLevel();
	}
	return EYvrFixedFoveatedRenderingLevel::FFR_Off;
}

void UYvrFunctionLibrary::SetFixedFoveatedRenderingLevel(EYvrFixedFoveatedRenderingLevel level)
{
	FYvrMobileHMD* YvrHMD = GetYvrHMD();
	if (YvrHMD != nullptr)
	{
		YvrHMD->SetFFRLevel(level);
	}
}

TArray<float> UYvrFunctionLibrary::GetAvailableDisplayFrequencies()
{
	return TArray<float>({ 72.0f, 90.0f });
}

float UYvrFunctionLibrary::GetCurrentDisplayFrequency()
{
	return FYvrMobilePlugin::GetPluginWrapper().GetDisplayRefreshRate();
}

void UYvrFunctionLibrary::SetDisplayFrequency(float RequestedFrequency)
{
	FYvrMobileHMD* YvrHMD = GetYvrHMD();
	if (YvrHMD != nullptr)
	{
		YvrHMD->SetDisplayFrequency(RequestedFrequency);
	}
}

void UYvrFunctionLibrary::SetExtraLatencyMode(int Mode)
{
#if PLATFORM_ANDROID
	FYvrMobilePlugin::GetPluginWrapper().SetExtraLatencyMode((yvrExtraLatencyMode)Mode);
#endif
}

void UYvrFunctionLibrary::SetColorScaleAndOffset(FLinearColor ColorScale, FLinearColor ColorOffset, bool bApplyToAllLayers)
{
	FYvrMobileHMD* YvrHMD = GetYvrHMD();
	if (YvrHMD != nullptr)
	{
		YvrHMD->SetColorScaleAndOffset(ColorScale, ColorOffset, bApplyToAllLayers);
	}
}

bool UYvrFunctionLibrary::IsGuardianDisplayed()
{
#if PLATFORM_ANDROID
	FYvrMobileHMD* YvrHMD = GetYvrHMD();
	if (YvrHMD != nullptr)
	{
		return FYvrMobilePlugin::GetPluginWrapper().GetBoundaryVisible();
	}
#endif
	return false;
}

bool UYvrFunctionLibrary::IsGuardianConfigured()
{
#if PLATFORM_ANDROID
	FYvrMobileHMD* YvrHMD = GetYvrHMD();
	if (YvrHMD != nullptr)
	{
		return FYvrMobilePlugin::GetPluginWrapper().GetBoundaryConfigured();
	}
#endif
	return false;
}

TArray<FVector> UYvrFunctionLibrary::GetGuardianPoints()
{
	TArray<FVector> BoundaryPointList;
#if PLATFORM_ANDROID
	int NumPoints = FYvrMobilePlugin::GetPluginWrapper().GetBoundaryGeometryPointsCount();
	yvrVector3f* BoundaryPoints = new yvrVector3f[NumPoints];
	FYvrMobilePlugin::GetPluginWrapper().GetBoundaryGeometry(BoundaryPoints);

	for (int i = 0; i < NumPoints; ++i)
	{
		FVector Point;
		Point = FYvrMobileHMD::ToFVector(BoundaryPoints[i]);
		BoundaryPointList.Add(Point);
	}
#endif
	return BoundaryPointList;
}

FVector UYvrFunctionLibrary::GetGuardianDimensions()
{
#if PLATFORM_ANDROID
	FYvrMobileHMD* YvrHMD = GetYvrHMD();
	if (YvrHMD != nullptr)
	{
		BoundaryDimensions Dimensions;
		FYvrMobilePlugin::GetPluginWrapper().GetBoundaryDimensions(Dimensions);
		yvrVector3f Temp;
		Temp.x = Dimensions.dimensions.x;
		Temp.y = Dimensions.dimensions.y;
		Temp.z = Dimensions.dimensions.z;
		return FYvrMobileHMD::ToFVector(Temp);
	}
#endif
	return FVector::ZeroVector;
}

FTransform UYvrFunctionLibrary::GetPlayAreaTransform()
{
	return FTransform();
}

FYvrGuardianTestResult UYvrFunctionLibrary::GetPointGuardianIntersection(const FVector Point)
{
	FYvrGuardianTestResult InteractionInfo;
	memset(&InteractionInfo, 0, sizeof(FYvrGuardianTestResult));
#if PLATFORM_ANDROID
	FYvrMobileHMD* YvrHMD = GetYvrHMD();
	if (YvrHMD != nullptr)
	{
		yvrVector3f Temp = FYvrMobileHMD::ToYvrFVector(Point);
		BoundaryPoint YvrpPoint;
		YvrpPoint.point.x = Temp.x;
		YvrpPoint.point.y = Temp.y;
		YvrpPoint.point.z = Temp.z;
		BoundaryTestResult InteractionResult;

		FYvrMobilePlugin::GetPluginWrapper().TestBoundaryPoint(YvrpPoint, &InteractionResult);
		InteractionInfo.IsTriggering = InteractionResult.IsTriggering;
		InteractionInfo.ClosestDistance = InteractionResult.ClosestDistance;
		InteractionInfo.ClosestPoint = FYvrMobileHMD::ToFVector(InteractionResult.ClosestPoint);
		InteractionInfo.ClosestPointNormal = FYvrMobileHMD::ToFVector(InteractionResult.ClosestPointNormal);
		
	}
#endif
	return InteractionInfo;
}

FYvrGuardianTestResult UYvrFunctionLibrary::GetNodeGuardianIntersection(EYvrTrackedDeviceType DeviceType)
{
	FYvrGuardianTestResult InteractionInfo;
	memset(&InteractionInfo, 0, sizeof(FYvrGuardianTestResult));
#if PLATFORM_ANDROID
	FYvrMobileHMD* YvrHMD = GetYvrHMD();
	if (YvrHMD != nullptr)
	{
		yvrDeviceNode DeviceNode = FYvrMobileHMD::ToYvrNode(DeviceType);
		BoundaryTestResult InteractionResult;

		FYvrMobilePlugin::GetPluginWrapper().TestBoundaryNode(DeviceNode, &InteractionResult);
		InteractionInfo.IsTriggering = InteractionResult.IsTriggering;
		InteractionInfo.ClosestDistance = InteractionResult.ClosestDistance;
		InteractionInfo.ClosestPoint = FYvrMobileHMD::ToFVector(InteractionResult.ClosestPoint);
		InteractionInfo.ClosestPointNormal = FYvrMobileHMD::ToFVector(InteractionResult.ClosestPointNormal);

	}
#endif
	return InteractionInfo;
}

void UYvrFunctionLibrary::SetGuardianVisibility(bool GuardianVisible)
{
#if PLATFORM_ANDROID
	FYvrMobileHMD* YvrHMD = GetYvrHMD();
	if (YvrHMD != nullptr)
	{
		FYvrMobilePlugin::GetPluginWrapper().SetBoundaryVisible(GuardianVisible);
	}
#endif
}
