/**
 * \usergroup{SceIncomingDialog}
 * \usage{psp2/incoming_dialog.h,SceIncomingDialog_stub,SCE_SYSMODULE_INCOMING_DIALOG}
 */


#ifndef _DOLCESDK_PSP2_INCOMINGDIALOG_H_
#define _DOLCESDK_PSP2_INCOMINGDIALOG_H_

#include <psp2/kernel/clib.h>
#include <psp2/types.h>
#include <psp2/scebase.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Dialog status
 */
typedef enum SceIncomingDialogStatus {
	SCE_INCOMING_DIALOG_NOT_RUNNING,
	SCE_INCOMING_DIALOG_BUSY,
	SCE_INCOMING_DIALOG_RUNNING,
	SCE_INCOMING_DIALOG_ACCEPTED,
	SCE_INCOMING_DIALOG_REJECTED,
	SCE_INCOMING_DIALOG_CLOSED,
	SCE_INCOMING_DIALOG_TIMEOUT
} SceIncomingDialogStatus;

/**
 * Errors
 */
#define SCE_INCOMINGDIALOG_ERROR_INVALID_ARG               0x80106202;

typedef struct SceIncomingDialogParam {
	int sdkVersion;
	char titleid[0x10];			//TitleId of the application to open when "accept" button has been pressed. Can be NULL
	char audioPath[0x80];			//Path to audio file that will be played during dialog, .mp3, .at9, m4a. Can be NULL
	unsigned int dialogTimer;		//Time to show dialog in seconds
	int unk_BC;				//Can be set to 0
	char reserved1[0x3E];
	SceWChar16 buttonRightText[0x1F];	//Text for "accept" button
	short separator0;			//must be 0
	SceWChar16 buttonLeftText[0x1F];	//Text for "reject" button. If NULL, only "accept" button will be created
	short separator1;			//must be 0
	SceWChar16 dialogWindowText[0x80];	//Text for dialog window, also shared with notification
	short separator2;			//must be 0
} SceIncomingDialogParam;

/**
 * Initialize incoming dialog library, init_type must be 0.
 */
SceInt32 SceIncomingDialogInitialize(int init_type); // SceIncomingDialog_18AF99EB

/**
 * Open incoming dialog.
 */
SceInt32 SceIncomingDialogOpen(SceIncomingDialogParam* dialogParam); // SceIncomingDialog_2BEDC1A0

/**
 * Returns current status of incoming dialog.
 */
SceInt32 SceIncomingDialogGetStatus(void); // SceIncomingDialog_839DE14C

/**
 * Force exit to LiveArea and show dialog window
 */
SceInt32 SceIncomingDialogSwitchToDialog(void); // SceIncomingDialog_0123B83A

/**
 * Close incoming dialog.
 */
SceInt32 SceIncomingDialogClose(void); // SceIncomingDialog_126BD15E

/**
 * Finish incoming dialog library
 */
SceInt32 SceIncomingDialogFinish(void); // SceIncomingDialog_860B1885

static inline
void sceIncomingDialogParamInit(SceIncomingDialogParam* dialogParam)
{
	sceClibMemset(dialogParam, 0x0, sizeof(SceIncomingDialogParam));
	dialogParam->sdkVersion = SCE_PSP2_SDK_VERSION;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _DOLCESDK_PSP2_INCOMINGDIALOG_H_ */
