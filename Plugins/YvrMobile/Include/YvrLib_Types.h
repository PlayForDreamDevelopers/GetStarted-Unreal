#ifndef YVRLIB_TYPES_H
#define YVRLIB_TYPES_H

#include <jni.h>
#include <stdbool.h>
#include <stdint.h>

#include "YvrLib_Compile.h"

#define YVR_MAX_RENDER_LAYERS 16

/// Java details about an activity
typedef struct yvrJava_
{
    JavaVM* Vm;              //< Java Virtual Machine
    JNIEnv* Env;             //< Thread specific environment
    jobject ActivityObject;  //< Java activity object
} yvrJava;
YVRLIB_ASSERT_TYPE_SIZE_32_BIT(yvrJava, 12);
YVRLIB_ASSERT_TYPE_SIZE_64_BIT(yvrJava, 24);

/// Return result
typedef signed int yvrResult;

/// Return successfully result
typedef enum yvrSuccessResult_
{
    yvrSuccess = 0,
} yvrSuccessResult;

/// Error code define
typedef enum yvrErrorResult_
{
    yvrError_unkown = -1000,
    yvrError_UnSupported = -1001,
    yvrError_InvalidParameter = -1002,
    yvrError_InvalidOperation = -1003,
    yvrError_InternalError = -1004,

    yvrResult_EnumSize = 0x7ffffff
} yvrErrorResult;

/// A 2D vector.
typedef struct yvrVector2f_
{
    float x, y;
} yvrVector2f;
YVRLIB_ASSERT_TYPE_SIZE(yvrVector2f, 8);

/// A 3D vector.
typedef struct yvrVector3f_
{
    float x, y, z;
} yvrVector3f;
YVRLIB_ASSERT_TYPE_SIZE(yvrVector3f, 12);

/// A 4D vector.
typedef struct yvrVector4f_
{
    float x, y, z, w;
} yvrVector4f;
YVRLIB_ASSERT_TYPE_SIZE(yvrVector4f, 16);

typedef struct yvrVector4s_
{
    int16_t x, y, z, w;
} yvrVector4s;
YVRLIB_ASSERT_TYPE_SIZE(yvrVector4s, 8);

/// Quaternion.
typedef struct yvrQuatf_
{
    float x, y, z, w;
} yvrQuatf;
YVRLIB_ASSERT_TYPE_SIZE(yvrQuatf, 16);

/// Row-major 4x4 matrix.
typedef struct yvrMatrix4f_
{
    float M[4][4];
} yvrMatrix4f;
YVRLIB_ASSERT_TYPE_SIZE(yvrMatrix4f, 64);

/// Row-major 1x16 matrix. -> yvrMatrix
typedef struct yvrMatrix16f_
{
    float M[16];
} yvrMatrix16f;
YVRLIB_ASSERT_TYPE_SIZE(yvrMatrix16f, 64);

typedef struct yvrRect4f_
{
    float x1, y1, x2, y2;
} yvrRect4f;
YVRLIB_ASSERT_TYPE_SIZE(yvrRect4f, 16);

/// Event type define
typedef enum yvrEventType_
{
    // No event
    YVRLIB_EVENT_NONE = 0,
    // Events were lost
    YVRLIB_EVENT_DATA_LOST = 1,
    // The application's frames are visible to the user.
    YVRLIB_EVENT_VISIBILITY_GAINED = 2,
    // The application's frames are no longer visible to the user.
    YVRLIB_EVENT_VISIBILITY_LOST = 3,
    // The current activity is in the foreground and has input focus.
    YVRLIB_EVENT_FOCUS_GAINED = 4,
    // The current activity is in the background (but possibly still visible) and has lost input
    // focus.
    YVRLIB_EVENT_FOCUS_LOST = 5,
    // The display refresh rate has changed
    YVRLIB_EVENT_DISPLAY_REFRESH_RATE_CHANGE = 6,
} yvrEventType;

typedef struct yvrEventHeader_
{
    yvrEventType eventType;
    uint64_t eventTimeStamp;
} yvrEventHeader;

