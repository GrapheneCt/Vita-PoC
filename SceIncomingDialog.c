#include <stdbool.h>
#include <psp2/kernel/modulemgr.h> 
#include <psp2/kernel/threadmgr.h> 
#include <psp2/appmgr.h> 
#include <psp2/kernel/sysmem.h> 
#include <psp2/kernel/clib.h>
#include <psp2/types.h> 
#include <psp2/sysmodule.h> 
#include <psp2/io/fcntl.h> 
#include <psp2/shellutil.h> 

typedef struct SceIncomingDialogParam {
	int fw; //ex. 0x1650041
	char titleid[0x10]; //ex. "PCSA00044" (icon0.png of that app will be shown in dialog window)
	char audioPath[0x80]; //ex. "app0:resource/CallRingingIn.mp3" .at9 and .aac also supported (audio will be played while dialog is active, playback is done by sceShell)
	unsigned int dialogTimer; //Time to show dialog in seconds (including audio)
	int unk_BC; //ex. 1
	char reserved1[0x3E];
	char buttonRightText[0x3E]; //UTF-16 (function - accept. Opens app from titleid)
	short separator0; //must be 0
	char buttonLeftText[0x3E]; //UTF-16 (function - reject). If 0, only right button will be created
	short separator1; //must be 0
	char dialogWindowText[0x100]; //UTF-16 (also displayed in first notification)
	short separator2; //must be 0
} SceIncomingDialogParam;

extern int SceIncomingDialog_18AF99EB(int);
extern int SceIncomingDialog_2BEDC1A0(const SceIncomingDialogParam*);
extern int SceIncomingDialog_126BD15E(void);
extern int SceIncomingDialog_839DE14C(void);
extern int SceIncomingDialog_0123B83A(void);

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

int main(void)
{
	int ret = sceSysmoduleLoadModule(SCE_SYSMODULE_INCOMING_DIALOG);
	sceClibPrintf("sceSysmoduleLoadModule: 0x%08x\n", ret);

	ret = SceIncomingDialog_18AF99EB(0);
	sceClibPrintf("SceIncomingDialog_18AF99EB: 0x%08x\n", ret);

	SceIncomingDialogParam params;
	sceClibMemset(&params, 0, sizeof(SceIncomingDialogParam));
	params.fw = 0x1650041;
	sceClibStrncpy(params.titleid, "SOMETILID", sizeof(params.titleid));
	sceClibStrncpy(params.audioPath, "app0:test.at9", sizeof(params.audioPath));
	params.dialogTimer = 100;
	params.unk_BC = 1;
	copycon(params.buttonRightText, "Accept");
	copycon(params.buttonLeftText, "Reject");
	copycon(params.dialogWindowText, "Test text!");

	ret = SceIncomingDialog_2BEDC1A0(&params);
	sceClibPrintf("SceIncomingDialog_2BEDC1A0: 0x%08x\n", ret);

	while (1) {
		sceKernelDelayThread(1000);
	}
	
	return 0;
}

