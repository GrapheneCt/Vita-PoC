/**
 * \usergroup{SceIniFileProcessor}
 * \usage{psp2/ini_file_processor.h,SceIniFileProcessor_stub,SCE_SYSMODULE_INTERNAL_INI_FILE_PROCESSOR}
 */

#ifndef _DOLCESDK_PSP2_INI_FILE_PROCESSOR_H_
#define _DOLCESDK_PSP2_INI_FILE_PROCESSOR_H_

#include <psp2/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Errors
 */
#define SCE_INI_FILE_PROCESSOR_ERROR_MODE              0x80840002
#define SCE_INI_FILE_PROCESSOR_ERROR_EOF               0x80840005
#define SCE_INI_FILE_PROCESSOR_ERROR_KEY_NOT_FOUND     0x80840010

#define SCE_INI_FILE_PROCESSOR_PARSE_COMPLETED         0x00840001

typedef void*(*SceIniFileProcessorAllocFunc)(SceSize size);

typedef void(*SceIniFileProcessorFreeFunc)(void* ptr);

typedef struct SceIniFileProcessorMemCallbacks {
	SceIniFileProcessorAllocFunc allocFunc;
	SceIniFileProcessorFreeFunc freeFunc;
} SceIniFileProcessorMemCallbacks;

typedef struct SceIniFileProcessorParam {
	int a1; //size or mode? usually 0, seen: 0x1000, 0x2000
	int a2; //size or mode? usually 0, seen: 0x1000, 0x2000
	SceIniFileProcessorMemCallbacks* pMemCallbacks; //can be NULL
	int a4;
} SceIniFileProcessorParam;

/**
 * Initialize INI file processor library
 *
 * @param[in] context - context buffer, size must be 8
 *
 */
void* sceIniFileProcessorCreateContext1(void* context); //SceIniFileProcessor_A0F71A2C

/**
 * Initialize INI file processor library
 *
 * @param[in] context - context buffer, size must be 8
 *
 */
void* sceIniFileProcessorCreateContext2(void* context); //SceIniFileProcessor_746B194F

/**
 * Initialize INI file processor param struct
 *
 * @param[in] param - ::SceIniFileProcessorParam to initialize
 *
 */
void sceIniFileProcessorInitializeParam1(SceIniFileProcessorParam* param); //SceIniFileProcessor_14146AF5

/**
 * Initialize INI file processor param struct
 *
 * @param[in] param - ::SceIniFileProcessorParam to initialize
 *
 */
void sceIniFileProcessorInitializeParam2(SceIniFileProcessorParam* param); //SceIniFileProcessor_9F8FC03F

/**
 * Create INI file processor instance to work with file
 *
 * @param[in] context - context buffer
 * @param[in] param - ::SceIniFileProcessorParam
 *
 * @return 0 on success, < 0 on error.
 *
 */
int sceIniFileProcessorCreateInstance(void* context, SceIniFileProcessorParam* param); //SceIniFileProcessor_0975D396

/**
 * Open INI memory
 *
 * @param[in] context - context buffer
 * @param[in] pMem - memery where INI data is stored
 * @param[in] memSize - size of the INI data
 *
 * @return 0 on success, < 0 on error.
 *
 */
int sceIniFileProcessorOpenMemory(void* context, void* pMem, SceSize memSize); //SceIniFileProcessor_B785FE67

/**
 * Open INI file
 *
 * @param[in] context - context buffer
 * @param[in] filePath - path to the INI file to open
 * @param[in] mode - file open mode
 * @param[in] unk - unknown, set to 0
 *
 * @return 0 on success, < 0 on error.
 *
 */
int sceIniFileProcessorOpenFile(void* context, const char* filePath, const char* mode, int unk); //SceIniFileProcessor_51B791E8

/**
 * Create new INI file and open it. If file is already present, it will be overwritten
 *
 * @param[in] context - context buffer
 * @param[in] filePath - path to the INI file to open
 * @param[in] mode - file open mode
 * @param[in] unk - unknown, set to 0
 *
 * @return 0 on success, < 0 on error.
 *
 */
int sceIniFileProcessorCreateFile(void* context, const char* filePath, const char* mode, int unk); //SceIniFileProcessor_FD8DE2F8

/**
 * Get file contents memory pointer
 *
 * @param[in] context - context buffer
 * @param[out] pMem - pointer to the memory block base where file contents are stored
 * @param[out] outMemSize - size of the memory block
 *
 * @return 0 on success, < 0 on error.
 *
 */
int sceIniFileProcessorGetIniFileMemBlock(void* context, void** pMem, SceSize* outMemSize); //SceIniFileProcessor_109B4E83

/**
 * Parse key and corresponding value, one key per call until eof
 *
 * @param[in] context - context buffer
 * @param[out] outKeyBuf - buffer to store key string
 * @param[out] outValueBuf - buffer to store value string
 * @param[in] outBufSize - size of output buffers
 *
 * @return 0 on success, < 0 on error.
 *
 */
int sceIniFileProcessorParse(void* context, char* outKeyBuf, char* outValueBuf, SceSize outBufSize); //SceIniFileProcessor_D5C7B3EE

