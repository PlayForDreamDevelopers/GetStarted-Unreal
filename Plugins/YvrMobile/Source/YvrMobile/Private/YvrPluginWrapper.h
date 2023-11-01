#pragma once

#if PLATFORM_ANDROID
#include "YvrLib_Api.h"
#include "YvrLib_Compile.h"
#include "YvrLib_Types.h"
#endif

class YvrPluginWrapper
{
public:
    YvrPluginWrapper() {
    }
    ~YvrPluginWrapper() {
    }

    static void SetCpuLevel(const uint32 CpuLevel);
    static void SetGpuLevel(const uint32 GpuLevel);
    static void SetClockLevels(const uint32 CpuLevel, const uint32 GpuLevel);
    static float GetGpuUtilization();
    static float GetDisplayRefreshRate();
    static float GetIPD();
    static void SetDisplayRefreshRate(const float refreshRate);
    static bool GetHMDWornState();
    static float GetPredictedDisplayTime();
    static FIntPoint GetRenderTargetSize();
    static FVector4 GetEyeFov(int Index);
    static FVector4 GetFoveationParameter(int Index);

    static void Shutdown();
    static void LeaveVrMode();
#if PLATFORM_ANDROID
    static yvrInitializeStatus Initialize(const yvrInitParams* InitParams);
    static void EnterVrMode(const yvrBeginParams* Parms);
    static void SetPerfThread(const yvrPerfThreadType Type, const uint32 ThreadId);
    static void GetControllerConnectedInfo(yvrControllerConnectedInfo* Info);
    static void SubmitFrame(yvrSubmitFrameDescription2* FrameDescription);
    static void GetNodePose(yvrDeviceNode NodeId, const float PredictTimeMs, yvrPoseState& Pose);
    static void GetControllerState(yvrController Mask, yvrControllerState* State);
    static void SetControllerVibration(yvrController Mask, float Frequency, float Amplitude);
    static void SetTrackingSpace(yvrTrackingSpace Space);
    static void GetTrackingSpace(yvrTrackingSpace& Space);
    static void SetExtraLatencyMode(yvrExtraLatencyMode mode);

    //boundary
    static void TestBoundaryNode(const yvrDeviceNode DeviceNode, BoundaryTestResult* Result);
    static void TestBoundaryPoint(const BoundaryPoint Point, BoundaryTestResult* Result);
    static bool GetBoundaryDimensions(BoundaryDimensions& Dimensions);
    static bool GetBoundaryVisible();
    static bool SetBoundaryVisible(bool Visible);
    static int GetBoundaryGeometryPointsCount();
    static void GetBoundaryGeometry(yvrVector3f* Geometry);
    static float GetGroundDistance();
    static bool GetBoundaryConfigured();

    static yvrTextureSwapChain* CreateTextureSwapChain(yvrSwapChainCreateInfo* CreateInfo);
    static int GetTextureSwapChainHandle(yvrTextureSwapChain* Chain, int Index);
    static void DestroyTextureSwapChain(yvrTextureSwapChain* Chain);
    static bool PollEvent(yvrEventDataBody& Event);
#endif
};


bool InitializeYvrPluginWrapper(YvrPluginWrapper* wrapper);
void DestroyYvrPluginWrapper(YvrPluginWrapper* wrapper);