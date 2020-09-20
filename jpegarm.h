﻿/**
 * \usergroup{SceJpegArm}
 * \usage{psp2/jpegarm.h,SceJpegArm_stub,SCE_SYSMODULE_INTERNAL_JPEG_ARM}
 */

#ifndef _DOLCESDK_PSP2_JPEGARM_H_
#define _DOLCESDK_PSP2_JPEGARM_H_

#include <psp2/types.h>
#include <psp2/jpeg.h>

#define SCE_JPEG_ARM_OK									0

#define SCE_JPEG_ARM_ERROR_IMAGE_EMPTY					0x80650103
#define SCE_JPEG_ARM_ERROR_BAD_MARKER_LENGTH			0x80650104
#define SCE_JPEG_ARM_ERROR_BAD_DHT_COUNTS				0x80650105
#define SCE_JPEG_ARM_ERROR_BAD_DHT_INDEX				0x80650106
#define SCE_JPEG_ARM_ERROR_BAD_DQT_INDEX				0x80650107
#define SCE_JPEG_ARM_ERROR_DECODE_ERROR					0x80650109
#define SCE_JPEG_ARM_ERROR_INVALID_POINTER				0x80650110
#define SCE_JPEG_ARM_ERROR_BAD_COMPONENT_ID				0x80650111
#define SCE_JPEG_ARM_ERROR_UNSUPPORT_COLORSPACE			0x80650113
#define SCE_JPEG_ARM_ERROR_BAD_MCU_SIZE					0x80650114
#define SCE_JPEG_ARM_ERROR_BAD_PRECISION				0x80650115
#define SCE_JPEG_ARM_ERROR_UNSUPPORT_SAMPLING			0x80650116
#define SCE_JPEG_ARM_ERROR_COMPONENT_COUNT				0x80650117
#define SCE_JPEG_ARM_ERROR_EOI_EXPECTED					0x80650119
#define SCE_JPEG_ARM_ERROR_UNSUPPORT_IMAGE_SIZE			0x80650120
#define SCE_JPEG_ARM_ERROR_NO_HUFF_TABLE				0x80650121
#define SCE_JPEG_ARM_ERROR_NO_QUANT_TABLE				0x80650122
#define SCE_JPEG_ARM_ERROR_NO_SOI						0x80650123
#define SCE_JPEG_ARM_ERROR_BAD_DHT_MARKER				0x80650125
#define SCE_JPEG_ARM_ERROR_BAD_DRI_MARKER				0x80650126
#define SCE_JPEG_ARM_ERROR_BAD_SOF_MARKER				0x80650127
#define SCE_JPEG_ARM_ERROR_BAD_SOS_MARKER				0x80650128
#define SCE_JPEG_ARM_ERROR_SOF_DUPLICATE				0x80650129
#define SCE_JPEG_ARM_ERROR_NO_LOSSLESS_SUPPORT			0x80650131
#define SCE_JPEG_ARM_ERROR_NO_ARITH_SUPPORT				0x80650132
#define SCE_JPEG_ARM_ERROR_UNKNOWN_MARKER				0x80650131
#define SCE_JPEG_ARM_ERROR_BAD_RESTART_MARKER			0x80650136
#define SCE_JPEG_ARM_ERROR_UNEXPECTED_MARKER			0x80650137
#define SCE_JPEG_ARM_ERROR_INVALID_REGION				0x80650138
#define SCE_JPEG_ARM_ERROR_INVALID_STATE				0x80650139
#define SCE_JPEG_ARM_ERROR_CANNOT_CONTINUE				0x80650140
#define SCE_JPEG_ARM_ERROR_MEMORY_SIZE					0x80650141
#define SCE_JPEG_ARM_ERROR_INVALID_COLOR_FORMAT			0x80650150
#define SCE_JPEG_ARM_ERROR_INVALID_DECODE_MODE			0x80650160
#define SCE_JPEG_ARM_ERROR_BAD_PROGRESSIVE_PARAM		0x80650161
#define SCE_JPEG_ARM_ERROR_INIT_DONE					0x80650163
#define SCE_JPEG_ARM_ERROR_INPUT_SUSPENDED				0x80650164
#define SCE_JPEG_ARM_ERROR_INPUT_DATA_TOO_BIG			0x80650165
#define SCE_JPEG_ARM_ERROR_INVALID_DATA_SIZE			0x80650166

#define SCE_JPEG_ARM_ERROR_UNSUPPORT_DOWNSCALE	(SCE_JPEG_ARM_ERROR_INVALID_DECODE_MODE)
#define SCE_JPEG_ARM_ERROR_OUT_OF_MEMORY		(SCE_JPEG_ARM_ERROR_MEMORY_SIZE)

int sceJpegArmDecodeMJpeg(												// SceJpegArm_A4ABFCE3
		const unsigned char *pJpeg,
		SceSize isize,
		int decodeMode,
		void *pRGBA,
		SceSize osize,
		void *pCoefBuffer,
		SceSize coefBufferSize);

int sceJpegArmDecodeMJpegYCbCr(											// SceJpegArm_E9B1B86F
		const unsigned char *pJpeg,
		SceSize isize,
		int decodeMode,
		unsigned char *pYCbCr,
		SceSize osize,
		void *pCoefBuffer,
		SceSize coefBufferSize);

int sceJpegArmGetOutputInfo(											// 	SceJpegArm_23AE3BEA
		const unsigned char *pJpeg,
		SceSize isize,
		int decodeMode,
		int outputFormat,
		SceJpegOutputInfo *pOutputInfo);

/* split decoder (control struct is different from sceJpeg, no reference anywhere in the apps) */

/*int sceJpegArmCreateSplitDecoder(void *pCtrl);						// SceJpegArm_9DA48DB6
int sceJpegArmDeleteSplitDecoder(void *pCtrl);							// SceJpegArm_E9CB3DFD

int sceJpegArmSplitDecodeMJpeg(void *pCtrl);							// SceJpegArm_5D83C606*/

#endif

