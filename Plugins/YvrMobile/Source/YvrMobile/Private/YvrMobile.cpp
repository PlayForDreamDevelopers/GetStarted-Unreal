// Copyright 2020-2021 Yvr Technology Co., Ltd. All Rights Reserved.

#include "YvrMobile.h"
#include "YvrCustomPresent.h"

#include "XRThreadUtils.h"
#include "PostProcess/PostProcessHMD.h"
#include "Engine/World.h"
#include "GameFramework/WorldSettings.h"
#include "Interfaces/IPluginManager.h"
#include "HardwareInfo.h"
#if WITH_EDITOR
#include "YvrSettingsCustomization.h"
#include "PropertyEditorModule.h"
#include "ISettingsModule.h"
#endif

#if PLATFORM_ANDROID
#include "Android/AndroidWindow.h"
#include "Android/AndroidJNI.h"
#include "Android/AndroidApplication.h"

#include <android_native_app_glue.h>

extern struct android_app* GNativeAndroidApp;
#endif

#define LOCTEXT_NAMESPACE "FYvrMobilePlugin"

IMPLEMENT_MODULE(FYvrMobilePlugin, YvrMobile)

YvrPluginWrapper FYvrMobilePlugin::PluginWrapper;

TSharedPtr< class IXRTrackingSystem, ESPMode::ThreadSafe > FYvrMobilePlugin::CreateTrackingSystem()
{
	TSharedPtr< FYvrMobileHMD, ESPMode::ThreadSafe > YvrMobileHMD = FSceneViewExtensions::NewExtension<FYvrMobileHMD>();
	if (YvrMobileHMD)
	{
		return YvrMobileHMD;
	}
	return nullptr;
}

void FYvrMobilePlugin::StartupModule()
{
	IHeadMountedDisplayModule::StartupModule();
#if WITH_EDITOR
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.RegisterCustomClassLayout(
		UYvrHMDSettings::StaticClass()->GetFName(),
		FOnGetDetailCustomizationInstance::CreateStatic(&FYvrSettingsCustomization::MakeInstance)
	);
	PropertyModule.NotifyCustomizationModuleChanged();

	ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");
	if (SettingsModule != nullptr)
	{
		SettingsModule->RegisterSettings("Project", "Plugins", "YvrMobile",
			LOCTEXT("YvrMobileSettingsName", "YVR Mobile"),
			LOCTEXT("YvrMobileSettingsDescription", "Configure the YvrMobile plug-in."),
			GetMutableDefault<UYvrHMDSettings>()
		);
	}
#endif
	InitializeYvrPluginWrapper(&PluginWrapper);
}

void FYvrMobilePlugin::ShutdownModule()
{
	IHeadMountedDisplayModule::ShutdownModule();
#if WITH_EDITOR
	// unregister settings
	ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");

	if (SettingsModule != nullptr)
	{
		SettingsModule->UnregisterSettings("Project", "Plugins", "YvrMobile");
	}
#endif
	DestroyYvrPluginWrapper(&PluginWrapper);
}


////////////////////
// FYvrMobileHMD //
////////////////////

FYvrMobileHMD::FYvrMobileHMD(const FAutoRegister& AutoRegister) :
	FHeadMountedDisplayBase(nullptr),
	FSceneViewExtensionBase(AutoRegister),
	bIsStereoEnabled(true),
	bIsRenderThreadInit(false),
	bIsForeground(false),
	bUsesArrayTexture(false),
	NextLayerId(0),
	CustomPresent(nullptr)
{
	Settings = GetMutableDefault<UYvrHMDSettings>();
#if PLATFORM_ANDROID
	memset(&YvrLayerSubmit, 0, sizeof(YvrLayerSubmit));
#endif
	Startup();

	const TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin(TEXT("YvrMobile"));
	if (Plugin.IsValid())
	{
		UE_LOG(LogHMD, Log, TEXT("YvrMobile version '%s'"), *(Plugin->GetDescriptor().VersionName));
	}
}

FYvrMobileHMD::~FYvrMobileHMD()
{
	Shutdown();
}

// IXRSystemIdentifier

FName FYvrMobileHMD::GetSystemName() const
{
	static FName DefaultName(TEXT("YvrHMD"));
	return DefaultName;
}

// IXRTrackingSystem

bool FYvrMobileHMD::DoesSupportPositionalTracking() const
{
	return true;
}

bool FYvrMobileHMD::HasValidTrackingPosition()
{
	return true;
}

bool FYvrMobileHMD::EnumerateTrackedDevices(TArray<int32>& OutDevices, EXRTrackedDeviceType Type)
{
	if (Type == EXRTrackedDeviceType::Any || Type == EXRTrackedDeviceType::HeadMountedDisplay)
	{
		OutDevices.Add(IXRTrackingSystem::HMDDeviceId);
		return true;
	}
	return false;
}

