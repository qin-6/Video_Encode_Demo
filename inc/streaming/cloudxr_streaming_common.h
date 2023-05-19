#ifndef _CLOUDXR_STREAMING_COMMON_
#define _CLOUDXR_STREAMING_COMMON_

#include <stdint.h>


#ifdef __cplusplus
extern "C"
{ // extern "C"
#endif

typedef enum
{
    scxrResult_Success = 0,
    scxrResult_Failed = 1,
    scxrResult_ClientHandle_Null = 2,
    scxrResult_Frame_Has_No_Cpu_Buffer = 3,
    scxrResult_Frame_Released = 4,
    scxrResult_Not_Initialized = 5,
    scxrResult_Max
} scxrResult;

typedef enum
{
    scxrDebugFlag_LogVerbose                = 0x00000001, ///< record very verbose output
    scxrDebugFlag_LogQuiet                  = 0x00000002, ///< do not open normal log files at all
    scxrDebugFlag_LogPrivacyDisabled        = 0x00000010, ///< disable any privacy filtering in output
    scxrDebugFlag_TraceLocalEvents          = 0x00000100, ///< record trace of local event timing
    scxrDebugFlag_TraceStreamEvents         = 0x00000200, ///< record trace of c/s stream event timing
    scxrDebugFlag_TraceQosStats             = 0x00000400, ///< record QoS statistics
    scxrDebugFlag_DumpImages                = 0x00001000, ///< Dump images from stream
    scxrDebugFlag_DumpAudio                 = 0x00002000, ///< Dump audio from stream
    scxrDebugFlag_EmbedServerInfo           = 0x00004000, ///< server embeds stats into frame buffers for streaming
    scxrDebugFlag_EmbedClientInfo           = 0x00008000, ///< client embeds stats into frame buffers or overlays
    scxrDebugFlag_CaptureServerBitstream    = 0x00010000, ///< record the server-sent video bitstream
    scxrDebugFlag_CaptureClientBitstream    = 0x00020000, ///< record the client-received video bitstream
    scxrDebugFlag_FallbackDecoder           = 0x00100000, ///< try to use a fallback decoder for platform
    scxrDebugFlag_EnableSXRDecoder          = 0x00200000, ///< `[ANDROID only]` enable the experimental SXR decoder
    scxrDebugFlag_EnableImageReaderDecoder  = 0x00400000, ///< `[ANDROID only]` enable the experimental ImageReader decoder
} scxrDebugFlag;


typedef enum
{
    scxrGraphicsContext_D3D11,
    scxrGraphicsContext_D3D12,
    scxrGraphicsContext_GL,
    scxrGraphicsContext_GLES,
    scxrGraphicsContext_Vulkan,
    scxrGraphicsContext_Metal,
    scxrGraphicsContext_Cuda,
    scxrGraphicsContext_NvMedia,
    // force to a qword
    scxr_GraphicsContext_Max = ~0,
} scxrGraphicsContextType;

typedef struct scxrGraphicsContext
{
    scxrGraphicsContextType type;

    union
    {
#if defined(__linux__)
        struct
        {
            void* display;
            void* context;
            void* surface;
        } egl;
#endif

#if defined(_WIN32)
        void* wglContext;

        struct ID3D11DeviceContext* d3d11Context;

        struct
        {
            struct ID3D12Device* device;
            struct ID3D12CommandQueue* queue;
        } d3d12;

#endif

        struct
        {
            int device;
            void* ctx;
        } cuda;

#if defined(WITH_VULKAN)
        struct
        {
            void* instance;
            void* physicalDevice;
            void* device;
            void* queue;
        } vk;
#endif

#if defined(__APPLE__) && defined(WITH_METAL)
        struct
        {
            id<MTLDevice> device;
            id<MTLCommandQueue> queue;
        } metal;
#endif

        uint8_t _[32];
    };
} scxrGraphicsContext;

typedef enum
{
    scxrDeviceType_Android = 0,
    scxrDeviceType_Ios = 1
} scxrDeviceType;


typedef enum
{
    scxrTrackingResult_Uninitialized          = 1,

    scxrTrackingResult_Calibrating_InProgress = 100,
    scxrTrackingResult_Calibrating_OutOfRange = 101,

    scxrTrackingResult_Running_OK             = 200,
    scxrTrackingResult_Running_OutOfRange     = 201,

    scxrTrackingResult_Fallback_RotationOnly  = 300,
} scxrTrackingResult;

typedef enum
{
    scxrButton_System,
    scxrButton_ApplicationMenu,

    scxrButton_Grip_Touch,
    scxrButton_Grip_Click,

    scxrButton_Trigger_Touch,
    scxrButton_Trigger_Click,

    scxrButton_Touchpad_Touch,
    scxrButton_Touchpad_Click,

    scxrButton_Joystick_Touch,
    scxrButton_Joystick_Click,

    scxrButton_A,
    scxrButton_B,

    scxrButton_X = scxrButton_A,
    scxrButton_Y = scxrButton_B,

    scxrButton_Num
} scxrButtonId;

typedef enum
{
    scxrAnalog_Trigger,

    scxrAnalog_TouchpadX,
    scxrAnalog_TouchpadY,

    scxrAnalog_JoystickX,
    scxrAnalog_JoystickY,

    scxrAnalog_Grip,
    scxrAnalog_Grip_Force,

    scxrAnalog_Num
} scxrAnalogId;

typedef enum
{
    scxrController_Left  = 0,
    scxrController_Right = 1,
} scxrControllerId;

typedef enum
{
    scxrUniverseOrigin_Seated,
    scxrUniverseOrigin_Standing
} scxrUniverseOrigin;

typedef enum
{
    scxrDeliveryType_Mono_RGB,
    scxrDeliveryType_Mono_RGBA,
    scxrDeliveryType_Stereo_RGB
} scxrDeliveryType;

typedef enum
{
    scxrControllerType_HtcVive,
    scxrControllerType_ValveIndex,
    scxrControllerType_OculusTouch,
    scxrControllerType_None
} scxrControllerType;

typedef enum {
    scxrDeviceModel_Mono_AR,    // frame: rgb + alpha + id
    scxrDeviceModel_Mono_VR,    // reversed...
    scxrDeviceModel_Stereo_AR,  // frame: rgb + rgb + id (to be verified)
    scxrDeviceModel_Stereo_VR,  // frame: rgb + rgb + id
}scxrDeviceModel;

typedef enum {
    scxrRenderMethod_OVR = 0,
    scxrRenderMethod_NVCLOUDXR,
    scxrRenderMethod_Hook
} scxrRenderMethod;

/// Device Active/Idle State
typedef enum
{
    scxrDeviceActivityLevel_Unknown = -1,                ///< Unknown state.
    scxrDeviceActivityLevel_Idle = 0,                    ///< No activity for the last 10 seconds
    scxrDeviceActivityLevel_UserInteraction = 1,         ///< Activity is happening now (movement or prox sensor)
    scxrDeviceActivityLevel_UserInteraction_Timeout = 2, ///< No activity for the last 0.5 seconds
    scxrDeviceActivityLevel_Standby = 3,                 ///< Idle for at least 5 seconds (configurable in Settings : Power Management)
} scxrDeviceActivityLevel;

typedef enum
{
    scxrHmdTrackingFlags_HasProjection = 0x00000001, ///< Tracking state has updated projection parameters.
    scxrHmdTrackingFlags_HasIPD        = 0x00000002, ///< Tracking state has updated IPD.
    scxrHmdTrackingFlags_HasRefresh    = 0x00000004, ///< Tracking state has updated refresh rate.
    scxrHmdTrackingFlags_HasPoseID     = 0x00000008, ///< Tracking state contains a 64-bit pose ID.
} scxrHmdTrackingFlags;

typedef struct scxrMatrix34
{
    float m[3][4];
} scxrMatrix34;

typedef struct scxrVector2
{
    float v[2];
} scxrVector2;

typedef struct scxrVector3
{
    float v[3];
} scxrVector3;

typedef struct scxrQuaternion
{
    float x;
    float y;
    float z;
    float w;
} scxrQuaternion;

typedef struct scxrChaperone
{
    scxrUniverseOrigin universe;
    scxrMatrix34 origin;
    scxrVector2 playArea;
} scxrChaperone;

const int SCXR_NUM_CONTROLLERS = 2;

typedef struct scxrTrackedDevicePose
{
    uint32_t id;
    scxrMatrix34 deviceToAbsoluteTracking;
    scxrTrackingResult trackingResult;
    scxrVector3 velocity;
    scxrVector3 angularVelocity;
    bool poseIsValid;
    bool deviceIsConnected;

} scxrTrackedDevicePose;

typedef struct scxrHmdTrackingState
{
    uint64_t flags;
    float proj[2][4];
    float ipd;
    float displayRefresh;
    scxrTrackedDevicePose pose;
    scxrDeviceActivityLevel activityLevel;
} scxrHmdTrackingState;

typedef struct scxrHandTrackingState
{
    scxrQuaternion boneRotations[19];
    scxrVector3 bonePositionsBase[19];
    scxrQuaternion boneRootOrientation;
    scxrVector3 boneRootPosition;
    unsigned int handFingerConfidences;
} scxrHandTrackingState;

typedef struct scxrControllerTrackingState
{
    scxrTrackedDevicePose pose;
    bool isHand;
    scxrHandTrackingState hand;
    uint32_t booleanComps;
    uint32_t booleanCompsChanged;
    float scalarComps[scxrAnalog_Num];
} scxrControllerTrackingState;

typedef struct scxrXRTrackingState
{
    scxrHmdTrackingState hmd;
    scxrControllerTrackingState controller[SCXR_NUM_CONTROLLERS];
    float poseTimeOffset;                 // Offset in seconds that affects the amount of pose extrapoation. Default is 0.
} scxrXRTrackingState;

typedef enum
{
    scxrConnectionQuality_Unstable = 0,   ///< Initial value while estimating quality. Thereafter, expect disconnects. Details in @ref cxrConnectionQualityReasons.
    scxrConnectionQuality_Bad = 1,        ///< Expect very low bitrate and/or high latency. Details in @ref cxrConnectionQualityReasons.
    scxrConnectionQuality_Poor = 2,       ///< Expect low bitrate and/or high latency. Details in @ref cxrConnectionQualityReasons.
    scxrConnectionQuality_Fair = 3,       ///< Expect frequent impact on bitrate or latency. Details in @ref cxrConnectionQualityReasons.
    scxrConnectionQuality_Good = 4,       ///< Expect ocassional impacts on bitrate or latency
    scxrConnectionQuality_Excellent = 5   ///< Expect infrequent impacts on bitrate or latency
} scxrConnectionQuality;

typedef enum
{
    scxrConnectionQualityReason_EstimatingQuality = 0x0,   ///< Inital value while estimating quality.
    scxrConnectionQualityReason_LowBandwidth = 0x1,        ///< The percentage of unutilized bandwidth is too low to maintain bitrate
    scxrConnectionQualityReason_HighLatency = 0x2,         ///< The round trip time is too high to maintain low latency
    scxrConnectionQualityReason_HighPacketLoss = 0x4       ///< The packet loss is too high to overcome without re-transmission
} scxrConnectionQualityReason;

typedef struct scxrProfileHandle
{
    uint32_t RTT;
    uint32_t encodeFps;	
    uint32_t decodeFps;	
    uint32_t nackCount;
    uint32_t firCount;
    uint32_t pliCount;	
    uint32_t packetLossRate;
    uint32_t bitRate;
    uint32_t bandwidthAvailableKbps;
    uint32_t jitterBufferDelay;
    uint32_t avgEncodeTime;
    uint32_t maxEncodeTime;
    uint32_t avgDecodeTime;
    uint32_t maxDecodeTime;
    uint32_t qpSum;
    uint32_t bitRateSend;
    uint32_t frameDelay;
    uint32_t recIdle;
    uint32_t sendDelay;

    scxrConnectionQuality quality;
    uint32_t qualityReasons;
} scxrProfileHandle;

const uint32_t scxrFrameMask_Left  = 0x01; ///< Maps to index/stream 0 for stereo modes
const uint32_t scxrFrameMask_Right = 0x02; ///< Maps to index/stream 1 for stereo modes
const uint32_t scxrFrameMask_Special = 0x80000000; ///< Special-case masks that aren't direct indices are noted by setting this bit.
const uint32_t scxrFrameMask_All   = 0xFFFFFFFF; ///< Requests the system map to all available frames

typedef void* scxrVideoFrame;

typedef struct scxrHapticFeedback
{
    int   controllerIdx;
    float amplitude;
    float seconds;
    float frequency;
} scxrHapticFeedback;

typedef struct scxrAudioFrame
{
    int16_t* streamBuffer;
    uint32_t streamSizeBytes;
    int sampleRate;
    int channelCount;
} scxrAudioFrame;


#ifdef __cplusplus
} // extern "C"
#endif

#endif