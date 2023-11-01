// Copyright 2020-2021 Yvr Technology Co., Ltd. All Rights Reserved.

#pragma once
#include "IYvrMobilePlugin.h"
#include "YvrPluginWrapper.h"
#include "YvrHMDSettings.h"
#include "YvrFunctionLibrary.h"
#include "YvrHMDLayer.h"

#include "HeadMountedDisplayBase.h"
#include "SceneViewExtension.h"
#include "XRRenderTargetManager.h"
#include "XRRenderBridge.h"
#include "IStereoLayers.h"

enum FRecenterTypes
{
	RecenterOrientation = 0x1,
	RecenterPosition = 0x2,
	RecenterOrientationAndPosition = 0x3
};

// FYvrMobilePlugin
class FYvrMobilePlugin : public IYvrMobilePlugin
{
	virtual TSharedPtr< class IXRTrackingSystem, ESPMode::ThreadSafe > CreateTrackingSystem() override;

	FString GetModuleKeyName() const override
	{
		return FString(TEXT("YvrMobile"));
	}

	virtual void StartupModule() override;

	virtual void ShutdownModule() override;

	virtual bool SupportsDynamicReloading() override
	{
		return false;
	}

public:

	static YvrPluginWrapper PluginWrapper;

	YVRMOBILE_API static inline YvrPluginWrapper& GetPluginWrapper() { return PluginWrapper; }

};

//class FYvrCustomPresent;
class FYvrMobileHMD : public FHeadMountedDisplayBase, public FSceneViewExtensionBase, public FXRRenderTargetManager, public IStereoLayers
{
	friend class FYvrCustomPresent;
public:
	FYvrMobileHMD(const FAutoRegister&);
	virtual ~FYvrMobileHMD();

	// IXRSystemIdentifier
	virtual FName GetSystemName() const override;
	virtual int32 GetXRSystemFlags() const override { return EXRSystemFlags::IsHeadMounted; }

	// IXRTrackingSystem
	virtual bool DoesSupportPositionalTracking() const override;
	virtual bool HasValidTrackingPosition() override;
	virtual bool EnumerateTrackedDevices(TArray<int32>& OutDevices, EXRTrackedDeviceType Type = EXRTrackedDeviceType::Any) override;
	virtual bool GetCurrentPose(int32 DeviceId, FQuat& OutOrientation, FVector& OutPosition) override;
	virtual void SetTrackingOrigin(EHMDTrackingOrigin::Type NewOrigin) override;
	virtual EHMDTrackingOrigin::Type GetTrackingOrigin() const override;

