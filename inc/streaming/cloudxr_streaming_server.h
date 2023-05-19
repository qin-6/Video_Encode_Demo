#ifndef _CLOUDXR_STREAMING_SERVER_
#define _CLOUDXR_STREAMING_SERVER_

#include <functional>
#include <string>
#include "cloudxr_streaming_common.h"
#include "cloudxr_input_events.h"

#ifdef SCloudXRServerLib_EXPORTS
#define SCXR_SERVER_API __declspec(dllexport)
#else
#define SCXR_SERVER_API __declspec(dllimport)
#endif


enum scxrGaphicDeviceType {
    scxrGaphicDeviceType_D3D11 = 0,
	scxrGraphicDeviceType_OpenGL = 1
};

enum scxrServerConnectionState {
    scxrServerConnectionState_ReadyToConnect = 0,                  ///< ��ʼ��״̬
    scxrServerConnectionState_ConnectionAttemptInProgress = 1,     ///< �������ӷ�����
    scxrServerConnectionState_ConnectionAttemptFailed = 2,         ///< ���ӷ�����ʧ�ܣ������������������ʧ�ܣ�ice���Ӵ����
    scxrServerConnectionState_StreamingSessionInProgress = 3,      ///< ���ӳɹ�����ʼstreaming�����״̬����Կ�ʼ����pose�Լ�input����
    scxrServerConnectionState_Disconnected = 4,                    ///< �Ͽ�����
    scxrServerConnectionState_Exiting = 5                          ///< �ͻ��˽���
};

enum scxrServerSyncEvent {
    scxrServerSyncEvent_NewFrameReady = 0,
    scxrServerSyncEvent_WaitNewFrame,
    scxrServerSyncEvent_EncodeFinished,
    scxrServerSyncEvent_WaitEncoding,
    scxrServerSyncEvent_EncodeEntered,
    scxrServerSyncEvent_EncodeExited,
    scxrServerSyncEvent_WaitEncodeExiting
};

typedef struct scxrTextureData {
    int frameId;
    void* textureHandle;
} scxrTextureData;

typedef void* scxrGraphicDevice;
typedef struct scxrServer* scxrServerHandle;

typedef struct scxrServerCallback {
    void (*projectionCallback)(float* proj);
    void (*stateCallback)(scxrXRTrackingState state);
    void (*inputCallback)(scxrInputEvent input);
    void (*connectState)(scxrServerConnectionState state, const char *msg);
    void (*logout)(const char* msg);
    void (*saveTexture)(void* texture, const char* name);
    void (*ipdCallback)(float ipd);
	void (*renderReconfig)(int res, int devModel, int fps);
} scxrServerCallbacks;

enum scxrTextureFormat {
    scxrTextureFormat_R8G8B8A8
};

typedef struct scxrRenderEngineParam {
    scxrGaphicDeviceType graphicDeviceType;
    scxrGraphicDevice graphicDevice;
    scxrTextureFormat textureFormat;
} scxrEngineParam;

typedef struct scxrEncodeParam {
    int width;
    int height;
    int frameRate;
    int bitRate;
    bool isAuto;
} scxrEncodeParam;

typedef struct scxrServiceParam
{
    uint32_t port;
    char*     ipAddr;
    char*     configPath;
    char*     profileConfigPath;
}scxrServiceParam;

typedef struct scxrServerDesc {
    uint32_t version;
    scxrEncodeParam encodeParam;
    scxrServiceParam serviceParam;
    scxrRenderEngineParam engineParam;
    scxrServerCallbacks callbacks;
    scxrDeviceModel deviceModel;
    scxrRenderMethod renderMethod;
    bool audioEnable;
    bool enablePaas;
    bool enableLowLatencyMode;
} scxrServerDesc;

#ifdef _WIN32
extern "C" SCXR_SERVER_API scxrResult scxrServerInitialize(scxrServerHandle* serverHandler, scxrServerDesc* serverDesc);

extern "C" SCXR_SERVER_API scxrResult scxrServerConnect(scxrServerHandle serverHandle);

extern "C" SCXR_SERVER_API scxrResult scxrServerSendVideoFrame(scxrServerHandle serverHandler, scxrTextureData* textureHandle);

extern "C" SCXR_SERVER_API scxrResult scxrServerSendHaptics(scxrServerHandle serverHandler, scxrHapticFeedback* haptics);

extern "C" SCXR_SERVER_API scxrResult scxrServerSetRates(scxrServerHandle serverHandler, int bitRate, int frameRate);

extern "C" SCXR_SERVER_API scxrResult scxrServerSetRatesAuto(scxrServerHandle serverHandler, bool isAutoRate);

extern "C" SCXR_SERVER_API scxrResult scxrServerDispose(scxrServerHandle serverHandler);

extern "C" SCXR_SERVER_API scxrResult scxrServerUpdateResolution(scxrServerHandle serverHandler, uint32_t width, uint32_t height);

extern "C" SCXR_SERVER_API scxrResult scxrServerSyncWithEvent(scxrServerHandle serverHandler, scxrServerSyncEvent event);
#else
extern "C" scxrResult scxrServerInitialize(scxrServerHandle* serverHandler, scxrServerDesc* serverDesc);

extern "C" scxrResult scxrServerConnect(scxrServerHandle serverHandle);

extern "C" scxrResult scxrServerSendVideoFrame(scxrServerHandle serverHandler, scxrTextureData* textureHandle);

extern "C" scxrResult scxrServerSetRates(scxrServerHandle serverHandler, int bitRate, int frameRate);

extern "C" scxrResult scxrServerSetRatesAuto(scxrServerHandle serverHandler, bool isAutoRate);

extern "C" scxrResult scxrServerDispose(scxrServerHandle serverHandler);

extern "C" scxrResult scxrServerUpdateResolution(scxrServerHandle serverHandler, uint32_t width, uint32_t height);
#endif

#endif
