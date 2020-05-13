#include <psp2/kernel/modulemgr.h> 
#include <psp2/kernel/threadmgr.h> 
#include <psp2/appmgr.h> 
#include <psp2/kernel/sysmem.h> 
#include <psp2/kernel/clib.h>
#include <psp2/types.h> 
#include <psp2/sysmodule.h> 
#include <psp2/io/fcntl.h> 
#include <psp2/shellutil.h> 

/* EXTERNAL */

typedef struct SceShellSvcCustomAudioBgmOpt {
	int flag; // set to 0 if not used
	int param1;
	int param2;
	int param3;
} SceShellSvcCustomAudioBgmOpt;

/* INTERNAL */

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
	int flag;
	int unk_04;
	int unk_08;
	int unk_0C;
} SceShellSvcCustomAudioSubParams2;

typedef struct SceShellSvcCustomAudioSubParams3 {
	int param1;
	int param2;
} SceShellSvcCustomAudioSubParams3;

typedef struct SceShellSvcCustomAudioParams {
	void* params1;  	// optional params1
	SceSize params1Size; 	// size of optional params1
	void* params2; 		// optional params2, ex. path to audio file
	SceSize params2Size;	// size of optional params2
	void* params3;		// optional params3
	SceSize params3Size;	// size of optional params3
} SceShellSvcAudioCustomParams;

typedef struct SceShellSvcAudioSoundParams {
	int soundParam; // usually 0
	int soundType;  // usually 0
} SceShellSvcAudioSoundParams;

typedef struct SceShellSvcTable {
	void *pFunc_0x00;
	void *pFunc_0x04;
	void *pFunc_0x08;
	void *pFunc_0x0C;
	void *pFunc_0x10;
	int(*sceShellSvcAudioControl)(void *obj, int flag, SceShellSvcAudioCustomParams*, int a4_flag, SceShellSvcAudioSoundParams*, void *a6, int a7);
	void *pFunc_0x18;
	int(*sceShellSvcAsyncMethod)(void *obj, int asyncMethodId /* more args here */);

	// more ...
} SceShellSvcTable;

typedef enum SceShellSvcAudioErrorCodes {
	SCE_SHELLAUDIO_ERROR_INVALID_ARG = 0x80101900,
	SCE_SHELLAUDIO_ERROR_NOT_INITIALIZED,
	SCE_SHELLAUDIO_ERROR_ALREADY_INITIALIZED,
	SCE_SHELLAUDIO_ERROR_NOT_READY
} SceShellSvcAudioErrorCodes;

extern int sceAppMgrSetBgmProxyApp(const char*);
extern void* SceShellSvc_B31E7F1C(void); //SceShellSvcGetTable

static int isInitialized = 0;
static int isReady = 0;

static int global_1 = 0;
static int global_2 = 0;

int bgmStop(int init_type)
{
	int ret;

	if (isInitialized) {
		ret = SCE_SHELLAUDIO_ERROR_ALREADY_INITIALIZED;
		goto end;
	}

	if (init_type != 0 && init_type != 1) {
		ret = SCE_SHELLAUDIO_ERROR_INVALID_ARG;

		goto end;
	}

	if (init_type == 0)
		sceAppMgrSetBgmProxyApp("NPXS19999");

	void* tptr = SceShellSvc_B31E7F1C();

	SceShellSvcCustomAudioSubParams1 params1;
	params1.unk_00 = 0;
	params1.unk_04 = init_type;
	params1.unk_08 = 0;

	SceShellSvcAudioCustomParams mainParams;
	mainParams.params1 = &params1;
	mainParams.params1Size = 0xC;

	SceShellSvcAudioSoundParams soundParams;
	sceClibMemset(&soundParams, 0, 0x8);

	ret = ((SceShellSvcTable *)(*(uint32_t *)tptr))->sceShellSvcAudioControl(tptr, 0xD0000, &mainParams, 1, &soundParams, 0, 0);

	if (ret != 0)
		goto end;
		
	if (soundParams.soundParam == 0) {
		global_1 = params1.unk_04;
		global_2 = params1.unk_08;
		isInitialized = 1;
		isReady = 0;
	}

end:

	return ret;
}

int bgmSetCustomAudio(char* path, SceShellSvcCustomAudioBgmOpt* optParams)
{
	int ret, pathlen;

	if (!isInitialized) {
		ret = SCE_SHELLAUDIO_ERROR_NOT_INITIALIZED;
		goto end;
	}

	if (path == NULL) {
		ret = SCE_SHELLAUDIO_ERROR_INVALID_ARG;
		goto end;
	}

	if (optParams->param1 != 0 && optParams->param1 != 1) {
		ret = SCE_SHELLAUDIO_ERROR_INVALID_ARG;
		goto end;
	}

	pathlen = sceClibStrnlen(path, 0x401);

	void* tptr = SceShellSvc_B31E7F1C();

	SceShellSvcCustomAudioSubParams1 params1;
	params1.unk_00 = 0;
	params1.unk_04 = global_1;
	params1.unk_08 = global_2;

	SceShellSvcCustomAudioSubParams2 params2;
	sceClibMemset(&params2, 0, 0x10);

	if (optParams->flag != 0) {
		sceClibMemcpy(&params2, optParams, 0x10);
	}

	SceShellSvcAudioCustomParams mainParams;
	mainParams.params1 = &params1;
	mainParams.params1Size = 0xC;
	mainParams.params2 = path;
	mainParams.params2Size = pathlen;
	mainParams.params3 = &params2;
	mainParams.params3Size = 0x10;

	SceShellSvcAudioSoundParams soundParams;
	sceClibMemset(&soundParams, 0, 0x8);

	ret = ((SceShellSvcTable *)(*(uint32_t *)tptr))->sceShellSvcAudioControl(tptr, 0xD0002, &mainParams, 3, &soundParams, 0, 0);

	if (ret != 0)
		goto end;

	if (soundParams.soundParam == 0) {
		isReady = 1;
	}

end:

	return ret;
}