typedef struct yvrEventDataLost_
{
    yvrEventHeader eventHeader;
} yvrEventDataLost;

typedef struct yvrEventVisibilityGained_
{
    yvrEventHeader eventHeader;
} yvrEventVisibilityGained;

typedef struct yvrEventVisibilityLost_
{
    yvrEventHeader eventHeader;
} yvrEventVisibilityLost;

typedef struct yvrEventFocusGained_
{
    yvrEventHeader eventHeader;
} yvrEventFocusGained;

typedef struct yvrEventFocusLost_
{
    yvrEventHeader eventHeader;
} yvrEventFocusLost;

typedef struct yvrEventDataBody_
{
    yvrEventHeader eventHeader;
    unsigned char eventData[40];
} yvrEventDataBody;

#define YVRLIB_LARGEST_EVENT_TYPE yvrEventDataBody

typedef enum yvrEventSize_
{
    YVRLIB_MAX_EVENT_SIZE = sizeof(YVRLIB_LARGEST_EVENT_TYPE)
} yvrEventSize;

typedef enum yvrDeviceNode_
{
    LeftController = 0,
    RightController,
    Hmd,
    LeftEye,
    RightEye,
    NodeCount
} yvrDeviceNode;

//! \brief Enum used to indicate which controller is being used
typedef enum yvrController_ : uint8_t
{
    kControllerLeft = 0,
    kControllerRight = 1,
    kNumControllers = 2
} yvrController;

struct ANativeWindow;

//! \brief Enum used to indicate which eye is being used
typedef enum yvrEye_
{
    kLeftEye = 0,
    kRightEye = 1,
    kNumEyes = 2,
} yvrEye;

//! \brief Enum used to indicate which eye to apply a render layer
typedef enum yvrEyeMask_
{
    kEyeMaskLeft = 0x00000001,
    kEyeMaskRight = 0x00000002,
    kEyeMaskBoth = 0x00000003
} yvrEyeMask;

//! \brief Enum used to indicate which colorspace is being used
typedef enum yvrColorSpace_
{
    kColorSpaceLinear = 0,
    kColorSpaceSRGB = 1,
    kNumColorSpaces
} yvrColorSpace;

/// Identifies a VR-related application thread.
typedef enum yvrPerfThreadType_
{
    YVRLIB_PERF_THREAD_TYPE_MAIN = 0,
    YVRLIB_PERF_THREAD_TYPE_RENDERER = 1,
} yvrPerfThreadType;

//! \brief Flags used to set YVR options
typedef enum yvrOptionFlags_
{
    kProtectedContent = (1 << 0),
    kMotionAwareFrames = (1 << 1),
    kFoveationSubsampled = (1 << 2),
    kPhaseSync = (1 << 3),
} yvrOptionFlags;

typedef enum yvrBatteryStatus_
{
    BATTERY_STATUS_UNKNOWN = 1,
    BATTERY_STATUS_CHARGING = 2,
    BATTERY_STATUS_DISCHARGING = 3,
    BATTERY_STATUS_NOT_CHARGING = 4,
    BATTERY_STATUS_FULL = 5,
} yvrBatteryStatus;

typedef enum yvrPowerSavingState_
{
    OUT_POWER_SAVING_MODE = 0,
    IN_POWER_SAVING_MODE = 1,
    POWER_SAVING_MODE_UNKOWN = 2,
} yvrPowerSavingState;

