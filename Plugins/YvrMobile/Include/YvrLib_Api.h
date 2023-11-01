#ifndef YVRLIB_API_H
#define YVRLIB_API_H

#include "YvrLib_Compile.h"
#include "YvrLib_Types.h"

#if defined(__cplusplus)
extern "C"
{
#endif

    YVRLIB_EXPORT const char* yvrlib_GetVersionString();

    YVRLIB_EXPORT yvrInitializeStatus yvrlib_Initialize(const yvrInitParams* pInitParams);

    YVRLIB_EXPORT void yvrlib_Shutdown();

    YVRLIB_EXPORT yvrResult yvrlib_EnterVrMode(const yvrBeginParams* parms);

    YVRLIB_EXPORT void yvrlib_LeaveVrMode();

    YVRLIB_EXPORT float yvrlib_GetPredictedDisplayTime();

    YVRLIB_EXPORT yvrResult yvrlib_GetNodePose(yvrDeviceNode nodeId, const float predictTimeMs, yvrPoseState& pose);

    YVRLIB_EXPORT yvrResult yvrlib_GetPredictedPoseForRendering(const float predictedTimeMs, yvrPoseState& headPose,
                                                                yvrPoseState& lController, yvrPoseState& rController);

    YVRLIB_EXPORT yvrResult yvrlib_GetHistoricHeadPose(int64_t timestampNs, yvrPoseState& head);

    YVRLIB_EXPORT yvrResult yvrlib_GetControllerConnectedInfo(yvrControllerConnectedInfo* info);

    YVRLIB_EXPORT yvrResult yvrlib_GetControllerState(yvrController mask, yvrControllerState* state);

    YVRLIB_EXPORT yvrResult yvrlib_SetControllerVibration(yvrController mask, float frequency, float amplitude);

    YVRLIB_EXPORT yvrResult yvrlib_GetHandPose(const float predictTimeMs, HandPoseHeader* header);

    YVRLIB_EXPORT yvrResult yvrlib_GetHandSkeleton(const Handedness handedness, HandSkeletonHeader* header);

    YVRLIB_EXPORT yvrResult yvrlib_GetHandMesh(const Handedness handedness, HandMeshHeader* header);

    YVRLIB_EXPORT yvrResult yvrlib_GetHandCurrentInputState(const Handedness handedness, InputStateHeader* header);

    YVRLIB_EXPORT bool yvrlib_PollEvent(yvrEventDataBody& event);

    YVRLIB_EXPORT yvrTextureSwapChain* yvrlib_CreateTextureSwapChain(yvrSwapChainCreateInfo* createInfo);

    YVRLIB_EXPORT void yvrlib_DestroyTextureSwapChain(yvrTextureSwapChain* chain);

    YVRLIB_EXPORT unsigned int yvrlib_GetTextureSwapChainHandle(yvrTextureSwapChain* chain, int index);

    YVRLIB_EXPORT unsigned int yvrlib_GetTextureSwapChainLength(yvrTextureSwapChain* chain);

    YVRLIB_EXPORT void yvrlib_SubmitFrame(yvrSubmitFrameDescription* desc);

    YVRLIB_EXPORT void yvrlib_SubmitFrame2(yvrSubmitFrameDescription2* desc2);

    YVRLIB_EXPORT double yvrlib_GetTimeInSeconds();

    YVRLIB_EXPORT yvrResult yvrlib_GetSystemPropertyInt(const yvrSystemProperty propType);

    YVRLIB_EXPORT float yvrlib_GetSystemPropertyFloat(const yvrSystemProperty propType);

    YVRLIB_EXPORT yvrResult yvrlib_GetSystemPropertyFloatArray(const yvrSystemProperty propType, float* values, int numArrayValues);

    YVRLIB_EXPORT bool yvrlib_GetSystemPropertyBool(const yvrSystemProperty propType);

    YVRLIB_EXPORT yvrResult yvrlib_SetClockLevels(const uint32_t cpuLevel, const uint32_t gpuLevel);

    YVRLIB_EXPORT yvrResult yvrlib_SetPerfThread(const yvrPerfThreadType type, const uint32_t threadId);

    YVRLIB_EXPORT yvrResult yvrlib_SetDisplayRefreshRate(const float refreshRate);

    YVRLIB_EXPORT yvrResult yvrlib_SetTrackingSpace(yvrTrackingSpace whichSpace);

    YVRLIB_EXPORT yvrResult yvrlib_GetTrackingSpace(yvrTrackingSpace& space);

    YVRLIB_EXPORT yvrResult yvrlib_LocateTrackingSpace(yvrTrackingSpace target, yvrPosef& pose);

    YVRLIB_EXPORT void yvrlib_TestBoundaryNode(const yvrDeviceNode deviceNode, BoundaryTestResult* result);

    YVRLIB_EXPORT void yvrlib_TestBoundaryPoint(const BoundaryPoint point, BoundaryTestResult* result);

    YVRLIB_EXPORT bool yvrlib_GetBoundaryDimensions(BoundaryDimensions& dimensions);

    YVRLIB_EXPORT bool yvrlib_GetBoundaryVisible();

    YVRLIB_EXPORT bool yvrlib_SetBoundaryVisible(bool visible);

    YVRLIB_EXPORT int yvrlib_GetBoundaryGeometryPointsCount();

    YVRLIB_EXPORT void yvrlib_GetBoundaryGeometry(yvrVector3f* geometry);

    YVRLIB_EXPORT float yvrlib_GetGroundDistance();

    YVRLIB_EXPORT bool yvrlib_GetBoundaryConfigured();

    YVRLIB_EXPORT yvrResult yvrlib_SetExtraLatencyMode(yvrExtraLatencyMode mode);

    YVRLIB_EXPORT yvrResult yvrlib_WaitFrame(uint64_t frameIndex);

    YVRLIB_EXPORT yvrResult yvrlib_BeginFrame(uint64_t frameIndex);

    YVRLIB_EXPORT bool yvrlib_SetAswEnable(bool enable);

    YVRLIB_EXPORT bool yvrlib_GetAswEnable();

    YVRLIB_EXPORT yvrResult yvrlib_SetPrimaryController(yvrController mask);

    YVRLIB_EXPORT yvrResult yvrlib_GetPrimaryController(yvrController* mask);

#if defined(__cplusplus)
}  // extern "C"
#endif

#endif  // YVRLIB_H
