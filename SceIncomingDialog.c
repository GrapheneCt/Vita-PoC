//#include <stdio.h>
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

/*typedef struct SceIncomingDialogParam {
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
extern int SceIncomingDialog_0123B83A(void);*/

/* events
0xD0000 - stop BGM
0xD0002 - set custom BGM
0xD0004 - (seek/start?) BGM
*/

typedef struct SceShellSvcCustomAudioSubParams1 {
	int unk_00; // not used (set to 0)
	int unk_04; // [out]?
	int unk_08; // [out]?
} SceShellSvcCustomAudioSubParams1;

typedef struct SceShellSvcCustomAudioSubParams2 {
	char* audioPathPtr;
	int unk_04; // initially not used (set to 0), after to static var [out]?
	int unk_08; // initially not used (set to 0), after to static var [out]?
	int unk_0C; // initially not used (set to 0), after to static var [out]?
} SceShellSvcCustomAudioSubParams2;

typedef struct SceShellSvcCustomAudioParams {
	int unk_00; // not used (set to 0)
	void* params1; // optional params1
	SceSize params1Size; // size of optional params1
	void* params2; // optional params2, ex. path to audio file
	SceSize params2Size; // size of optional params2
	void* params3; // optional params3
	SceSize params3Size; // size of optional params3
} SceShellSvcCustomAudioParams;

typedef struct SceShellSvcAudioParams {
	SceShellSvcCustomAudioParams* audioParams;
	int audioType;
} SceShellSvcAudioParams;

typedef struct SceShellSvcTable {
	void *pFunc_0x00;
	void *pFunc_0x04;
	void *pFunc_0x08;
	void *pFunc_0x0C;
	void *pFunc_0x10;
	int(*sceShellSvcAudioControl)(void *obj, int flag, void* a3, int a4_flag, SceShellSvcAudioParams*, void *a6, int a7);
	void *pFunc_0x18;
	int(*sceShellSvcAsyncMethod)(void *obj, int asyncMethodId /* more args here */);

	// more ...
} SceShellSvcTable;

extern int sceAppMgrSetBgmProxyApp(const char*);
extern void* SceShellSvc_B31E7F1C(void); //SceShellSvcGetTable

static int init_step_1 = 0;
static int init_step_2 = 0;

static int global_1 = 0;
static int global_2 = 0;

/* for notification text */
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

void bgmStop(void)
{
	sceAppMgrSetBgmProxyApp("NPXS19999");
	void* tptr = SceShellSvc_B31E7F1C();

	SceShellSvcCustomAudioSubParams1 str1;
	str1.unk_00 = 0;
	str1.unk_04 = 0;
	str1.unk_08 = 0;

	SceShellSvcCustomAudioParams strmain;
	strmain.unk_00 = 0;
	strmain.params1 = &str1;
	strmain.params1Size = 0xC;

	SceShellSvcAudioParams audio;
	audio.audioParams = &strmain;
	audio.audioType = 0;

	int ret = ((SceShellSvcTable *)(*(uint32_t *)tptr))->sceShellSvcAudioControl(tptr, 0xD0000, &str1, 1, &audio, 0, 0);

	global_1 = str1.unk_04;
	global_2 = str1.unk_08;

	init_step_1 = 1;
	init_step_2 = 0;

	sceClibPrintf("bgmStop: 0x%08x\n", ret);
}

void bgmStart1(void)
{
	void* tptr = SceShellSvc_B31E7F1C();

	char path[] = "pd0:data/systembgm/near.at9";

	SceShellSvcCustomAudioSubParams1 str1;
	str1.unk_00 = 0;
	str1.unk_04 = global_1;
	str1.unk_08 = global_2;

	SceShellSvcCustomAudioSubParams2 str2;
	str2.audioPathPtr = &path;
	str2.unk_04 = 0;
	str2.unk_08 = 0;
	str2.unk_0C = 0;

	SceShellSvcCustomAudioParams strmain;
	strmain.unk_00 = 0;
	strmain.params1 = &str1;
	strmain.params1Size = 0xC;
	strmain.params2 = &path;
	strmain.params2Size = 27;
	strmain.params3 = &str2;
	strmain.params3Size = 0x10;

	SceShellSvcAudioParams audio;
	audio.audioParams = &strmain;
	audio.audioType = 0;

	int ret = ((SceShellSvcTable *)(*(uint32_t *)tptr))->sceShellSvcAudioControl(tptr, 0xD0002, &strmain.params1, 3, &audio, 0, 0);

	sceClibPrintf("bgmStart1: 0x%08x\n", ret);
}

void bgmStart2(void)
{
	void* tptr = SceShellSvc_B31E7F1C();

	SceShellSvcCustomAudioSubParams1 str1;
	str1.unk_00 = 0;
	str1.unk_04 = global_1;
	str1.unk_08 = global_2;

	SceShellSvcCustomAudioParams strmain;
	strmain.unk_00 = 0;
	strmain.params1 = NULL;
	strmain.params1Size = 0;
	strmain.params2 = &strmain.params1;
	strmain.params2Size = 8;
	strmain.params3 = &str1;
	strmain.params3Size = 0x0C;

	SceShellSvcAudioParams audio;
	audio.audioParams = &strmain;
	audio.audioType = 0;

	int ret = ((SceShellSvcTable *)(*(uint32_t *)tptr))->sceShellSvcAudioControl(tptr, 0xD0004, &strmain.params1, 2, &audio, 0, 0);

	sceClibPrintf("bgmStart2: 0x%08x\n", ret);
}

void _start(unsigned int args, void *argp)
{
	/*int ret = sceSysmoduleLoadModule(SCE_SYSMODULE_INCOMING_DIALOG);
	sceClibPrintf("sceSysmoduleLoadModule: 0x%08x\n", ret);

	ret = SceIncomingDialog_18AF99EB(0);
	sceClibPrintf("SceIncomingDialog_18AF99EB: 0x%08x\n", ret);

	SceIncomingDialogParam params;
	sceClibMemset(&params, 0, sizeof(SceIncomingDialogParam));
	params.fw = 0x1650041;
	sceClibStrncpy(params.titleid, "MARISAHIL", sizeof(params.titleid));
	sceClibStrncpy(params.audioPath, "app0:test.at9", sizeof(params.audioPath));
	params.dialogTimer = 100;
	params.unk_BC = 1;
	copycon(params.buttonRightText, "Accept");
	copycon(params.buttonLeftText, "Reject");
	copycon(params.dialogWindowText, "Test text!");

	ret = SceIncomingDialog_2BEDC1A0(&params);
	sceClibPrintf("SceIncomingDialog_2BEDC1A0: 0x%08x\n", ret);*/

	bgmStop();
	bgmStart1();
	bgmStart2();

	while (1) {
		sceKernelDelayThread(1000);
	}

}