bool FYvrMobileHMD::GetCurrentPose(int32 DeviceId, FQuat& OutOrientation, FVector& OutPosition)
{
	if (!IsInitialized())
		return false;

#if PLATFORM_ANDROID
	yvrPosef CurrrentPose;
	if (IsInRenderingThread())
	{
		UpdateCachedHeadPose();
		CurrrentPose = CurrentPoseState_RenderThread.Value.pose;
	}
	else if (IsInGameThread())
	{
		CurrrentPose = CurrentPoseState_GameThread.Value.pose;
	}
	PoseToOrientationAndPosition(CurrrentPose, OutOrientation, OutPosition, GetWorldToMetersScale());
	return true;
#endif

	return false;
}

void FYvrMobileHMD::SetTrackingOrigin(EHMDTrackingOrigin::Type NewOrigin)
{
#if PLATFORM_ANDROID
	CurTrackingOrigin = NewOrigin;
	yvrTrackingSpace yvrOrigin = YVRLIB_TRACKING_SPACE_LOCAL;
	if (NewOrigin == EHMDTrackingOrigin::Floor)
		yvrOrigin = YVRLIB_TRACKING_SPACE_LOCAL_FLOOR;

	if (NewOrigin == EHMDTrackingOrigin::Stage)
		yvrOrigin = YVRLIB_TRACKING_SPACE_STAGE;

	if (IsInitialized())
	{
		FYvrMobilePlugin::GetPluginWrapper().SetTrackingSpace(yvrOrigin);
	}

	OnTrackingOriginChanged();
#endif
}

EHMDTrackingOrigin::Type FYvrMobileHMD::GetTrackingOrigin() const
{
	EHMDTrackingOrigin::Type TrackingOrigin = EHMDTrackingOrigin::Eye;
#if PLATFORM_ANDROID
	yvrTrackingSpace yvrOrigin = YVRLIB_TRACKING_SPACE_LOCAL;

	if (IsInitialized())
	{
		FYvrMobilePlugin::GetPluginWrapper().GetTrackingSpace(yvrOrigin);
		switch (yvrOrigin)
		{
		case YVRLIB_TRACKING_SPACE_LOCAL:
			TrackingOrigin = EHMDTrackingOrigin::Eye;
			break;
		case YVRLIB_TRACKING_SPACE_LOCAL_FLOOR:
			TrackingOrigin = EHMDTrackingOrigin::Floor;
			break;
		case YVRLIB_TRACKING_SPACE_STAGE:
			TrackingOrigin = EHMDTrackingOrigin::Stage;
			break;
		default:
			UE_LOG(LogHMD, Error, TEXT("Unsupported yvr tracking origin type %d"), int(yvrOrigin));
			break;
		}
	}
#endif
	return TrackingOrigin;
}

bool FYvrMobileHMD::GetRelativeEyePose(int32 DeviceId, EStereoscopicPass Eye, FQuat& OutOrientation, FVector& OutPosition)
{
	OutOrientation = FQuat::Identity;
	OutPosition = FVector::ZeroVector;
	if (DeviceId == IXRTrackingSystem::HMDDeviceId && (Eye == eSSP_LEFT_EYE || Eye == eSSP_RIGHT_EYE))
	{
		OutPosition = FVector(0, (Eye == EStereoscopicPass::eSSP_LEFT_EYE ? -.5 : .5) * GetInterpupillaryDistance() * GetWorldToMetersScale(), 0);
		return true;
	}
	else
	{
		return false;
	}
}

void FYvrMobileHMD::ResetOrientationAndPosition(float Yaw)
{
	Recenter(RecenterOrientationAndPosition, Yaw);
}

void FYvrMobileHMD::ResetOrientation(float Yaw)
{
	Recenter(RecenterOrientation, Yaw);
}

void FYvrMobileHMD::ResetPosition()
{
	Recenter(RecenterPosition, 0);
}

void FYvrMobileHMD::SetBaseRotation(const FRotator& BaseRot)
{
	SetBaseOrientation(BaseRot.Quaternion());
}

FRotator FYvrMobileHMD::GetBaseRotation() const
{
	return GetBaseOrientation().Rotator();
}

void FYvrMobileHMD::SetBaseOrientation(const FQuat& BaseOrient)
{
	Settings->BaseOrientation = BaseOrient;
}

FQuat FYvrMobileHMD::GetBaseOrientation() const
{
	return Settings->BaseOrientation;
}

IHeadMountedDisplay* FYvrMobileHMD::GetHMDDevice()
{
	return this;
}