typedef enum yvrSystemProperty_
{
    YVRLIB_SYS_PROP_DISPLAY_PIXELS_WIDTH = 0,  // int
    YVRLIB_SYS_PROP_DISPLAY_PIXELS_HEIGHT,     // int
    YVRLIB_SYS_PROP_RENDER_PIXELS_WIDTH,       // int
    YVRLIB_SYS_PROP_RENDER_PIXELS_HEIGHT,      // int
    YVRLIB_SYS_PROP_DISPLAY_REFRESH_RATE,      // float
    YVRLIB_SYS_PROP_BATTERY_LEVEL,             // float
    YVRLIB_SYS_PROP_BATTERY_TEMPERATURE,       // float
    YVRLIB_SYS_PROP_CHARGE_STATUS,             // int
    YVRLIB_SYS_PROP_VOLUME_LEVEL,              // float
    YVRLIB_SYS_PROP_POWERSAVING_MODE,          // int
    YVRLIB_SYS_PROP_CPU_UTILIZATION,           // float
    YVRLIB_SYS_PROP_GPU_UTILIZATION,           // float
    YVRLIB_SYS_PROP_CPU_LEVEL,                 // int
    YVRLIB_SYS_PROP_GPU_LEVEL,                 // int
    YVRLIB_SYS_PROP_HMD_MOUNT,                 // bool
    YVRLIB_SYS_PROP_LEFT_EYE_FOV,              // float array
    YVRLIB_SYS_PROP_RIGHT_EYE_FOV,             // float array
    YVRLIB_SYS_PROP_EYE_IPD,                   // float
    YVRLIB_SYS_PROP_FOVEATION_LOW,             // float array
    YVRLIB_SYS_PROP_FOVEATION_MED,             // float array
    YVRLIB_SYS_PROP_FOVEATION_HIGH,            // float array
    YVRLIB_SYS_PROP_FOVEATION_TOP,             // float array
    YVRLLIB_SYS_PROP_LEFT_FRUSTUM_POSITION,    // float array
    YVRLLIB_SYS_PROP_LEFT_FRUSTUM_ROTATION,    // float array
    YVRLLIB_SYS_PROP_RIGHT_FRUSTUM_POSITION,   // float array
    YVRLLIB_SYS_PROP_RIGHT_FRUSTUM_ROTATION,   // float array
} yvrSystemProperty;

/// Position and orientation together.
typedef struct yvrPosef_
{
    union
    {
        yvrQuatf rotation;
        yvrQuatf orientation;
    };
    union
    {
        yvrVector3f position;
        yvrVector3f translation;
    };
} yvrPosef;
YVRLIB_ASSERT_TYPE_SIZE(yvrPosef, 28);

// pose status mask
typedef enum yvrPoseStatus_
{
    POSE_STATUS_INVALID = 0,                 //< Orientation and Position invalid.
    POSE_STATUS_ORIENTATION_VALID = 1 << 0,  //< Orientation is currently valid.
    POSE_STATUS_POSITION_VALID = 1 << 1,     //< Position is currently valid.
} yvrPoseStatus;

typedef struct yvrPoseState_
{
    yvrPosef pose;
    int32_t poseStatus; // see yvrPoseStatus
    uint64_t poseTimeStampNs;
    uint64_t poseFetchTimeNs;
    uint64_t expectedDisplayTimeNs;
    yvrVector3f angularVelocity;
    yvrVector3f linearVelocity;
    yvrVector3f angularAcceleration;
    yvrVector3f linearAcceleration;
} yvrPoseState;
YVRLIB_ASSERT_TYPE_SIZE(yvrPoseState, 104);

//! \brief Enum used to indicate the type of texture passed in as a render layer
//! \sa yvrSubmitFrame
typedef enum yvrTextureType_
{
    kTypeTexture = 0,      //!< Standard texture
    kTypeTextureArray,     //!< Standard texture array (Left eye is first layer, right eye is second layer)
    kTypeImage,            //!< EGL Image texture
    kTypeEquiRectTexture,  //!< Equirectangular texture
    kTypeEquiRectImage,    //!< Equirectangular Image texture
    kTypeCubemapTexture,   //!< Cubemap texture (Not supporting cubemap image)
    kTypeVulkan,           //!< Vulkan texture
} yvrTextureType;

/// Flags used in yvrSubmitLayer.layerFlags
typedef enum yvrLayerFlags_
{
    kLayerFlagNone = 0x00000000,
    kLayerFlagHeadLocked = 0x00000001,
    kLayerFlagOpaque = 0x00000002,
    kLayerFlagSubsampled = 0x00000004,
    kLayerFlagPremultipliedAlpha = 0x00000008,
    kLayerFlagBlendTextureSourceAlpha = 0x00000010,
    kLayerFlagFlipY = 0x00000020,
    kLayerFlagDepthStencil = 0x00000040,
} yvrLayerFlags;

