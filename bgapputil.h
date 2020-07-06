/**
 * \usergroup{SceBgAppUtil}
 * \usage{psp2/bgapputil.h,SceBgAppUtil_stub,SCE_SYSMODULE_BG_APP_UTIL}
 */


#ifndef _DOLCESDK_PSP2_BGAPP_UTIL_H_
#define _DOLCESDK_PSP2_BGAPP_UTIL_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Errors
 */
#define SCE_BGAPP_UTIL_ERROR_INVALID_ARG                       0x80106501

/**
 * Start BG application (eboot2.bin)
 *
 * @param[in] mode - must be 0
 *
 * @return 0 on success, <0 otherwise.
 */
int sceBgAppUtilStartBgApp(int mode);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _DOLCESDK_PSP2_BGAPP_UTIL_H_ */