int bgmSetParam1(int param)
{
	int ret;

	if (!isInitialized) {
		ret = SCE_SHELLAUDIO_ERROR_NOT_INITIALIZED;
		goto end;
	}

	if (param < 0 || param > 0x8000) {
		ret = SCE_SHELLAUDIO_ERROR_INVALID_ARG;
		goto end;
	}

	void* tptr = SceShellSvc_B31E7F1C();

	SceShellSvcCustomAudioSubParams1 params1;
	params1.unk_00 = 0;
	params1.unk_04 = global_1;
	params1.unk_08 = global_2;

	SceShellSvcAudioCustomParams mainParams;
	mainParams.params1 = &param;
	mainParams.params1Size = 0x4;
	mainParams.params2 = &params1;
	mainParams.params2Size = 0xC;

	SceShellSvcAudioSoundParams soundParams;
	sceClibMemset(&soundParams, 0, 0x8);

	ret = ((SceShellSvcTable *)(*(uint32_t *)tptr))->sceShellSvcAudioControl(tptr, 0xD0006, &mainParams, 2, &soundParams, 0, 0);

	if (ret != 0)
		goto end;

end:

	return ret;
}

int bgmSetParam2(int8_t param)
{
	int ret;

	if (!isInitialized) {
		ret = SCE_SHELLAUDIO_ERROR_NOT_INITIALIZED;
		goto end;
	}

	if (param != 0 && param != 1 && param != 2) {
		ret = SCE_SHELLAUDIO_ERROR_INVALID_ARG;
		goto end;
	}

	void* tptr = SceShellSvc_B31E7F1C();

	SceShellSvcCustomAudioSubParams1 params1;
	params1.unk_00 = 0;
	params1.unk_04 = global_1;
	params1.unk_08 = global_2;

	SceShellSvcAudioCustomParams mainParams;
	mainParams.params1 = &param;
	mainParams.params1Size = 0x1;
	mainParams.params2 = &params1;
	mainParams.params2Size = 0xC;

	SceShellSvcAudioSoundParams soundParams;
	sceClibMemset(&soundParams, 0, 0x8);

	ret = ((SceShellSvcTable *)(*(uint32_t *)tptr))->sceShellSvcAudioControl(tptr, 0xD0007, &mainParams, 2, &soundParams, 0, 0);

	if (ret != 0)
		goto end;

end:

	return ret;
}

int bgmStart(int param_1, int param_2)
{
	int ret;

	if (!isInitialized) {
		ret = SCE_SHELLAUDIO_ERROR_NOT_INITIALIZED;
		goto end;
	}

	if (!isReady) {
		ret = SCE_SHELLAUDIO_ERROR_NOT_READY;
		goto end;
	}

	if (param_1 > 0x11 || param_1 < 0) {
		ret = SCE_SHELLAUDIO_ERROR_INVALID_ARG;
		goto end;
	}

	void* tptr = SceShellSvc_B31E7F1C();

	SceShellSvcCustomAudioSubParams1 params1;
	params1.unk_00 = 0;
	params1.unk_04 = global_1;
	params1.unk_08 = global_2;

	SceShellSvcCustomAudioSubParams3 params2;
	params2.param1 = param_1;
	params2.param2 = param_2;

	SceShellSvcAudioCustomParams mainParams;
	mainParams.params1 = &params2;
	mainParams.params1Size = 0x8;
	mainParams.params2 = &params1;
	mainParams.params2Size = 0xC;

	SceShellSvcAudioSoundParams soundParams;
	sceClibMemset(&soundParams, 0, 0x8);

	ret = ((SceShellSvcTable *)(*(uint32_t *)tptr))->sceShellSvcAudioControl(tptr, 0xD0004, &mainParams, 2, &soundParams, 0, 0);

	if (ret != 0)
		goto end;

end:

	return ret;
}

int main(void)
{
	int ret;

	ret = bgmStop(0);
	sceClibPrintf("bgmStop: 0x%08x\n", ret);

	SceShellSvcCustomAudioBgmOpt optParams;
	sceClibMemset(&optParams, 0, 0x10);
	optParams.flag = -1;
	ret = bgmSetCustomAudio("pd0:data/systembgm/near.at9", &optParams);
	sceClibPrintf("bgmSetCustomAudio: 0x%08x\n", ret);

	ret = bgmSetParam1(0x4E20);
	sceClibPrintf("bgmSetParam1: 0x%08x\n", ret);

	ret = bgmSetParam2(1);
	sceClibPrintf("bgmSetParam2: 0x%08x\n", ret);

	ret = bgmStart(0, 0);
	sceClibPrintf("bgmStart: 0x%08x\n", ret);

	while (1) {
		sceKernelDelayThread(1000);
	}

	return 0;
}

