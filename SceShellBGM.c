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

extern int sceAppMgrSetBgmProxyApp(const char*);
extern void* SceShellSvc_B31E7F1C(void); //SceShellSvcGetTable

void bgmStop(void)
{
	sceAppMgrSetBgmProxyApp("NPXS19999");
	void* tptr = SceShellSvc_B31E7F1C();

	SceShellSvcCustomAudioSubParams1 str1;
	str1.unk_00 = 0;
	str1.unk_04 = 0;
	str1.unk_08 = 0;

	SceShellSvcAudioCustomParams mainParams;
	mainParams.params1 = &str1;
	mainParams.params1Size = 0xC;

	SceShellSvcAudioSoundParams soundParams;
	soundParams.audioParams = 0;
	soundParams.audioType = 0;

	int ret = ((SceShellSvcTable *)(*(uint32_t *)tptr))->sceShellSvcAudioControl(tptr, 0xD0000, &mainParams, 1, &soundParams, 0, 0);

	sceClibPrintf("bgmStop: 0x%08x\n", ret);
}

void bgmStart1(void)
{
	void* tptr = SceShellSvc_B31E7F1C();

	char path[] = "pd0:data/systembgm/near.at9";

	SceShellSvcCustomAudioSubParams1 str1;
	str1.unk_00 = 0;
	str1.unk_04 = 0;
	str1.unk_08 = 0;

	SceShellSvcCustomAudioSubParams2 str2;
	str2.audioPathPtr = &path;
	str2.unk_04 = 0;
	str2.unk_08 = 0;
	str2.unk_0C = 0;

	SceShellSvcAudioCustomParams mainParams;
	mainParams.params1 = &str1;
	mainParams.params1Size = 0xC;
	mainParams.params2 = &path;
	mainParams.params2Size = 27;
	mainParams.params3 = &str2;
	mainParams.params3Size = 0x10;

	SceShellSvcAudioSoundParams soundParams;
	soundParams.audioParams = 0;
	soundParams.audioType = 0;

	int ret = ((SceShellSvcTable *)(*(uint32_t *)tptr))->sceShellSvcAudioControl(tptr, 0xD0002, &mainParams, 3, &soundParams, 0, 0);

	sceClibPrintf("bgmStart1: 0x%08x\n", ret);
}

void bgmStart2(void)
{
	void* tptr = SceShellSvc_B31E7F1C();

	SceShellSvcCustomAudioSubParams1 str1;
	str1.unk_00 = 0;
	str1.unk_04 = global_1;
	str1.unk_08 = global_2;

	SceShellSvcAudioCustomParams mainParams;
	mainParams.params1 = NULL;
	mainParams.params1Size = 0;
	mainParams.params2 = &strmain.params1;
	mainParams.params2Size = 8;
	mainParams.params3 = &str1;
	mainParams.params3Size = 0x0C;

	SceShellSvcAudioSoundParams soundParams;
	soundParams.audioParams = 0;
	soundParams.audioType = 0;

	int ret = ((SceShellSvcTable *)(*(uint32_t *)tptr))->sceShellSvcAudioControl(tptr, 0xD0004, &mainParams, 2, &soundParams, 0, 0);

	sceClibPrintf("bgmStart2: 0x%08x\n", ret);
}

int main(void)
{
	bgmStop();
	bgmStart1();
	bgmStart2();

	while (1) {
		sceKernelDelayThread(1000);
	}
	
	return 0;
}