/// Flags used in SubmitFrameDescriptionInternal.frameOptions
typedef enum yvrFrameOptions_
{
    kDisableDistortionCorrection = (1 << 0),  //!< Disables the lens distortion correction (useful for debugging)
    kDisableReprojection = (1 << 1),          //!< Disables re-projection
    EnableMotionToPhoton = (1 << 2),          //!< Enables motion to photon testing
    kDisableChromaticCorrection = (1 << 3)    //!< Disables the lens chromatic aberration correction (performance optimization)
} yvrFrameOptions;

/// Possible results of initialization.
typedef enum yvrInitializeStatus_
{
    YVRLIB_INITIALIZE_SUCCESS = 0,
    YVRLIB_INITIALIZE_UNKNOWN_ERROR = -1,
    YVRLIB_INITIALIZE_PERMISSIONS_ERROR = -2,
    YVRLIB_INITIALIZE_ALREADY_INITIALIZED = -3,
    YVRLIB_INITIALIZE_INTERNAL_ERROR = -4,
    YVRLIB_INITIALIZE_DEVICE_NOT_SUPPORTED = -5,
    YVRLIB_INITIALIZE_PARAMETERS_ERROR = -6,
} yvrInitializeStatus;

//! \brief Initialization parameters that are constant over the life-cycle of the application
//! \sa yvrInitialize
typedef struct yvrInitParams_
{
    yvrJava Java;
} yvrInitParams;

//! \brief Structure containing parameters needed to enter VR mode
//! \sa yvrlib_EnterVrMode, yvrOptionFlags
typedef struct yvrBeginParams_
{
    ANativeWindow* nativeWindow;
    yvrOptionFlags optionFlags;       //!< Flags to specify YVR options (yvrOptionFlags)
    yvrColorSpace colorSpace;  //!< Color space to utilize for the display surface
} yvrBeginParams;

typedef enum yvrButton_
{
    kButtonA = 0x00000001,
    kButtonB = 0x00000002,
    kButtonX = 0x00000004,
    kButtonY = 0x00000008,
    kButtonStart = 0x00000010,
    kButtonReserved = 0x00000020,
    kButtonLIndexTrigger = 0x00000040,
    kButtonRIndexTrigger = 0x00000080,
    kButtonLHandTrigger = 0x00000100,
    kButtonRHandTrigger = 0x00000200,
    kButtonLThumbstick = 0x00000400,
    kButtonRThumbstick = 0x00000800,
    kButtonLThumbstickUp = 0x00001000,
    kButtonLThumbstickDown = 0x00002000,
    kButtonLThumbstickLeft = 0x00004000,
    kButtonLThumbstickRight = 0x00008000,
    kButtonRThumbstickUp = 0x00010000,
    kButtonRThumbstickDown = 0x00020000,
    kButtonRThumbstickLeft = 0x00040000,
    kButtonRThumbstickRight = 0x00808000,
    yvrButton_EnumSize
} yvrButton;

typedef struct yvrControllerState_
{
    uint32_t Buttons;
    uint32_t Touches;
    float IndexTrigger;
    float Thumbstick[2];
    uint8_t BatteryPercentRemaining;
    uint8_t is_charging;
    YVRLIB_PADDING(2)
} __packed yvrControllerState;
YVRLIB_ASSERT_TYPE_SIZE(yvrControllerState, 24);

typedef struct yvrControllerConnectedInfo_
{
    bool left_connected;
    bool right_connected;
    YVRLIB_PADDING(6)
} __packed yvrControllerConnectedInfo;
YVRLIB_ASSERT_TYPE_SIZE(yvrControllerConnectedInfo, 8);

typedef struct yvrEyeFov_
{
    float UpFov;
    float DownFov;
    float LeftFov;
    float RightFov;
} yvrEyeFov;

typedef struct yvrTextureSwapChain yvrTextureSwapChain;

typedef enum yvrSwapChainCreateFlags_
{

} yvrSwapChainCreateFlags;

