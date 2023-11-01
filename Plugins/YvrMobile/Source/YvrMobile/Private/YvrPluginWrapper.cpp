#include "YvrPluginWrapper.h"
#include "YvrMobile.h"

bool InitializeYvrPluginWrapper(YvrPluginWrapper* wrapper)
{
	return true;
}

void DestroyYvrPluginWrapper(YvrPluginWrapper* wrapper)
{

}

void YvrPluginWrapper::SetCpuLevel(uint32 CpuLevel)
{
#if PLATFORM_ANDROID
	uint32 GpuLevel = yvrlib_GetSystemPropertyInt(YVRLIB_SYS_PROP_GPU_LEVEL);
	yvrlib_SetClockLevels(CpuLevel, GpuLevel);
#endif
}

void YvrPluginWrapper::SetGpuLevel(uint32 GpuLevel)
{
#if PLATFORM_ANDROID
	uint32 CpuLevel = yvrlib_GetSystemPropertyInt(YVRLIB_SYS_PROP_CPU_LEVEL);
	yvrlib_SetClockLevels(CpuLevel, GpuLevel);
#endif
}

void YvrPluginWrapper::SetClockLevels(const uint32 CpuLevel, const uint32 GpuLevel)
{
#if PLATFORM_ANDROID
	yvrlib_SetClockLevels(CpuLevel, GpuLevel);
#endif
}

float YvrPluginWrapper::GetGpuUtilization()
{
#if PLATFORM_ANDROID
	return yvrlib_GetSystemPropertyFloat(YVRLIB_SYS_PROP_GPU_UTILIZATION);
#endif
	return 0.0f;
}

float YvrPluginWrapper::GetDisplayRefreshRate()
{
#if PLATFORM_ANDROID
	return yvrlib_GetSystemPropertyFloat(YVRLIB_SYS_PROP_DISPLAY_REFRESH_RATE);
#endif
	return 0.0f;
}

float YvrPluginWrapper::GetIPD()
{
#if PLATFORM_ANDROID
	return yvrlib_GetSystemPropertyFloat(YVRLIB_SYS_PROP_EYE_IPD);
#endif
	return 0.0f;
}

void YvrPluginWrapper::SetDisplayRefreshRate(const float refreshRate)
{
#if PLATFORM_ANDROID
	yvrlib_SetDisplayRefreshRate(refreshRate);
#endif
}

bool YvrPluginWrapper::GetHMDWornState()
{
#if PLATFORM_ANDROID
	return yvrlib_GetSystemPropertyBool(YVRLIB_SYS_PROP_HMD_MOUNT);
#endif
	return false;
}

float YvrPluginWrapper::GetPredictedDisplayTime()
{
#if PLATFORM_ANDROID
	return yvrlib_GetPredictedDisplayTime();
#endif
	return 0.0f;
}

FIntPoint YvrPluginWrapper::GetRenderTargetSize()
{
#if PLATFORM_ANDROID
	int x = yvrlib_GetSystemPropertyInt(YVRLIB_SYS_PROP_RENDER_PIXELS_WIDTH);
	int y = yvrlib_GetSystemPropertyInt(YVRLIB_SYS_PROP_RENDER_PIXELS_HEIGHT);
	return FIntPoint(x, y);
#endif
	return FIntPoint();
}

FVector4 YvrPluginWrapper::GetEyeFov(int Index)
{
#if PLATFORM_ANDROID
	//left right up down
	float values[4] = { 0.0f };
	//left
	if (Index == 0)
	{
		yvrlib_GetSystemPropertyFloatArray(YVRLIB_SYS_PROP_LEFT_EYE_FOV, values, 4);
	}
	//right
	else if (Index == 1)
	{
		yvrlib_GetSystemPropertyFloatArray(YVRLIB_SYS_PROP_RIGHT_EYE_FOV, values, 4);
	}
	return FVector4(values[0], values[1], values[2], values[3]);
#endif
	return FVector4();
}

FVector4 YvrPluginWrapper::GetFoveationParameter(int Index)
{
#if PLATFORM_ANDROID
	float values[4] = { 0.0f };
	switch (Index)
	{
	case 0:
		yvrlib_GetSystemPropertyFloatArray(YVRLIB_SYS_PROP_FOVEATION_LOW, values, 4);
		break;
	case 1:
		yvrlib_GetSystemPropertyFloatArray(YVRLIB_SYS_PROP_FOVEATION_MED, values, 4);
		break;
	case 2:
		yvrlib_GetSystemPropertyFloatArray(YVRLIB_SYS_PROP_FOVEATION_HIGH, values, 4);
		break;
	case 3:
		yvrlib_GetSystemPropertyFloatArray(YVRLIB_SYS_PROP_FOVEATION_TOP, values, 4);
		break;
	default:
		break;
	}
	return FVector4(values[0], values[1], values[2], values[3]);
#endif
	return FVector4();
}