TSharedPtr<class IStereoRendering, ESPMode::ThreadSafe> FYvrMobileHMD::GetStereoRenderingDevice()
{
	return SharedThis(this);
}

bool FYvrMobileHMD::OnStartGameFrame(FWorldContext& WorldContext)
{
	PollSystemEvent();
	RefreshTrackingToWorldTransform(WorldContext);
#if PLATFORM_ANDROID
	CurrentPoseState_GameThread = LastPoseState_RenderThread;
#endif
	return true;
}

bool FYvrMobileHMD::OnEndGameFrame(FWorldContext& WorldContext)
{
	TrackingToWorld = ComputeTrackingToWorldTransform(WorldContext);
	return true;
}

void FYvrMobileHMD::OnBeginRendering_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneViewFamily& ViewFamily)
{
	if (!IsInitialized() && bIsForeground)
	{
		ExecuteOnRHIThread([this]()
		{
			BeginVRMode();
		});

		bIsRenderThreadInit = true;
		SetTrackingOrigin(CurTrackingOrigin);
	}
}

// IHeadMountedDisplay
EHMDWornState::Type FYvrMobileHMD::GetHMDWornState()
{
	return FYvrMobilePlugin::GetPluginWrapper().GetHMDWornState() ? EHMDWornState::Worn : EHMDWornState::NotWorn;
}

void FYvrMobileHMD::EnableHMD(bool enable)
{
}

bool FYvrMobileHMD::GetHMDMonitorInfo(MonitorInfo& MonitorDesc)
{
#if PLATFORM_ANDROID
	MonitorDesc.MonitorName = FString("YVR");
	MonitorDesc.MonitorId = 0;
	MonitorDesc.DesktopX = MonitorDesc.DesktopY = MonitorDesc.ResolutionX = MonitorDesc.ResolutionY = 0;

	MonitorDesc.ResolutionX = RenderTargetSize.X;
	MonitorDesc.ResolutionY = RenderTargetSize.Y;

	return true;
#endif
	return false;
}

void FYvrMobileHMD::GetFieldOfView(float& OutHFOVInDegrees, float& OutVFOVInDegrees) const
{
	FVector4 LeftEyeFov;
	FVector4 RightEyeFov;
#if PLATFORM_ANDROID
	LeftEyeFov = FYvrMobilePlugin::GetPluginWrapper().GetEyeFov(0);
	RightEyeFov = FYvrMobilePlugin::GetPluginWrapper().GetEyeFov(1);
#endif
	OutVFOVInDegrees = FMath::RadiansToDegrees(LeftEyeFov.Z + LeftEyeFov.W + RightEyeFov.Z + RightEyeFov.W) / 2;
	OutHFOVInDegrees = FMath::RadiansToDegrees(LeftEyeFov.X + LeftEyeFov.Y + RightEyeFov.X + RightEyeFov.Y) / 2;
}

void FYvrMobileHMD::SetInterpupillaryDistance(float NewInterpupillaryDistance)
{

}

float FYvrMobileHMD::GetInterpupillaryDistance() const
{
	float iPDValue = FYvrMobilePlugin::GetPluginWrapper().GetIPD();
	return iPDValue;
}


bool FYvrMobileHMD::GetHMDDistortionEnabled(EShadingPath ShadingPath) const
{
	return false;
}


bool FYvrMobileHMD::IsChromaAbCorrectionEnabled() const
{
	return false;
}

float FYvrMobileHMD::GetPixelDenity() const
{
	return Settings->PixelDensity;
}

void FYvrMobileHMD::SetPixelDensity(const float NewPixelDensity)
{
	Settings->PixelDensity = FMath::Clamp(NewPixelDensity, 0.5f, 2.0f);
}

FIntPoint FYvrMobileHMD::GetIdealRenderTargetSize() const
{
	return RenderTargetSize;
}

// IStereoRendering

bool FYvrMobileHMD::IsStereoEnabled() const
{
	return bIsStereoEnabled;
}

bool FYvrMobileHMD::EnableStereo(bool stereo)
{
	check(IsInGameThread());

	bIsStereoEnabled = true;

	return bIsStereoEnabled;
}

void FYvrMobileHMD::AdjustViewRect(EStereoscopicPass StereoPass, int32& X, int32& Y, uint32& SizeX, uint32& SizeY) const
{
#if PLATFORM_ANDROID
	if (CustomPresent->IsArrayTexture())
	{
		SizeX = RenderTargetSize.X;
	}
	else
	{
		SizeX = RenderTargetSize.X / 2;
	}
	SizeY = RenderTargetSize.Y;
	if (StereoPass == eSSP_RIGHT_EYE)
	{
		X += SizeX;
	}
#endif
}