typedef enum yvrSwapChainUsageFlags_
{

} yvrSwapChainUsageFlags;

typedef struct yvrSwapChainCreateInfo_
{
    int64_t Format;
    int Width;
    int Height;
    int Levels;
    int FaceCount;
    int ArraySize;
    int BufferCount;
    uint64_t CreateFlags;
    uint64_t UsageFlags;
} yvrSwapChainCreateInfo;
YVRLIB_ASSERT_TYPE_SIZE_32_BIT(yvrSwapChainCreateInfo, 48);
YVRLIB_ASSERT_TYPE_SIZE_64_BIT(yvrSwapChainCreateInfo, 48);

typedef struct yvrSubmitLayer_
{
    int SwapChainIndex;
    yvrTextureType imageType;
    yvrVector4f VertexCoord[4];
    yvrVector2f TextureCoord[4];
    yvrMatrix4f TexCoordsMatrix;
    yvrEyeMask eyeMask;
    uint32_t layerFlags;
    yvrTextureSwapChain* ColorSwapChain;
} yvrSubmitLayer;

typedef struct yvrLayerProjection_
{
    yvrSubmitLayer mTextures[kNumEyes];
} yvrLayerProjection;

typedef struct yvrSubmitFrameDescription_
{
    uint64_t FrameIndex;
    int32_t minVsyncs;
    uint32_t LayerCount;
    uint32_t frameOptions;
    yvrPoseState HeadPoseState;
    yvrLayerProjection** Layers;
} yvrSubmitFrameDescription;

typedef struct yvrSubmitLayer2_
{
    int32_t SwapChainIndex;
    yvrTextureType imageType;
    yvrVector4f VertexCoord[4];
    yvrVector2f TextureCoord[4];
    yvrMatrix4f TexCoordsMatrix;
    yvrEyeMask eyeMask;
    uint32_t layerFlags;
    yvrTextureSwapChain* ColorSwapChain;
    yvrTextureSwapChain* DepthSwapChain;
    yvrVector4f ColorScale;
    yvrVector4f ColorOffset;
    uint8_t reserved[8];
} yvrSubmitLayer2;
YVRLIB_ASSERT_TYPE_SIZE_64_BIT(yvrSubmitLayer2, 232);
YVRLIB_ASSERT_TYPE_SIZE_32_BIT(yvrSubmitLayer2, 224);

typedef struct yvrLayerProjection2_
{
    yvrSubmitLayer2 mTextures[kNumEyes];
} yvrLayerProjection2;

typedef struct yvrSubmitFrameDescription2_
{
    uint64_t FrameIndex;
    int32_t minVsyncs;
    uint32_t LayerCount;
    uint32_t frameOptions;
    yvrPoseState HeadPoseState;
    yvrLayerProjection2** Layers;
    uint8_t reserved[8];
} yvrSubmitFrameDescription2;
YVRLIB_ASSERT_TYPE_SIZE(yvrSubmitFrameDescription2, 144);

typedef enum yvrTrackingSpace_
{
    YVRLIB_TRACKING_SPACE_LOCAL = 0,            // Eye level origin - controlled by system recentering
    YVRLIB_TRACKING_SPACE_LOCAL_FLOOR = 1,      // Floor level origin - controlled by system recentering
    YVRLIB_TRACKING_SPACE_LOCAL_TILTED = 2,     // Tilted pose for "bed mode" - controlled by system recentering
    YVRLIB_TRACKING_SPACE_STAGE = 3,            // Floor level origin - controlled by Guardian setup
    YVRLIB_TRACKING_SPACE_LOCAL_FIXED_YAW = 4,  // Position of local space, but yaw stays constant
} yvrTrackingSpace;

typedef struct BoundaryTestResult_
{
    bool IsTriggering;
    float ClosestDistance;
    yvrVector3f ClosestPoint;
    yvrVector3f ClosestPointNormal;
} BoundaryTestResult;

typedef struct BoundaryPoint_
{
    yvrVector3f point;
} BoundaryPoint;

typedef struct BoundaryDimensions_
{
    yvrVector3f dimensions;
} BoundaryDimensions;

