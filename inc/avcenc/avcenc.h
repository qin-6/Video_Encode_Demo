#ifndef AVCENC_H
#define AVCENC_H

#ifdef __cplusplus
extern "C" {
#endif


typedef struct _VA_ENC_INIT_PARAMS
{
	void *device;
	unsigned int width;
	unsigned int height;
	unsigned int bitrate;	
	unsigned int qp;
	unsigned int gop_length;
	unsigned int intra_frame_period;
}VA_ENC_INIT_PARAMS;

typedef struct _VA_ENC_PIC_PARAMS
{
	unsigned long long frameNumber;
	struct
	{
		unsigned char *y;
		unsigned char *u;
		unsigned char *v;
	}frameData;
	unsigned int textureId;
} VA_ENC_PIC_PARAMS;

typedef struct _VA_ENC_BITSTREAM
{
	unsigned char *buf;
	unsigned int size;
}VA_ENC_BITSTREAM;

typedef void* VA_ENC_INPUT_FRAME;

typedef void (*PVAENCINITIALIZEENCODER)(VA_ENC_INIT_PARAMS *initParams);
typedef void (*PVAENCENCODERPICTURE)(VA_ENC_PIC_PARAMS *picParams);
typedef bool (*PVAENCINPUTFRAME)(VA_ENC_INPUT_FRAME frame);
typedef bool (*PVAENCLOCKBITSREAM)(VA_ENC_BITSTREAM *bitStream);
typedef bool (*PVAENCUNLOCKBITSTREAM)();
typedef bool (*PVAENCDESTROYENCODER)();


typedef struct _VA_ENC_API
{
	PVAENCINITIALIZEENCODER 	VaEncInitializeEncoder;
	PVAENCENCODERPICTURE 		VaEncEncodePicture;
	PVAENCLOCKBITSREAM			VaEncLockBitstream;
	PVAENCUNLOCKBITSTREAM	    VaEncUnLockBitstream;
	PVAENCDESTROYENCODER		VaEncDestroyEncoder;
	PVAENCINPUTFRAME			VaEncInputFrame;
}VA_ENC_API;

#define VaEncApiInstance(module, vaApi) 																	\
do{ 																										\	
	(vaApi).VaEncInitializeEncoder = (PVAENCINITIALIZEENCODER)dlsym(module, "VaEncInitializeEncoder"); 		\
	(vaApi).VaEncEncodePicture = (PVAENCENCODERPICTURE)dlsym(module, "VaEncEncodePicture");			 		\
	(vaApi).VaEncLockBitstream = (PVAENCLOCKBITSREAM)dlsym(module, "VaEncLockBitstream");					\
	(vaApi).VaEncUnLockBitstream = (PVAENCUNLOCKBITSTREAM)dlsym(module, "VaEncUnLockBitstream");		 	\
	(vaApi).VaEncInputFrame = (PVAENCINPUTFRAME)dlsym(module, "VaEncInputFrame");							\
}while(0)

void VaEncInitializeEncoder(VA_ENC_INIT_PARAMS *initParams);

void VaEncEncodePicture(VA_ENC_PIC_PARAMS * picParams);

void VaEncInputFrame(VA_ENC_INPUT_FRAME * frame);

bool VaEncLockBitstream(VA_ENC_BITSTREAM * bitStream);

bool VaEncUnLockBitstream();

bool VaEncDestroyEncoder();

#ifdef __cplusplus
}
#endif

#endif
