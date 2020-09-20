/**
 * \usergroup{SceTriggerUtil}
 * \usage{psp2/triggerutil.h,SceTriggerUtil_stub,SCE_SYSMODULE_TRIGGER_UTIL}
 */


#ifndef _DOLCESDK_PSP2_TRIGGER_UTIL_H_
#define _DOLCESDK_PSP2_TRIGGER_UTIL_H_

#include <psp2/kernel/clib.h>
#include <psp2/rtc.h>
#include <psp2/types.h>

#ifdef __cplusplus
extern "C" {
#endif

// NOTE1: Last two params in all of the functions are never used, probably callbacks since all sceTriggerUtil functions are non-blocking
// NOTE2: Max number of events (eventId) per application is 6

#define SCE_TRIGGER_UTIL_VERSION						0x3200000

/**
 * Days of the week for use in repeatDays member of ::SceTriggerUtilEventParamDaily
 */
typedef enum SceTriggerUtilDays {
	SCE_TRIGGER_UTIL_SUNDAY = 0x1,
	SCE_TRIGGER_UTIL_MONDAY = 0x2,
	SCE_TRIGGER_UTIL_TUESDAY = 0x4,
	SCE_TRIGGER_UTIL_WEDNESDAY = 0x8,
	SCE_TRIGGER_UTIL_THURSDAY = 0x10,
	SCE_TRIGGER_UTIL_FRIDAY = 0x20,
	SCE_TRIGGER_UTIL_SATURDAY = 0x40,
} SceTriggerUtilDays;

/**
 * Errors
 */
#define SCE_TRIGGER_UTIL_ERROR_BUSY                       0x80103600
#define SCE_TRIGGER_UTIL_ERROR_NOT_FOUND_USER             0x80103611
#define SCE_TRIGGER_UTIL_ERROR_NOT_FOUND_SYSTEM           0x80103614
#define SCE_TRIGGER_UTIL_ERROR_NOT_REGISTERED             0x80103621
#define SCE_TRIGGER_UTIL_ERROR_EVENT_TYPE_MISMATCH        0x80103624
#define SCE_TRIGGER_UTIL_ERROR_INVALID_ARG                0x80103660

typedef struct SceTriggerUtilEventParamDaily { // size is 0x50
	SceUInt32 ver;
	SceInt16 extraParam1;                      // set to 1
	SceInt16 extraParam2;                      // set to 0
	SceInt32 triggerTime;                      // POSIX time
	SceUInt16 repeatDays;                      // bitwise
	SceChar8 reserved[0x40];
} SceTriggerUtilEventParamDaily;

typedef struct SceTriggerUtilEventParamOneTime { // size is 0x54
	SceUInt32 ver;
	SceRtcTick triggerTime;                      // SceRtcTick, UTC
	SceUInt8 extraParam1;                        // set to 1
	SceUInt8 extraParam2;                        // set to 0
	SceChar8 reserved[0x44];
} SceTriggerUtilEventParamOneTime;

typedef struct SceTriggerUtilUserAppInfo { // size is 0x46A
	SceWChar16 name[0x34];
	SceChar8 iconPath[0x400];
	short unk;
} SceTriggerUtilUserAppInfo;

typedef struct SceTriggerUtilSystemAppInfo { // size is 0x602
	SceWChar16 name[0x100];
	SceChar8 iconPath[0x400];
	char reserved[0x102];
} SceTriggerUtilSystemAppInfo;

/**
 * Register application start event that will be repeated on certain days
 *
 * @param[in] titleid - title ID of application to register event for.
 * @param[in] param - event parameters.
 * @param[in] eventId - ID number of event.
 * @param[in] a4 - Unknown, set to 0.
 * @param[in] a5 - Unknown, set to 0.
 *
 * @return 0 on success, <0 otherwise.
 */
int sceTriggerUtilRegisterDailyEvent(const char* titleid, const SceTriggerUtilEventParamDaily* param, int eventId, int a4, int a5); //SceTriggerUtil_A69102DE

/**
 * Register one time application start event
 *
 * @param[in] titleid - title ID of application to register event for.
 * @param[in] param - event parameters.
 * @param[in] eventId - ID number of event.
 * @param[in] a4 - Unknown, set to 0.
 * @param[in] a5 - Unknown, set to 0.
 *
 * @return 0 on success, <0 otherwise.
 */
int sceTriggerUtilRegisterOneTimeEvent(const char* titleid, const SceTriggerUtilEventParamOneTime* param, int eventId, int a4, int a5); //SceTriggerUtil_04218D35

/**
 * Unregister daily event for caller application
 *
 * @param[in] eventId - ID number of event to unregister.
 * @param[in] a2 - Unknown, set to 0.
 * @param[in] a3 - Unknown, set to 0.
 *
 * @return 0 on success, <0 otherwise.
 */
int sceTriggerUtilUnregisterDailyEvent(int eventId, int a2, int a3); //SceTriggerUtil_58BF700F

/**
 * Unregister one time event for caller application
 *
 * @param[in] eventId - ID number of event to unregister.
 * @param[in] a2 - Unknown, set to 0.
 * @param[in] a3 - Unknown, set to 0.
 *
 * @return 0 on success, <0 otherwise.
 */
int sceTriggerUtilUnregisterOneTimeEvent(int eventId, int a2, int a3); //SceTriggerUtil_AF73E0A8

/**
 * Get value from "Settings->System->Auto-Start Settings" for caller application. Required to be 1 to use sceTriggerUtil
 *
 * @param[out] status - auto-start status. Required to be 1 to use sceTriggerUtil.
 * @param[in] a2 - Unknown, set to 0.
 * @param[in] a3 - Unknown, set to 0.
 *
 * @return 0 on success, <0 otherwise.
 */
int sceTriggerUtilGetAutoStartStatus(int* status, int a2, int a3); //SceTriggerUtil_DD2CA8BB

/**
 * Get one time event info for caller application
 *
 * @param[in] eventId - ID number of event to get information for.
 * @param[out] triggerTime - SceRtcTick, UTC
 * @param[in] a4 - Unknown, set to 0.
 * @param[in] a5 - Unknown, set to 0.
 *
 * @return 0 on success, <0 otherwise.
 */
int sceTriggerUtilGetOneTimeEventInfo(int eventId, SceRtcTick* triggerTime, int a4, int a5); //SceTriggerUtil_912434E0

/**
 * Get daily event info for caller application
 *
 * @param[in] eventId - ID number of event to get information for.
 * @param[out] param - event parameters.
 * @param[in] a5 - Unknown, set to 0.
 * @param[in] a6 - Unknown, set to 0.
 *
 * @return 0 on success, <0 otherwise.
 */
int sceTriggerUtilGetDailyEventInfo(int eventId, SceTriggerUtilEventParamDaily* param, int a5, int a6); //SceTriggerUtil_DF3ED08E

/**
 * Get info for user application that has registered sceTriggerUtil events
 *
 * @param[in] titleid - title ID of application to get info for.
 * @param[out] appInfo - application information
 * @param[in] a4 - Unknown, set to 0.
 * @param[in] a5 - Unknown, set to 0.
 *
 * @return 0 on success, <0 otherwise.
 */
int sceTriggerUtilGetUserAppInfo(const char* titleid, SceTriggerUtilUserAppInfo* appInfo, int a4, int a5); //SceTriggerUtil_0ACE6552

/**
 * Get list of user applications that has registered sceTriggerUtil events. List contains null-separated title IDs
 *
 * @param[out] titleIdBuffer - pointer to buffer to recieve title ID list. Max size is 0x1000, min size is unlimited
 * @param[out] numOfIds - number of title IDs stored in the list
 *
 * @return 0 on success, <0 otherwise.
 */
int sceTriggerUtilGetRegisteredUserTitleIdList(char* titleIdBuffer, int numOfIds); //SceTriggerUtil_E2B2D2D0

/**
 * Get info for system application that has registered sceTriggerUtil events
 *
 * @param[in] titleid - title ID of application to get info for.
 * @param[out] appInfo - application information
 * @param[in] a4 - Unknown, set to 0.
 * @param[in] a5 - Unknown, set to 0.
 *
 * @return 0 on success, <0 otherwise.
 */
int sceTriggerUtilGetSystemAppInfo(const char* titleid, SceTriggerUtilSystemAppInfo* appInfo, int a4, int a5); //SceTriggerUtil_D659CCB0

/**
 * Get list of system applications that has registered sceTriggerUtil events. List contains null-separated fake title IDs
 *
 * @param[out] titleIdBuffer - pointer to buffer to recieve fake title ID list. Max size is 0x140, min size is unlimited
 * @param[out] numOfIds - number of fake title IDs stored in the list
 *
 * @return 0 on success, <0 otherwise.
 */
int sceTriggerUtilGetRegisteredSystemTitleIdList(char* buffer, int numOfIds); //SceTriggerUtil_2A67EF8C

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _DOLCESDK_PSP2_TRIGGER_UTIL_H_ */
