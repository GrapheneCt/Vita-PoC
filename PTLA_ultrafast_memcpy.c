#include <psp2/kernel/sysmem.h> 
#include <psp2/libdbg.h>
#include <psp2/gxm.h>
#include <taihen.h>

#include "transfer.h"

static volatile SceUInt32 *state;

void PTLAInitialize(void)
{
	int ret = 0;
	void* test_mem;
	SceUID mb_id = sceKernelAllocMemBlock("TransferTestBlock", SCE_KERNEL_MEMBLOCK_TYPE_USER_RW_UNCACHE, 1 * 1024 * 1024, NULL);

	if (mb_id < 0) {
		SCE_DBG_LOG_ERROR("[TRN] sceKernelAllocMemBlock(): 0x%X\n", mb_id);
		SCE_DBG_LOG_WARNING("[TRN] Initializing in limited mode...");
		return;
	}

	sceKernelGetMemBlockBase(mb_id, &test_mem);
	ret = sceGxmMapMemory(test_mem, 1 * 1024 * 1024, SCE_GXM_MEMORY_ATTRIB_READ | SCE_GXM_MEMORY_ATTRIB_WRITE);

	if (ret < 0) {
		SCE_DBG_LOG_ERROR("[TRN] sceGxmMapMemory(): 0x%X\n", ret);
		SCE_DBG_LOG_WARNING("[TRN] Initializing in limited mode...");
		sceKernelFreeMemBlock(mb_id);
		return;
	}

	ret = sceGxmTransferCopy(
		1025,
		1,
		0,
		0,
		SCE_GXM_TRANSFER_COLORKEY_NONE,
		SCE_GXM_TRANSFER_FORMAT_U8_R,
		SCE_GXM_TRANSFER_LINEAR,
		test_mem,
		0,
		0,
		0,
		SCE_GXM_TRANSFER_FORMAT_U8_R,
		SCE_GXM_TRANSFER_LINEAR,
		test_mem + 1023,
		0,
		0,
		0,
		NULL,
		0,
		NULL);
	sceGxmTransferFinish();

	if (ret < 0) {

		SCE_DBG_LOG_DEBUG("[TRN] Injecting size checks");

		tai_module_info_t info;
		info.size = sizeof(info);
		taiGetModuleInfo("SceGxm", &info);
		SceUID inj_id_1, inj_id_2, inj_id_3, inj_id_4;

		inj_id_1 = taiInjectData(info.modid, 0x00, 0x1BD14, "\x03\x0A\x52\xE3", 0x04);
		inj_id_2 = taiInjectData(info.modid, 0x00, 0x1BD28, "\x03\x0A\x53\xE3", 0x04);

		inj_id_3 = taiInjectData(info.modid, 0x00, 0x1B880, "\x03\x0A\x53\xE3", 0x04);
		inj_id_4 = taiInjectData(info.modid, 0x00, 0x1B88C, "\x03\x0A\x53\xE3", 0x04);

		if (inj_id_1 < 0 || inj_id_2 < 0 || inj_id_3 < 0 || inj_id_4 < 0)
			SCE_DBG_LOG_WARNING("[TRN] Injecting failed, initializing in limited mode...");
	}
	else
		SCE_DBG_LOG_DEBUG("[TRN] Checks have been injected already");

	state = sceGxmGetNotificationRegion();

	sceGxmUnmapMemory(test_mem);
	sceKernelFreeMemBlock(mb_id);
}

SceUInt32 PTLAPollAsync(void)
{
	if (!*state)
		return 0;
	else {
		*state = 0;
		return 1;
	}
}

void* PTLAMemcpy(void *dst, const void *src, SceSize blockSize, SceSize numOfBlocks)
{
	sceGxmTransferCopy(
		blockSize,
		numOfBlocks,
		0,
		0,
		SCE_GXM_TRANSFER_COLORKEY_NONE,
		SCE_GXM_TRANSFER_FORMAT_U8_R,
		SCE_GXM_TRANSFER_LINEAR,
		src,
		0,
		0,
		blockSize,
		SCE_GXM_TRANSFER_FORMAT_U8_R,
		SCE_GXM_TRANSFER_LINEAR,
		dst,
		0,
		0,
		blockSize,
		NULL,
		0,
		NULL);
	sceGxmTransferFinish();

	return dst;
}

void* PTLAMemcpyAsync(void *dst, const void *src, SceSize blockSize, SceSize numOfBlocks)
{
	SceGxmNotification async_notify;
	async_notify.address = state;
	async_notify.value = 1;

	sceGxmTransferCopy(
		blockSize,
		numOfBlocks,
		0,
		0,
		SCE_GXM_TRANSFER_COLORKEY_NONE,
		SCE_GXM_TRANSFER_FORMAT_U8_R,
		SCE_GXM_TRANSFER_LINEAR,
		src,
		0,
		0,
		blockSize,
		SCE_GXM_TRANSFER_FORMAT_U8_R,
		SCE_GXM_TRANSFER_LINEAR,
		dst,
		0,
		0,
		blockSize,
		NULL,
		0,
		&async_notify);

	return dst;
}