	virtual bool GetRelativeEyePose(int32 DeviceId, EStereoscopicPass Eye, FQuat& OutOrientation, FVector& OutPosition) override;
	virtual void ResetOrientationAndPosition(float Yaw = 0.f) override;
	virtual void ResetOrientation(float Yaw = 0.f) override;
	virtual void ResetPosition() override;
	virtual void SetBaseRotation(const FRotator& BaseRot) override;
	virtual FRotator GetBaseRotation() const override;
	virtual void SetBaseOrientation(const FQuat& BaseOrient) override;
	virtual FQuat GetBaseOrientation() const override;
	virtual class IHeadMountedDisplay* GetHMDDevice() override;
	virtual class TSharedPtr< class IStereoRendering, ESPMode::ThreadSafe > GetStereoRenderingDevice() override;
	virtual bool OnStartGameFrame(FWorldContext& WorldContext) override;
	virtual bool OnEndGameFrame(FWorldContext& WorldContext) override;
	virtual void OnBeginRendering_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneViewFamily& ViewFamily) override;

	// IHeadMountedDisplay
	virtual bool IsHMDConnected() override { return true; }
	virtual bool IsHMDEnabled() const override { return true; };
	virtual EHMDWornState::Type GetHMDWornState();
	virtual void EnableHMD(bool allow = true) override;
	virtual bool GetHMDMonitorInfo(MonitorInfo&) override;
	virtual void GetFieldOfView(float& OutHFOVInDegrees, float& OutVFOVInDegrees) const override;
	virtual void SetInterpupillaryDistance(float NewInterpupillaryDistance) override;
	virtual float GetInterpupillaryDistance() const override;
	virtual bool GetHMDDistortionEnabled(EShadingPath ShadingPath) const override;
	virtual bool IsChromaAbCorrectionEnabled() const override;
	virtual float GetPixelDenity() const override;
	virtual void SetPixelDensity(const float NewPixelDensity) override;
	virtual FIntPoint GetIdealRenderTargetSize() const override;

	// IStereoRendering
	virtual bool IsStereoEnabled() const override;
	virtual bool EnableStereo(bool stereo = true) override;
	virtual void AdjustViewRect(EStereoscopicPass StereoPass, int32& X, int32& Y, uint32& SizeX, uint32& SizeY) const override;
	virtual FMatrix GetStereoProjectionMatrix(const enum EStereoscopicPass StereoPassType) const override;
	virtual IStereoRenderTargetManager* GetRenderTargetManager() override { return this; }

	// ISceneViewExtension
	virtual void SetupViewFamily(FSceneViewFamily& InViewFamily) override;
	virtual void SetupView(FSceneViewFamily& InViewFamily, FSceneView& InView) override;
	virtual void BeginRenderViewFamily(FSceneViewFamily& InViewFamily) override;
	virtual void PreRenderViewFamily_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneViewFamily& InViewFamily) override;
	virtual void PreRenderView_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneView& InView) override;
	virtual void PostRenderViewFamily_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneViewFamily& InViewFamily) override;
	virtual int32 GetPriority() const override { return -1; }

	// IStereoRenderTargetManager
	virtual bool ShouldUseSeparateRenderTarget() const override;
	virtual void CalculateRenderTargetSize(const class FViewport& Viewport, uint32& InOutSizeX, uint32& InOutSizeY) override;
	virtual bool NeedReAllocateViewportRenderTarget(const FViewport& Viewport) override;
	virtual uint32 GetNumberOfBufferedFrames() const { return 1; }
	virtual bool AllocateRenderTargetTexture(uint32 Index, uint32 SizeX, uint32 SizeY, uint8 Format, uint32 NumMips, ETextureCreateFlags InTexFlags, ETextureCreateFlags InTargetableTextureFlags, FTexture2DRHIRef& OutTargetableTexture, FTexture2DRHIRef& OutShaderResourceTexture, uint32 NumSamples = 1) override;
	virtual FXRRenderBridge* GetActiveRenderBridge_GameThread(bool bUseSeparateRenderTarget) override;

	//IStereoLayers
	virtual IStereoLayers* GetStereoLayers() override { return this; }
	virtual uint32 CreateLayer(const IStereoLayers::FLayerDesc& InLayerDesc) override;
	virtual void DestroyLayer(uint32 LayerId) override;
	virtual void SetLayerDesc(uint32 LayerId, const IStereoLayers::FLayerDesc& InLayerDesc) override;
	virtual bool GetLayerDesc(uint32 LayerId, IStereoLayers::FLayerDesc& OutLayerDesc) override;
	virtual void MarkTextureForUpdate(uint32 LayerId) override;
	virtual IStereoLayers::FLayerDesc GetDebugCanvasLayerDesc(FTextureRHIRef Texture) override;
	virtual bool ShouldCopyDebugLayersToSpectatorScreen() const override { return true; }

protected:
	// FXRTrackingSystemBase
	virtual float GetWorldToMetersScale() const override;

public:
	bool IsInitialized() const;
	void PollSystemEvent();
	bool IsAppFocused = true;