FMatrix FYvrMobileHMD::GetStereoProjectionMatrix(const EStereoscopicPass StereoPassType) const
{
#if PLATFORM_ANDROID
	FVector4 EyeFov;
	switch (StereoPassType)
	{
	case eSSP_LEFT_EYE:
		EyeFov = FYvrMobilePlugin::GetPluginWrapper().GetEyeFov(0);
		break;
	case eSSP_RIGHT_EYE:
		EyeFov = FYvrMobilePlugin::GetPluginWrapper().GetEyeFov(1);
		break;
	default:
		break;
	}

	float Left = -FPlatformMath::Tan(FMath::DegreesToRadians(EyeFov.X));
	float Right = FPlatformMath::Tan(FMath::DegreesToRadians(EyeFov.Y));
	float Top = FPlatformMath::Tan(FMath::DegreesToRadians(EyeFov.Z));
	float Bottom = -FPlatformMath::Tan(FMath::DegreesToRadians(EyeFov.W));

	float ZNear = GNearClippingPlane;

	float SumRL = (Right + Left);
	float SumTB = (Top + Bottom);
	float InvRL = (1.0f / (Right - Left));
	float InvTB = (1.0f / (Top - Bottom));

	return FMatrix(
		FPlane((2.0f * InvRL), 0.0f, 0.0f, 0.0f),
		FPlane(0.0f, (2.0f * InvTB), 0.0f, 0.0f),
		FPlane((SumRL * InvRL), (SumTB * InvTB), 0.0f, 1.0f),
		FPlane(0.0f, 0.0f, ZNear, 0.0f)
	);

#endif
	return FMatrix();
}

// ISceneViewExtension
void FYvrMobileHMD::SetupViewFamily(FSceneViewFamily& InViewFamily)
{
	InViewFamily.EngineShowFlags.ScreenPercentage = true;
}

void FYvrMobileHMD::SetupView(FSceneViewFamily& InViewFamily, FSceneView& InView) {}

void FYvrMobileHMD::BeginRenderViewFamily(FSceneViewFamily& InViewFamily)
{
	int LayerNum = LayerMap.Num();

	if (LayerNum != 0)
	{
		TArray<FYvrLayerPtr> XLayers;
		XLayers.Empty(LayerNum);

		for (auto Pair : LayerMap)
		{
			if (!(Pair.Value->GetDesc().Flags & IStereoLayers::LAYER_FLAG_HIDDEN))
			{
				XLayers.Emplace(Pair.Value->Clone());
			}
		}

		XLayers.Sort(FYvrLayerPtr_CompareId());

		ExecuteOnRenderThread_DoNotWait([this, XLayers]()
		{
#if PLATFORM_ANDROID
			LastPoseState_RenderThread = CurrentPoseState_RenderThread;
			CurrentPoseState_RenderThread = CurrentPoseState_GameThread;
#endif
			int32 LayerNum = XLayers.Num();
			for (int32 LayerIndex = 0; LayerIndex < LayerNum; LayerIndex++)
			{
				if (LayerIndex < Layers_RenderThread.Num())
				{
					XLayers[LayerIndex]->Initialize_RenderThread(CustomPresent, Layers_RenderThread[LayerIndex].Get());

				}
				else
				{
					XLayers[LayerIndex]->Initialize_RenderThread(CustomPresent);
				}
			}
			Layers_RenderThread = XLayers;
		});
	}
}

void FYvrMobileHMD::PreRenderViewFamily_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneViewFamily& ViewFamily)
{
	TArray<FYvrLayerPtr> XLayers;

	for (int32 XLayerIndex = 0; XLayerIndex < Layers_RenderThread.Num(); XLayerIndex++)
	{
		auto SwapChain = Layers_RenderThread[XLayerIndex]->GetSwapChain();
		if (SwapChain.IsValid())
		{
			XLayers.Emplace(Layers_RenderThread[XLayerIndex]->Clone());
		}
	}


	ExecuteOnRHIThread_DoNotWait([this, XLayers]()
	{
#if PLATFORM_ANDROID

		CurrentPoseState_RHIThread = CurrentPoseState_RenderThread;
#endif
		Layers_RHIThread = XLayers;
	});
}

void FYvrMobileHMD::PreRenderView_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneView& InView) {}

void FYvrMobileHMD::PostRenderViewFamily_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneViewFamily& InViewFamily)
{
	int32 LayerNum = Layers_RenderThread.Num();

	for (int32 LayerIndex = 0; LayerIndex < LayerNum; LayerIndex++)
	{
		Layers_RenderThread[LayerIndex]->UpdateTexture_RenderThread(CustomPresent, RHICmdList);
	}
}