typedef struct yvrFoveation_
{
    yvrVector2f gain;  //!< Foveation Gain Rate [1, ...]
    float area;        //!< Foveation Area Size [0, ...]
    float minimum;     //!< Foveation Minimum Resolution [1, 1/2, 1/4, ..., 1/16, 0]
} yvrFoveation;

typedef enum yvrExtraLatencyMode_
{
    YVRLIB_EXTRA_LATENCY_MODE_OFF = 0,
    YVRLIB_EXTRA_LATENCY_MODE_ON = 1,
    YVRLIB_EXTRA_LATENCY_MODE_DYNAMIC = 2
} yvrExtraLatencyMode;

typedef int16_t yvrVertexIndex;
typedef int16_t yvrHandBoneIndex;

typedef enum HandTrackingStatus_
{
    HandTrackingStatus_Untracked = 0,  // not tracked
    HandTrackingStatus_Tracked = 1,    // tracked
    HandTrackingStatus_EnumSize = 0x7fffffff
} HandTrackingStatus;

typedef enum Confidence_
{
    Confidence_LOW = 0x00000000,
    Confidence_HIGH = 0x3f800000
} Confidence;

typedef enum HandFingers_
{
    HandFinger_Thumb = 0,
    HandFinger_Index = 1,
    HandFinger_Middle = 2,
    HandFinger_Ring = 3,
    HandFinger_Pinky = 4,
    HandFinger_Max,
    HandFinger_EnumSize = 0x7fffffff
} HandFingers;

typedef enum HandPinchStrength_
{
    HandPinchStrength_Index = 0,   // hand is in the index finger pinch state
    HandPinchStrength_Middle = 1,  // hand is in the middle finger pinch state
    HandPinchStrength_Ring = 2,    // hand is in the ring finger pinch state
    HandPinchStrength_Pinky = 3,   // hand is in the pinky finger pinch state
    HandPinchStrength_Max = 4,
    HandPinchStrength_EnumSize = 0x7fffffff
} HandPinchStrength;

/// Specifies left or right handedness.
typedef enum Handedness_
{
    HAND_UNKNOWN = 0,
    HAND_LEFT = 1,
    HAND_RIGHT = 2
} Handedness;

typedef enum InputStateHandStatus_
{
    InputStateHandStatus_PointerValid = (1 << 1),             // if this is set the PointerPose and PinchStrength contain valid data, otherwise
                                                              // they should not be used.
    InputStateHandStatus_IndexPinching = (1 << 2),            // if this is set the pinch gesture for that finger is on
    InputStateHandStatus_MiddlePinching = (1 << 3),           // if this is set the pinch gesture for that finger is on
    InputStateHandStatus_RingPinching = (1 << 4),             // if this is set the pinch gesture for that finger is on
    InputStateHandStatus_PinkyPinching = (1 << 5),            // if this is set the pinch gesture for that finger is on
    InputStateHandStatus_SystemGestureProcessing = (1 << 6),  // if this is set the hand is currently processing a system gesture
    InputStateHandStatus_DominantHand = (1 << 7),             // if this is set the hand is considered the dominant hand
    InputStateHandStatus_MenuPressed = (1 << 8),              // if this is set the hand performed the system gesture as the non-dominant hand
    InputStateHandStatus_EnumSize = 0x7fffffff
} InputStateHandStatus;

