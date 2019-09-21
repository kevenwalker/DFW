#ifndef _LOG_H_
#define _LOG_H_
#include "common.h"
#include "misc.h"
enum {
    TRC_LEVEL_DEBUG, /* 用于调试信息 */
    TRC_LEVEL_INFO,  /* 用于日常流程 */
    TRC_LEVEL_WARN,  /* 用于异常流程(不影响程序功能) */
    TRC_LEVEL_ERROR  /* 用于异常流程(影响程序功能) */
} LOG_TraceLevel;

#define LOG_TRESS(level, ...) \
    INTF_LOG_RecordInfo((level),  __FILE__, __func__, __LINE__, ##__VA_ARGS__);  

void INTF_LOG_RecordInfo(int level, ...);
int LOG_InitModule(void* ctx);
int Log_FiniModule(void* ctx);
#endif