FXRRenderBridge* FYvrMobileHMD::GetActiveRenderBridge_GameThread(bool bUseSeparateRenderTarget)
{
	check(IsInGameThread());
	if (bUseSeparateRenderTarget)
	{
		return CustomPresent;
	}
	else
	{
		return nullptr;
	}
}

uint32 FYvrMobileHMD::CreateLayer(const IStereoLayers::FLayerDesc& InLayerDesc)
{
	check(IsInGameThread());

	uint32 LayerId = NextLayerId++;
	LayerMap.Add(LayerId, MakeShareable(new FYvrLayer(LayerId, InLayerDesc)));
	return LayerId;
}

void FYvrMobileHMD::DestroyLayer(uint32 LayerId)
{
	check(IsInGameThread());
	LayerMap.Remove(LayerId);
}

void FYvrMobileHMD::SetLayerDesc(uint32 LayerId, const IStereoLayers::FLayerDesc& InLayerDesc)
{
	check(IsInGameThread());
	FYvrLayerPtr* LayerFound = LayerMap.Find(LayerId);

	if (LayerFound)
	{
		LayerFound->Get()->SetDesc(InLayerDesc);
	}
}

bool FYvrMobileHMD::GetLayerDesc(uint32 LayerId, IStereoLayers::FLayerDesc& OutLayerDesc)
{
	check(IsInGameThread());
	FYvrLayerPtr* LayerFound = LayerMap.Find(LayerId);
	if (LayerFound)
	{
		OutLayerDesc = (*LayerFound)->GetDesc();
		return true;
	}
	return false;
}

void FYvrMobileHMD::MarkTextureForUpdate(uint32 LayerId)
{
	check(IsInGameThread());
	FYvrLayerPtr* LayerFound = LayerMap.Find(LayerId);

	if (LayerFound)
	{
		(*LayerFound)->MarkTextureForUpdate();
	}
}

IStereoLayers::FLayerDesc FYvrMobileHMD::GetDebugCanvasLayerDesc(FTextureRHIRef Texture)
{
	IStereoLayers::FLayerDesc StereoLayerDesc;
	return StereoLayerDesc;
}

// IStereoRenderTargetManager
bool FYvrMobileHMD::ShouldUseSeparateRenderTarget() const
{
	return IsStereoEnabled();
}

void FYvrMobileHMD::CalculateRenderTargetSize(const class FViewport& Viewport, uint32& InOutSizeX, uint32& InOutSizeY)
{
	check(IsInGameThread());
#if PLATFORM_ANDROID
	InOutSizeX = RenderTargetSize.X;
	InOutSizeY = RenderTargetSize.Y;
#endif
}

bool FYvrMobileHMD::AllocateRenderTargetTexture(uint32 Index, uint32 SizeX, uint32 SizeY, uint8 Format, uint32 NumMips, ETextureCreateFlags InFlags, ETextureCreateFlags TargetableTextureFlags, FTexture2DRHIRef& OutTargetableTexture, FTexture2DRHIRef& OutShaderResourceTexture, uint32 NumSamples)
{
	check(IsInGameThread());
	check(IsInRenderingThread());

	check(Index == 0);

	if (LayerMap[0].IsValid())
	{
		const FXRSwapChainPtr& SwapChain = EyeLayer_RenderThread->GetSwapChain();
		if (SwapChain.IsValid())
		{
			OutTargetableTexture = OutShaderResourceTexture = SwapChain->GetTexture2DArray() ? SwapChain->GetTexture2DArray() : SwapChain->GetTexture2D();
			return true;
		}
	}

	return false;
}

bool FYvrMobileHMD::NeedReAllocateViewportRenderTarget(const FViewport& Viewport)
{
	check(IsInGameThread());

	const uint32 InSizeX = Viewport.GetSizeXY().X;
	const uint32 InSizeY = Viewport.GetSizeXY().Y;

	FIntPoint RTSize;
	RTSize.X = Viewport.GetRenderTargetTexture()->GetSizeX();
	RTSize.Y = Viewport.GetRenderTargetTexture()->GetSizeY();

	uint32 NewSizeX = InSizeX, NewSizeY = InSizeY;
	CalculateRenderTargetSize(Viewport, NewSizeX, NewSizeY);
	if (NewSizeX != RTSize.X || NewSizeY != RTSize.Y)
	{
		return true;
	}
	return false;
}

// FXRTrackingSystemBase
float FYvrMobileHMD::GetWorldToMetersScale() const
{
	if (IsInGameThread() && GWorld != nullptr)
	{
		return GWorld->GetWorldSettings()->WorldToMeters;
	}
	// Default value, assume Unreal units are in centimeters
	return 100.0f;
}

bool FYvrMobileHMD::IsInitialized() const
{
	return bIsRenderThreadInit;
}