void YvrPluginWrapper::Shutdown()
{
#if PLATFORM_ANDROID
	yvrlib_Shutdown();
#endif
}

void YvrPluginWrapper::LeaveVrMode()
{
#if PLATFORM_ANDROID
	yvrlib_LeaveVrMode();
#endif
}

#if PLATFORM_ANDROID
yvrInitializeStatus YvrPluginWrapper::Initialize(const yvrInitParams* InitParams)
{
	return yvrlib_Initialize(InitParams);
}
void YvrPluginWrapper::EnterVrMode(const yvrBeginParams* Parms)
{
	yvrlib_EnterVrMode(Parms);
}
void YvrPluginWrapper::GetControllerConnectedInfo(yvrControllerConnectedInfo* Info)
{
	yvrlib_GetControllerConnectedInfo(Info);
}
void YvrPluginWrapper::SubmitFrame(yvrSubmitFrameDescription2* FrameDescription)
{
	yvrlib_SubmitFrame2(FrameDescription);
}
void YvrPluginWrapper::GetNodePose(yvrDeviceNode NodeId, const float PredictTimeMs, yvrPoseState& Pose)
{
	yvrlib_GetNodePose(NodeId, PredictTimeMs, Pose);
}
void YvrPluginWrapper::GetControllerState(yvrController Mask, yvrControllerState* State)
{
	yvrlib_GetControllerState(Mask, State);
}
void YvrPluginWrapper::SetControllerVibration(yvrController Mask, float Frequency, float Amplitude)
{
	yvrlib_SetControllerVibration(Mask, Frequency, Amplitude);
}
void YvrPluginWrapper::SetTrackingSpace(yvrTrackingSpace Space)
{
	yvrlib_SetTrackingSpace(Space);
}
void YvrPluginWrapper::GetTrackingSpace(yvrTrackingSpace& Space)
{
	yvrlib_GetTrackingSpace(Space);
}
void YvrPluginWrapper::SetExtraLatencyMode(yvrExtraLatencyMode mode)
{
	yvrlib_SetExtraLatencyMode(mode);
}
void YvrPluginWrapper::TestBoundaryNode(const yvrDeviceNode DeviceNode, BoundaryTestResult* Result)
{
	yvrlib_TestBoundaryNode(DeviceNode, Result);
}
void YvrPluginWrapper::TestBoundaryPoint(const BoundaryPoint Point, BoundaryTestResult* Result)
{
	yvrlib_TestBoundaryPoint(Point, Result);
}
bool YvrPluginWrapper::GetBoundaryDimensions(BoundaryDimensions& Dimensions)
{
	return yvrlib_GetBoundaryDimensions(Dimensions);
}
bool YvrPluginWrapper::GetBoundaryVisible()
{
	return yvrlib_GetBoundaryVisible();
}
bool YvrPluginWrapper::SetBoundaryVisible(bool Visible)
{
	return yvrlib_SetBoundaryVisible(Visible);
}
int YvrPluginWrapper::GetBoundaryGeometryPointsCount()
{
	return yvrlib_GetBoundaryGeometryPointsCount();
}
void YvrPluginWrapper::GetBoundaryGeometry(yvrVector3f* Geometry)
{
	yvrlib_GetBoundaryGeometry(Geometry);
}
float YvrPluginWrapper::GetGroundDistance()
{
	return yvrlib_GetGroundDistance();
}
bool YvrPluginWrapper::GetBoundaryConfigured()
{
	return yvrlib_GetBoundaryConfigured();
}
yvrTextureSwapChain* YvrPluginWrapper::CreateTextureSwapChain(yvrSwapChainCreateInfo* CreateInfo)
{
	return yvrlib_CreateTextureSwapChain(CreateInfo);
}
int YvrPluginWrapper::GetTextureSwapChainHandle(yvrTextureSwapChain* Chain, int Index)
{
	return yvrlib_GetTextureSwapChainHandle(Chain, Index);
}
void YvrPluginWrapper::DestroyTextureSwapChain(yvrTextureSwapChain* Chain)
{
	yvrlib_DestroyTextureSwapChain(Chain);
}
bool YvrPluginWrapper::PollEvent(yvrEventDataBody& Event)
{
	return yvrlib_PollEvent(Event);
}
void YvrPluginWrapper::SetPerfThread(const yvrPerfThreadType Type, const uint32 ThreadId)
{
	yvrlib_SetPerfThread(Type, ThreadId);
}
#endif