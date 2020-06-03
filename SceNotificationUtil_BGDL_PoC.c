#include <stdbool.h>
#include <psp2/kernel/modulemgr.h> 
#include <psp2/kernel/threadmgr.h> 
#include <psp2/appmgr.h> 
#include <psp2/kernel/sysmem.h> 
#include <psp2/kernel/clib.h>
#include <psp2/types.h> 
#include <psp2/sysmodule.h> 
#include <psp2/kernel/processmgr.h> 

typedef void(*SceNotificationUtilProgressEventHandler)(int a1);

typedef struct SceNotificationUtilProgressInitParams {
	SceWChar16 notificationText[0x273];
	short separator0; //must be 0
	SceNotificationUtilProgressEventHandler eventHandler;
	int unk_4EC;
	SceWChar16 cancelDialogText[0x3E];
	short separator1; //must be 0
} SceNotificationUtilProgressInitParams;

typedef struct SceNotificationUtilProgressUpdateParams {
	SceWChar16 notificationText[0x7E];
	short separator0; //must be 0
	float targetProgress;
	char reserved[0x3C];
} SceNotificationUtilProgressUpdateParams;

typedef struct SceNotificationUtilProgressFinishParams {
	SceWChar16 notificationText[0x273];
	short separator0; //must be 0
} SceNotificationUtilProgressFinishParams;

int SceNotificationUtilProgress_FD0769B0(SceNotificationUtilProgressInitParams* initParams); // sceNotificationUtilProgressBegin
int SceNotificationUtilProgress_D9890A65(SceNotificationUtilProgressUpdateParams* updateParams); // sceNotificationUtilProgressUpdate
int SceNotificationUtilProgress_8CD688A1(SceNotificationUtilProgressFinishParams* finishParams); // sceNotificationUtilProgressFinish

void copycon(char* str1, const char* str2)
{
	while (*str2)
	{
		*str1 = *str2;
		str1++;
		*str1 = '\0';
		str1++;
		str2++;
	}
}

void eventHandler(int a1)
{
	sceClibPrintf("EVENT DETECTED: %d\n", a1);
}

void _start(unsigned int args, void *argp)
{
	int ret = 0;

	sceSysmoduleLoadModule(SCE_SYSMODULE_NOTIFICATION_UTIL);

	SceNotificationUtilProgressInitParams initParams;
	sceClibMemset(&initParams, 0, sizeof(SceNotificationUtilProgressInitParams));

	copycon(initParams.notificationText, "Test BGDL Notification");
	copycon(initParams.cancelDialogText, "Cancel test BGDL?");

	initParams.eventHandler = eventHandler;

	ret = SceNotificationUtilProgress_FD0769B0(&initParams);
	sceClibPrintf("sceNotificationUtilProgressBegin: 0x%x\n", ret);

	sceKernelDelayThread(1000000);

	SceNotificationUtilProgressUpdateParams updateParams;
	sceClibMemset(&updateParams, 0, sizeof(SceNotificationUtilProgressUpdateParams));

	while (1) {
		updateParams.targetProgress++;
		ret = SceNotificationUtilProgress_D9890A65(&updateParams);
		if (ret == 0)
			sceClibPrintf("CURRENT PROGRESS SET TO %f\n", updateParams.targetProgress);
		if (updateParams.targetProgress == 100 || ret < 0)
			break;
		sceKernelDelayThread(100000);
	}

	SceNotificationUtilProgressFinishParams finishParams;
	sceClibMemset(&finishParams, 0, sizeof(SceNotificationUtilProgressFinishParams));

	copycon(finishParams.notificationText, "Test BGDL has been finished!");

	ret = SceNotificationUtilProgress_8CD688A1(&finishParams);
	sceClibPrintf("sceNotificationUtilProgressFinish: 0x%x\n", ret);
	
}