void FYvrMobileHMD::PollSystemEvent()
{
#if PLATFORM_ANDROID
	yvrEventDataBody Data = {};

	while(true)
	{
		bool Res = FYvrMobilePlugin::GetPluginWrapper().PollEvent(Data);
		if (!Res)
		{
			break;
		}
		ProcessEvent(Data);
	}
#endif
}

#if PLATFORM_ANDROID
void FYvrMobileHMD::ProcessEvent(const yvrEventDataBody& EventData)
{
	switch (EventData.eventHeader.eventType)
	{
		case YVRLIB_EVENT_DATA_LOST:
		{
			UE_LOG(LogHMD, Log, TEXT("YVREvent YVRLIB_EVENT_DATA_LOST"));
			break;
		}
		case YVRLIB_EVENT_VISIBILITY_GAINED:
		{
			UE_LOG(LogHMD, Log, TEXT("YVREvent YVRLIB_EVENT_VISIBILITY_GAINED"));
			break;
		}
		case YVRLIB_EVENT_VISIBILITY_LOST:
		{
			UE_LOG(LogHMD, Log, TEXT("YVREvent YVRLIB_EVENT_VISIBILITY_LOST"));
			break;
		}
		case YVRLIB_EVENT_FOCUS_GAINED:
		{
			UE_LOG(LogHMD, Log, TEXT("YVREvent YVRLIB_EVENT_FOCUS_GAINED"));
			IsAppFocused = true;
			break;
		}
		case YVRLIB_EVENT_FOCUS_LOST:
		{
			UE_LOG(LogHMD, Log, TEXT("YVREvent YVRLIB_EVENT_FOCUS_LOST"));
			IsAppFocused = false;
			break;
		}
		default:
			break;
	}
}

const uint32& FYvrMobileHMD::GetRHIIndex()
{
	return CurrentPoseState_RHIThread.Key;
}

const yvrPoseState& FYvrMobileHMD::GetRHIHeadPoseState()
{
	return CurrentPoseState_RHIThread.Value;
}
#endif

void FYvrMobileHMD::SetBasePosition(const FVector& BaseOffsetInMeters)
{
	Settings->BasePosition = BaseOffsetInMeters;
}

FVector FYvrMobileHMD::GetBasePosition() const
{
	return Settings->BasePosition;
}

void FYvrMobileHMD::SetFFRLevel(const EYvrFixedFoveatedRenderingLevel Level)
{
	Settings->FFRLevel = Level;
}

EYvrFixedFoveatedRenderingLevel FYvrMobileHMD::GetFFRLevel() const
{
	return Settings->FFRLevel;
}

void FYvrMobileHMD::SubmitFrame()
{
#if PLATFORM_ANDROID
	FQuat OutOrientation;
	FVector OutPosition;

	//opengl coords
	auto HeadPoseState = GetRHIHeadPoseState();
	auto PosePosition = HeadPoseState.pose.position;
	auto PoseRotation = HeadPoseState.pose.rotation;

	OutPosition = FVector(PosePosition.x, PosePosition.y, PosePosition.z);
	OutOrientation = FQuat(PoseRotation.x, PoseRotation.y, PoseRotation.z, PoseRotation.w);

	auto OutMatrix = FRotationTranslationMatrix::Make(OutOrientation.Rotator(), OutPosition);

	TArray<FYvrLayerPtr> Layers = Layers_RHIThread;
	Layers.Sort(FYvrLayerPtr_CompareTotal());

	TArray<yvrLayerProjection2*> LayerSubmitPtr;
	int32 LayerNum = Layers.Num();
	LayerSubmitPtr.SetNum(LayerNum);

	EYvrFixedFoveatedRenderingLevel FFRLevel = GetFFRLevel();
	for (int32 LayerIndex = 0; LayerIndex < LayerNum; LayerIndex++)
	{
		LayerSubmitPtr[LayerIndex] = Layers[LayerIndex]->UpdateLayer_RHIThread(CustomPresent, OutMatrix, TrackingToWorld, Settings);
	}

	YvrLayerSubmit.minVsyncs = 1;
	YvrLayerSubmit.LayerCount = LayerNum;
	YvrLayerSubmit.FrameIndex = GetRHIIndex();
	YvrLayerSubmit.HeadPoseState = HeadPoseState;
	YvrLayerSubmit.Layers = LayerSubmitPtr.GetData();

	FYvrMobilePlugin::GetPluginWrapper().SubmitFrame(&YvrLayerSubmit);

	for (int32 LayerIndex = 0; LayerIndex < LayerNum; LayerIndex++)
	{
		Layers[LayerIndex]->IncrementSwapChainIndex_RHIThread(CustomPresent);
	}
#endif
}

