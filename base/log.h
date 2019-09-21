#ifndef _LOG_H_
#define _LOG_H_
#include "common.h"
#include "misc.h"
enum {
    TRC_LEVEL_DEBUG, /* ���ڵ�����Ϣ */
    TRC_LEVEL_INFO,  /* �����ճ����� */
    TRC_LEVEL_WARN,  /* �����쳣����(��Ӱ�������) */
    TRC_LEVEL_ERROR  /* �����쳣����(Ӱ�������) */
} LOG_TraceLevel;

#define LOG_TRESS(level, ...) \
    INTF_LOG_RecordInfo((level),  __FILE__, __func__, __LINE__, ##__VA_ARGS__);  

void INTF_LOG_RecordInfo(int level, ...);
int LOG_InitModule(void* ctx);
int Log_FiniModule(void* ctx);
#endif
