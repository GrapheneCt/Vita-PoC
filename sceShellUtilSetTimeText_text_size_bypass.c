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

typedef struct SceShellSvcCustomParams {
	void* params1;  	// optional params1
	SceSize params1Size; 	// size of optional params1
	void* params2; 		// optional params2, ex. path to audio file
	SceSize params2Size;	// size of optional params2
	void* params3;		// optional params3
	SceSize params3Size;	// size of optional params3
} SceShellSvcCustomParams;

typedef struct SceShellSvcTable {
	void *pFunc_0x00;
	void *pFunc_0x04;
	void *pFunc_0x08;
	void *pFunc_0x0C;
	void *pFunc_0x10;
	void *pFunc_0x14;
	void *pFunc_0x18;
	int(*sceShellSvcAsyncMethod)(void *obj, int asyncMethodId, SceShellSvcCustomParams*, int a4_flag, int* a5, void* a6);

	// more ...
} SceShellSvcTable;

void* SceShellSvc_B31E7F1C(void); //SceShellSvcGetTable

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

void _start(unsigned int args, void *argp)
{
	//////


	//I have no idea where that first text is displayed
	char msg1[0x40];
	sceClibMemset(&msg1, 0, 0x100);
	copycon(msg1, "Test");

	//Notification bar text
	char msg2[0x40];
	sceClibMemset(&msg2, 0, 0x100);
	copycon(msg2, "Bypassing notification bar text size limit has been successful!");

	void* ptr = SceShellSvc_B31E7F1C();

	SceShellSvcCustomParams mainParams;

	int arg5 = -1;

	mainParams.params1 = &msg1;
	mainParams.params1Size = 0x100;
	mainParams.params2 = &msg2;
	mainParams.params2Size = 0x100;

	ret = ((SceShellSvcTable *)(*(uint32_t *)ptr))->sceShellSvcAsyncMethod(ptr, 0xC0004, &mainParams, 2, &arg5, NULL);

	/////////

	while (1) {
		sceKernelDelayThread(100000);
	}
	
}