bool FYvrMobileHMD::Startup()
{

#if PLATFORM_ANDROID

	yvrInitParams params;
	memset(&params, 0, sizeof(params));
	params.Java.ActivityObject = FAndroidApplication::GetGameActivityThis();
	params.Java.Vm = GJavaVM;
	params.Java.Env = FAndroidApplication::GetJavaEnv();

	if (FYvrMobilePlugin::GetPluginWrapper().Initialize(&params) != YVRLIB_INITIALIZE_SUCCESS)
	{
		return false;
	}
	else
	{
		FCoreDelegates::ApplicationWillEnterBackgroundDelegate.AddRaw(this, &FYvrMobileHMD::ApplicationWillEnterBackgroundDelegate);
		FCoreDelegates::ApplicationHasEnteredForegroundDelegate.AddRaw(this, &FYvrMobileHMD::ApplicationHasEnteredForegroundDelegate);

		FYvrMobilePlugin::GetPluginWrapper().SetPerfThread(YVRLIB_PERF_THREAD_TYPE_MAIN, FPlatformTLS::GetCurrentThreadId());

		FString RHIString;
		{
			FString HardwareDetails = FHardwareInfo::GetHardwareDetailsString();
			FString RHILookup = NAME_RHI.ToString() + TEXT("=");
			
			if (!FParse::Value(*HardwareDetails, *RHILookup, RHIString))
			{
				return false;
			}

			if (RHIString == TEXT("OpenGL"))
			{
				CustomPresent = CreateCustomPresent_OpenGL(this);
			}
			else
			{
				UE_LOG(LogHMD, Warning, TEXT("%s is not currently supported by YvrMobile plugin"), *RHIString);
				return false;
			}
		}

		static const auto PixelDensityCVar = IConsoleManager::Get().FindConsoleVariable(TEXT("vr.PixelDensity"));
		SetPixelDensity(PixelDensityCVar ? PixelDensityCVar->GetFloat() : 1.0f);

		RenderTargetSize = FYvrMobilePlugin::GetPluginWrapper().GetRenderTargetSize();
		RenderTargetSize.X = FMath::CeilToInt(RenderTargetSize.X * Settings->PixelDensity);
		RenderTargetSize.Y = FMath::CeilToInt(RenderTargetSize.Y * Settings->PixelDensity);

		static const auto MobileMultiViewCVar = IConsoleManager::Get().FindTConsoleVariableDataInt(TEXT("vr.MobileMultiView"));
		const bool bIsMobileMultiViewEnabled = (MobileMultiViewCVar && MobileMultiViewCVar->GetValueOnAnyThread() != 0);
		const bool bIsUsingMobileMultiView = GSupportsMobileMultiView && bIsMobileMultiViewEnabled;

		if (bIsUsingMobileMultiView)
		{
			CustomPresent->SetIsArrayTexture(true);
		}
		else
		{
			RenderTargetSize.X *= 2;
		}

		InitializeEyeLayer();

		return true;
	}
#endif

	return false;
}

void FYvrMobileHMD::Shutdown()
{
#if PLATFORM_ANDROID
	FYvrMobilePlugin::GetPluginWrapper().LeaveVrMode();
	FYvrMobilePlugin::GetPluginWrapper().Shutdown();
#endif
	ExecuteOnRenderThread([this]()
	{
		ExecuteOnRHIThread([this]()
		{
			for (int32 LayerIndex = 0; LayerIndex < Layers_RenderThread.Num(); LayerIndex++)
			{
				Layers_RenderThread[LayerIndex]->ReleaseResources_RHIThread();
			}

			for (int32 LayerIndex = 0; LayerIndex < Layers_RHIThread.Num(); LayerIndex++)
			{
				Layers_RHIThread[LayerIndex]->ReleaseResources_RHIThread();
			}
		});
	});
}

void FYvrMobileHMD::BeginVRMode()
{
#if PLATFORM_ANDROID
	yvrBeginParams beginParams;
	memset(&beginParams, 0, sizeof(yvrBeginParams));

	beginParams.colorSpace = (yvrColorSpace)Settings->ColorSpace;
	beginParams.optionFlags = yvrOptionFlags::kFoveationSubsampled;
	beginParams.nativeWindow = GNativeAndroidApp->window;

	FYvrMobilePlugin::GetPluginWrapper().EnterVrMode(&beginParams);
	FYvrMobilePlugin::GetPluginWrapper().SetPerfThread(YVRLIB_PERF_THREAD_TYPE_RENDERER, FPlatformTLS::GetCurrentThreadId());
	FYvrMobilePlugin::GetPluginWrapper().SetClockLevels(Settings->CPULevel, Settings->GPULevel);
	SetDisplayFrequency(Settings->DisplayRefreshRate);
#endif
}