typedef enum HandBone_
{
    HandBone_Invalid = -1,
    HandBone_WristRoot = 0,    // root frame of the hand, where the wrist is located
    HandBone_ForearmStub = 1,  // frame for user's forearm
    HandBone_Thumb0 = 2,       // thumb trapezium bone
    HandBone_Thumb1 = 3,       // thumb metacarpal bone
    HandBone_Thumb2 = 4,       // thumb proximal phalange bone
    HandBone_Thumb3 = 5,       // thumb distal phalange bone
    HandBone_Index1 = 6,       // index proximal phalange bone
    HandBone_Index2 = 7,       // index intermediate phalange bone
    HandBone_Index3 = 8,       // index distal phalange bone
    HandBone_Middle1 = 9,      // middle proximal phalange bone
    HandBone_Middle2 = 10,     // middle intermediate phalange bone
    HandBone_Middle3 = 11,     // middle distal phalange bone
    HandBone_Ring1 = 12,       // ring proximal phalange bone
    HandBone_Ring2 = 13,       // ring intermediate phalange bone
    HandBone_Ring3 = 14,       // ring distal phalange bone
    HandBone_Pinky0 = 15,      // pinky metacarpal bone
    HandBone_Pinky1 = 16,      // pinky proximal phalange bone
    HandBone_Pinky2 = 17,      // pinky intermediate phalange bone
    HandBone_Pinky3 = 18,      // pinky distal phalange bone
    HandBone_MaxSkinnable = 19,

    // Bone tips are position only. They are not used for skinning but useful for hit-testing.
    // NOTE: HandBone_ThumbTip == HandBone_MaxSkinnable since the extended tips need to be
    // contiguous
    HandBone_ThumbTip = HandBone_MaxSkinnable + 0,   // tip of the thumb
    HandBone_IndexTip = HandBone_MaxSkinnable + 1,   // tip of the index finger
    HandBone_MiddleTip = HandBone_MaxSkinnable + 2,  // tip of the middle finger
    HandBone_RingTip = HandBone_MaxSkinnable + 3,    // tip of the ring finger
    HandBone_PinkyTip = HandBone_MaxSkinnable + 4,   // tip of the pinky
    HandBone_Max = HandBone_MaxSkinnable + 5,
    HandBone_EnumSize = 0x7fff
} HandBone;

typedef enum HandConstants_
{
    Hand_MaxVertices = 3000,
    Hand_MaxIndices = Hand_MaxVertices * 6,
    Hand_MaxFingers = HandFinger_Max,
    Hand_MaxPinchStrengths = HandPinchStrength_Max,
    Hand_MaxSkinnableBones = HandBone_MaxSkinnable,
    Hand_MaxBones = HandBone_Max,
    Hand_MaxCapsules = 19,
    Hand_EnumSize = 0x7fffffff
} HandConstants;

/// Unified version struct
typedef enum HandVersion_
{
    HandVersion_1 = 0xdf000001,  /// Current

    HandVersion_EnumSize = 0x7fffffff
} HandVersion;

// Header for all hand pose structures.
typedef struct HandPoseHeader_
{
    // The version number of the Pose structure.
    // When requesting a pose with GetHandPose this MUST be set to the proper version.
    // If this is not set to a known version, or if the version it is set to is no longer
    // supported for the current SDK, _GetHand* functions will return Error_InvalidParameter.
    HandVersion Version;

    /// Reserved for later use
    double Reserved;
} HandPoseHeader;

// Pass this structure to GetHandPose() to get the pose of the hand at a particular time.
typedef struct HandPose_
{
    // Status of tracking for this pose. This is not a bit field, but an exclusive state.
    HandTrackingStatus Status;

    // Root pose of the hand in world space. Not to be confused with the root bone's transform.
    // The root bone can still be offset from this by the skeleton's rest pose.
    yvrPosef RootPose;

    // Current rotation of each bone.
    yvrQuatf BoneRotations[HandBone_Max];

    // Time stamp for the pose that was requested in global system time.
    double RequestedTimeStamp;

    // Time stamp of the captured sample that the pose was extrapolated from.
    double SampleTimeStamp;

    // Tracking confidence.
    // This is the amount of confidence that the system has that the entire hand pose is correct.
    Confidence HandConfidence;

    // Scale of the hand relative to the original hand model. This value may change at any time
    // based on the size of the hand being tracked. The default is 1.0.
    float HandScale;

    // Per-finger tracking confidence.
    // This is the amount of confidence the system has that the individual finger poses are correct.
    Confidence FingerConfidences[HandFinger_Max];
} HandPose;

typedef struct HandPoses_
{
    HandPoseHeader Header;
    HandPose LeftHandPose;
    HandPose RightHandPose;
} HandPoses;

// Header for all mesh structures.
typedef struct HandMeshHeader_
{
    // The version number of the mesh structure.
    HandVersion Version;
} HandMeshHeader;