/**
 * Reset parser to the beginning of the file
 *
 * @param[in] context - context buffer
 *
 */
void* sceIniFileProcessorResetParser(void* context); //SceIniFileProcessor_B48C756B

/**
 * Get total number of keys in the opened INI file
 *
 * @param[in] context - context buffer
 *
 * @return toatal number of keys, < 0 on error.
 *
 */
int sceIniFileProcessorGetKeyNum(void* context); //SceIniFileProcessor_D7648B61

/**
 * Find value corresponding to set key
 *
 * @param[in] context - context buffer
 * @param[in] key - key string
 * @param[out] outValueBuf - buffer to store value string if found
 * @param[in] outValueBufSize - size of outValueBuf
 *
 * @return 0 on success, < 0 on error.
 *
 */
int sceIniFileProcessorFindValueByKey(void* context, const char* key, char* outValueBuf, SceSize outValueBufSize); //SceIniFileProcessor_7F22CED1

/**
 * Set value corresponding to set key
 *
 * @param[in] context - context buffer
 * @param[in] key - key string
 * @param[in] value - value string
 *
 * @return 0 on success, < 0 on error.
 *
 */
int sceIniFileProcessorSetValueForKey(void* context, const char* key, const char* value); //SceIniFileProcessor_DBC5F9A8

/**
 * Add key and set value for it
 *
 * @param[in] context - context buffer
 * @param[in] key - key string
 * @param[in] value - value string
 *
 * @return 0 on success, < 0 on error.
 *
 */
int sceIniFileProcessorAddKey(void* context, const char* key, const char* value); //SceIniFileProcessor_703E1BAE

/**
 * Delete key and corresponding value from INI file
 *
 * @param[in] context - context buffer
 * @param[in] key - key string
 *
 * @return 0 on success, < 0 on error.
 *
 */
int sceIniFileProcessorDeleteKey(void* context, const char* key); //SceIniFileProcessor_C2B3A41C

/**
 * Clean temp files
 *
 * @param[in] context - context buffer
 *
 * @return 0 on success, < 0 on error.
 *
 */
int sceIniFileProcessorCleanup(void* context); //SceIniFileProcessor_A51840C7

/**
 * Close file
 *
 * @param[in] context - context buffer
 *
 * @return 0 on success, < 0 on error.
 *
 */
void sceIniFileProcessorCloseFile(void* context); //SceIniFileProcessor_EDFAD6B4

/**
 * Destroy INI file processor instance
 *
 * @param[in] context - context buffer
 *
 * @return 0 on success, < 0 on error.
 *
 */
void sceIniFileProcessorDestroyInstance(void* context); //SceIniFileProcessor_2BEF7391

/**
 * Destroy INI file processor instance when file could not be opened
 *
 * @param[in] context - context buffer
 *
 * @return 0 on success, < 0 on error.
 *
 */
void sceIniFileProcessorDestroyInstanceForError(void* context); //SceIniFileProcessor_48A6BDCB

/**
 * Destroy context
 *
 * @param[in] context - context buffer
 *
 * @return 0 on success, < 0 on error.
 *
 */
void sceIniFileProcessorDestroyContext1(void* context); //SceIniFileProcessor_ACEB88BD

/**
 * Destroy context
 *
 * @param[in] context - context buffer
 *
 * @return 0 on success, < 0 on error.
 *
 */
void sceIniFileProcessorDestroyContext2(void* context); //SceIniFileProcessor_7169CE28

/**
 * Destroy context and free context memory (PAF only)
 *
 * @param[in] context - context buffer
 *
 * @return 0 on success, < 0 on error.
 *
 */
void sceIniFileProcessorDestroyFreeContext(void* context); //SceIniFileProcessor_9F57E743

/**
 * Finalize INI file processor library instance
 *
 * @param[in] context - context buffer
 *
 */
inline
void sceIniFileProcessorFinalize(void* context)
{
	sceIniFileProcessorCleanup(context);
	sceIniFileProcessorCloseFile(context);
	sceIniFileProcessorDestroyInstance(context);
	sceIniFileProcessorDestroyContext1(context);
}

/**
 * Initialize INI file processor library
 *
 * @param[in] context - context buffer, size must be 8
 *
 */
inline
void* sceIniFileProcessorCreateContext(void* context)
{
	return sceIniFileProcessorCreateContext1(context);
}

/**
 * Initialize INI file processor param struct
 *
 * @param[in] param - ::SceIniFileProcessorParam to initialize
 *
 */
inline
void sceIniFileProcessorInitializeParam(SceIniFileProcessorParam* param)
{
	sceIniFileProcessorInitializeParam1(param);
}

/**
 * Destroy context
 *
 * @param[in] context - context buffer
 *
 * @return 0 on success, < 0 on error.
 *
 */
inline
void sceIniFileProcessorDestroyContext(void* context)
{
	sceIniFileProcessorDestroyContext1(context);
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _DOLCESDK_PSP2_INI_FILE_PROCESSOR_H_ */