void FYvrMobileHMD::UpdateCachedHeadPose()
{
#if PLATFORM_ANDROID
	CurrentPoseState_RenderThread.Key = GFrameNumberRenderThread;
	float PredictedTime = FYvrMobilePlugin::GetPluginWrapper().GetPredictedDisplayTime();
	FYvrMobilePlugin::GetPluginWrapper().GetNodePose(Hmd, PredictedTime, CurrentPoseState_RenderThread.Value);
#endif
}

void FYvrMobileHMD::Recenter(FRecenterTypes RecenterType, float Yaw)
{
#if PLATFORM_ANDROID
	const bool FloorLevel = GetTrackingOrigin() != EHMDTrackingOrigin::Eye;

	yvrPoseState PoseState;
	float PredictedTime = FYvrMobilePlugin::GetPluginWrapper().GetPredictedDisplayTime();
	FYvrMobilePlugin::GetPluginWrapper().GetNodePose(Hmd, PredictedTime, PoseState);

	if (RecenterType & RecenterPosition)
	{
		Settings->BasePosition = FVector(-PoseState.pose.position.z, PoseState.pose.position.x, PoseState.pose.position.y);
		if (FloorLevel)
			Settings->BasePosition.Z = 0;
	}

	if (RecenterType & RecenterOrientation)
	{
		Settings->BaseOrientation = FRotator(0, FRotator(FQuat(PoseState.pose.rotation.z, -PoseState.pose.rotation.x, -PoseState.pose.rotation.y, PoseState.pose.rotation.w)).Yaw - Yaw, 0).Quaternion();
	}
#endif
}

void FYvrMobileHMD::SetDisplayFrequency(float Frequency)
{
	Settings->DisplayRefreshRate = Frequency;
	FYvrMobilePlugin::GetPluginWrapper().SetDisplayRefreshRate(Settings->DisplayRefreshRate);
}

void FYvrMobileHMD::SetColorScaleAndOffset(FLinearColor ColorScale, FLinearColor ColorOffset, bool bApplyToAllLayers)
{
	Settings->bApplyColorScaleAndOffsetToAllLayers = bApplyToAllLayers;
	Settings->ColorScale = ColorScale;
	Settings->ColorOffset = ColorOffset;
}

#if PLATFORM_ANDROID
void FYvrMobileHMD::PoseToOrientationAndPosition(const yvrPosef& Pose, FQuat& CurrentOrientation, FVector& CurrentPosition, const float WorldToMetersScale)
{
	CurrentOrientation = FQuat(Pose.rotation.z, -Pose.rotation.x, -Pose.rotation.y, Pose.rotation.w);
	CurrentOrientation = Settings->BaseOrientation.Inverse() * CurrentOrientation;
	CurrentOrientation.Normalize();
	CurrentPosition = FVector(-Pose.position.z, Pose.position.x, Pose.position.y);
	CurrentPosition = (CurrentPosition - Settings->BasePosition) * WorldToMetersScale;
	CurrentPosition = Settings->BaseOrientation.Inverse().RotateVector(CurrentPosition);
}
#endif

void FYvrMobileHMD::ApplicationWillEnterBackgroundDelegate()
{
#if PLATFORM_ANDROID
	if (IsInitialized())
	{
		FYvrMobilePlugin::GetPluginWrapper().LeaveVrMode();
		bIsRenderThreadInit = false;
	}
	bIsForeground = false;
#endif
}

void FYvrMobileHMD::ApplicationHasEnteredForegroundDelegate()
{
	bIsForeground = true;
}

void FYvrMobileHMD::InitializeEyeLayer()
{
	IStereoLayers::FLayerDesc EyeLayerDesc;
	EyeLayerDesc.Priority = INT_MIN;
	EyeLayerDesc.Flags = LAYER_FLAG_TEX_CONTINUOUS_UPDATE;
	EyeLayerDesc.LayerSize.X = RenderTargetSize.X;
	EyeLayerDesc.LayerSize.Y = RenderTargetSize.Y;
	const uint32 EyeLayerId = CreateLayer(EyeLayerDesc);
	check(EyeLayerId == 0);

	ExecuteOnRenderThread([this](FRHICommandListImmediate& RHICmdList)
	{
		if (LayerMap[0].IsValid())
		{
			FYvrLayerPtr EyeLayer = LayerMap[0]->Clone();
			EyeLayer->Initialize_RenderThread(CustomPresent, EyeLayer_RenderThread.Get());

			if (Layers_RenderThread.Num() > 0)
			{
				Layers_RenderThread[0] = EyeLayer;
			}
			else
			{
				Layers_RenderThread.Add(EyeLayer);
			}

			EyeLayer_RenderThread = EyeLayer;
		}
	});
}

#undef LOCTEXT_NAMESPACE