typedef struct HandMesh_V1_
{
    // All mesh structures will start with this header and the version.
    HandMeshHeader Header;

    // Number of unique vertices in the mesh.
    uint32_t NumVertices;
    // Number of unique indices in the mesh.
    uint32_t NumIndices;

    // Reserved for future use
    uint32_t Reserved[13];

    // An array of count NumVertices positions for each vertex.
    yvrVector3f VertexPositions[Hand_MaxVertices];
    // An array of count NumIndices of vertex indices specifying triangles that make up the mesh.
    yvrVertexIndex Indices[Hand_MaxIndices];
    // An array of count NumVertices of normals for each vertex.
    yvrVector3f VertexNormals[Hand_MaxVertices];
    // An array of count NumVertices of texture coordinates for each vertex.
    yvrVector2f VertexUV0[Hand_MaxVertices];
    // An array of count NumVertices of blend indices for each of the bones that each vertex is
    // weighted to. Always valid. An index of < 0 means no blend weight.
    yvrVector4s BlendIndices[Hand_MaxVertices];
    // An array of count NumVertices of weights for each of the bones affecting each vertex.
    yvrVector4f BlendWeights[Hand_MaxVertices];
} HandMesh;

// BoneCapsule
//    _---_
//  -"     "-
// /         \
// |----A----|
// |    |    |
// |    |    |
// |    |-r->|
// |    |    |
// |    |    |
// |----B----|
// \         /
//  -.     .-
//    '---'
typedef struct BoneCapsule_
{
    // Index of the bone this capsule is on.
    yvrHandBoneIndex BoneIndex;
    // Points at either end of the cylinder inscribed in the capsule. Also the center points for
    // spheres at either end of the capsule. Points A and B in the diagram above.
    yvrVector3f Points[2];
    // The radius of the capsule cylinder and of the half-sphere caps on the ends of the capsule.
    float Radius;
} BoneCapsule;

// Header for all mesh structures.
typedef struct HandSkeletonHeader_
{
    // The version number of the skeleton structure.
    HandVersion Version;
} HandSkeletonHeader;

typedef struct HandSkeleton_V1_
{
    // Version of the mesh structure.
    HandSkeletonHeader Header;

    // The number of bones in this skeleton.
    uint32_t NumBones;

    // The number of capsules on this skeleton.
    uint32_t NumCapsules;

    // reserved for future use
    uint32_t Reserved[5];

    // An array of count NumBones transforms for each bone in local (parent) space.
    yvrPosef BonePoses[Hand_MaxBones];

    // An array of count NumBones indicating the parent bone index for each bone.
    yvrHandBoneIndex BoneParentIndices[Hand_MaxBones];

    // An array of count NumCapsules HandCapsules. Note that the number of capsules
    BoneCapsule Capsules[Hand_MaxCapsules];
    // is not necessarily the same as the number of bones.
} HandSkeleton;

/// This header starts all InputState structures. It should only hold fields
/// that are common to all input controllers.
typedef struct InputStateHeader_
{
    /// System time when the controller state was last updated.
    double TimeInSeconds;
} InputStateHeader;

// Pass this structure to GetCurrentInputState() with a device id for a hand to get the
// current, second-order state of the hand.
typedef struct InputStateHand_
{
    InputStateHeader Header;

    // For each pinch type, indicates how far the fingers are into that pinch state. Range 0.0
    // to 1.0, where 1.0 is fully pinching. Indexable via the HandPinchStrength enums.
    float PinchStrength[HandPinchStrength_Max];

    // World space position and orientation of the pointer attached to the hand. This describes
    // a pointing ray useful for UI interactions.
    // Note that the pointer pose is not valid unless the InputStateHandStatus_PointerValid flag
    // is set in the InputStateStatus field.
    yvrPosef PointerPose;

    // Status flags for this hand's input state. Mask of InputStateHandStatus flags.
    uint32_t InputStateStatus;
} InputStateHand;

typedef void (*y_notify_event_fn)(yvrEventHeader *evt);

#endif