#if PLATFORM_ANDROID
	void ProcessEvent(const yvrEventDataBody& EventData);

	const uint32& GetRHIIndex();
	const yvrPoseState& GetRHIHeadPoseState();

	//helper
	static inline FVector ToFVector(const yvrVector3f& InVec)
	{
		return FVector(-InVec.z, InVec.x, InVec.y);
	}

	static inline yvrVector3f ToYvrFVector(const FVector& InVec)
	{
		yvrVector3f Temp;
		Temp.x = InVec.Y;
		Temp.y = InVec.Z;
		Temp.z = -InVec.X;
		return Temp;
	}

	static inline FMatrix ToFMatrix(const yvrMatrix4f& vtm)
	{
		return FMatrix(
			FPlane(vtm.M[0][0], vtm.M[0][1], vtm.M[0][2], vtm.M[0][3]),
			FPlane(vtm.M[1][0], vtm.M[1][1], vtm.M[1][2], vtm.M[1][3]),
			FPlane(vtm.M[2][0], vtm.M[2][1], vtm.M[2][2], vtm.M[2][3]),
			FPlane(vtm.M[3][0], vtm.M[3][1], vtm.M[3][2], vtm.M[3][3]));
	}

	static inline yvrMatrix4f ToYvrMatrix(const FMatrix& vtm)
	{
		yvrMatrix4f Temp;
		Temp.M[0][0] = vtm.M[0][0];	Temp.M[1][0] = vtm.M[0][1]; Temp.M[2][0] = vtm.M[0][2];	Temp.M[3][0] = vtm.M[0][3];
		Temp.M[0][1] = vtm.M[1][0]; Temp.M[1][1] = vtm.M[1][1]; Temp.M[2][1] = vtm.M[1][2]; Temp.M[3][1] = vtm.M[1][3];
		Temp.M[0][2] = vtm.M[2][0]; Temp.M[1][2] = vtm.M[2][1]; Temp.M[2][2] = vtm.M[2][2]; Temp.M[3][2] = vtm.M[2][3];
		Temp.M[0][3] = vtm.M[3][0]; Temp.M[1][3] = vtm.M[3][1]; Temp.M[2][3] = vtm.M[3][2]; Temp.M[3][3] = vtm.M[3][3];
		return Temp;
	}

	static inline yvrVector4f LinearColorToYvrVector4f(const FLinearColor& InColor)
	{
		return yvrVector4f{ InColor.R, InColor.G, InColor.B, InColor.A };
	}

	static inline yvrDeviceNode ToYvrNode(EYvrTrackedDeviceType Source)
	{
		yvrDeviceNode Destination = NodeCount;

		switch (Source)
		{
		case EYvrTrackedDeviceType::None:
			Destination = NodeCount;
			break;
		case EYvrTrackedDeviceType::HMD:
			Destination = Hmd;
			break;
		case EYvrTrackedDeviceType::LTouch:
			Destination = LeftController;
			break;
		case EYvrTrackedDeviceType::RTouch:
			Destination = RightController;
			break;
		default:
			break;
		}
		return Destination;
	}
#endif
	void SetBasePosition(const FVector& BaseOffsetInMeters);
	FVector GetBasePosition() const;
	void SetFFRLevel(const EYvrFixedFoveatedRenderingLevel Level);
	EYvrFixedFoveatedRenderingLevel GetFFRLevel() const;
	void SetDisplayFrequency(float Frequency);
	void SetColorScaleAndOffset(FLinearColor ColorScale, FLinearColor ColorOffset, bool bApplyToAllLayers);

	void SubmitFrame();


#if PLATFORM_ANDROID
	void PoseToOrientationAndPosition(const yvrPosef& Pose, FQuat& CurrentOrientation, FVector& CurrentPosition, const float WorldToMetersScale);
#endif

private:
	bool Startup();
	void Shutdown();

	void BeginVRMode();
	void UpdateCachedHeadPose();
	void Recenter(FRecenterTypes RecenterType, float Yaw);
	void ApplicationWillEnterBackgroundDelegate();
	void ApplicationHasEnteredForegroundDelegate();

	void InitializeEyeLayer();

	bool bIsStereoEnabled;
	bool bIsRenderThreadInit;
	bool bIsForeground;
	bool bUsesArrayTexture;

	FIntPoint RenderTargetSize;

	EHMDTrackingOrigin::Type CurTrackingOrigin;
	//Game Thread
	FTransform TrackingToWorld;
	uint32 NextLayerId;
	TMap<uint32, FYvrLayerPtr> LayerMap;
	////Render Thread
	FYvrLayerPtr EyeLayer_RenderThread;
	TArray<FYvrLayerPtr> Layers_RenderThread;
	////RHI Thread
	TArray<FYvrLayerPtr> Layers_RHIThread;

#if PLATFORM_ANDROID
	yvrSubmitFrameDescription2 YvrLayerSubmit;
	// Game Thread
	TTuple<uint32, yvrPoseState> CurrentPoseState_GameThread;
	// Render Thread
	TTuple<uint32, yvrPoseState> LastPoseState_RenderThread;
	TTuple<uint32, yvrPoseState> CurrentPoseState_RenderThread;
	// RHI Thread
	TTuple<uint32, yvrPoseState> CurrentPoseState_RHIThread;
#endif
	TRefCountPtr<FYvrCustomPresent> CustomPresent;
	UYvrHMDSettings *Settings;
};