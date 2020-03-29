#include "log.h"
#define STRLEN 64
#define CTXLEN 512
#define DISLEN 1024

#define MESGFILE "./messages.log" /* messages日志保存在可执行文件同级目录下 */
char g_displayBuf[DISLEN] = {0};
FILE *g_mesg = NULL;

/* 初始化日志模块,完成messages文件的创建 */
int LOG_InitModule(void* ctx)
{
    g_mesg = fopen(MESGFILE, "a+");
    if (g_mesg == NULL) {
        printf("Get the messages file handle failed.\n");
        return DFW_FAILED;
    }
    LOG_TRESS(TRC_LEVEL_INFO, "Init the message module is success.\n");
    return DFW_SUCCESS;
}

/* 日志模块退出接口，完成messages文件句柄关闭 */
int Log_FiniModule(void* ctx)
{
    if (g_mesg == NULL) {
        printf("The messages file handle is invalid.\n");
        return DFW_FAILED;
    }
    LOG_TRESS(TRC_LEVEL_INFO, "Exit the message module is success.\n");
    fclose(g_mesg);
    return DFW_SUCCESS;
}

/* 为日志信息打印获取系统的本地时间,使用系统time接口通过localtime转换得到 */
void LOG_GetTime(void)
{
    char *tmpStr = NULL;
    time_t t;
    struct tm * lt = NULL;
    tmpStr = g_displayBuf;
    tmpStr += strlen(g_displayBuf);
    time (&t); /* 获取Unix时间戳 */
    lt = localtime (&t); /* 转为时间结构 */
    sprintf(tmpStr, "[%d-%d-%d %d:%d:%d]",
        lt->tm_year + 1900, /* 按照接口设计, 起始年份从1900开始 */
        lt->tm_mon + 1,
        lt->tm_mday,
        lt->tm_hour,
        lt->tm_min,
        lt->tm_sec);
    return;
}

/* 按照指定的日志级别(DEBUG INFO WARN ERRO)进行格式识别 */
void LOG_GetSpecificInfotUsage(int level, char *file, char *func, int line)
{
    char *tmpStr = NULL;
    assert(file != NULL);
    assert(func != NULL);

    tmpStr = g_displayBuf;
    assert (tmpStr != NULL);
    tmpStr += strlen(g_displayBuf);
    
    switch (level) {
        case 0: {
            sprintf(tmpStr, "[DBUG][%s][%s][%d]", file, func, line);
            break;
        }
        case 1: {
            sprintf(tmpStr, "[INFO][%s][%s][%d]", file, func, line);
            break;
        }
        case 2: {
            sprintf(tmpStr, "[WARN][%s][%s][%d]", file, func, line);
            break;
        }
        case 3: {
            sprintf(tmpStr, "[ERRO][%s][%s][%d]", file, func, line);
            break;
        }
        default: {
            return;
        }
    }
    return;
}

/* 记录日志 */
void INTF_LOG_RecordInfo(int level, ...)
{
    char *fileNameTmp = NULL;
    char fileName[STRLEN] = {0};
    char funcName[STRLEN] = {0};
    int lineNumber;
    char context[CTXLEN] = {0};
    char *tmpStr = NULL;
    char *format = NULL;
    
    va_list ap; /* 定义参数列表 */
    va_start(ap, level); /* 初始化参数列表 */
    fileNameTmp = va_arg(ap, char*); /* 获取打印日志所属文件 */
    if (INTF_MISC_GetStringByCharacter(fileNameTmp, strlen(fileNameTmp), MISC_TURNLEFT,
        '/', fileName) != DFW_SUCCESS) {
        strcpy(fileName, fileNameTmp); 
    }
    strcpy(funcName, va_arg(ap, char*)); /* 获取打印日志所属函数 */
    lineNumber = va_arg(ap, int);  /* 获取打印日志所在行数 */
    format = va_arg(ap, char*);
    vsprintf(context, format, ap); /* 获取打印日志实际内容实体 */
    
    memset(g_displayBuf, 0, DISLEN);
    tmpStr = g_displayBuf;
    LOG_GetTime();
    LOG_GetSpecificInfotUsage(level, fileName, funcName, lineNumber);
    tmpStr += strlen(g_displayBuf);
    sprintf(tmpStr, context, strlen(context));
    if (g_mesg != NULL) {
        fprintf(g_mesg, "%s", g_displayBuf);
        fflush(g_mesg);
    }
    va_end(ap); /* 关闭参数列表 */
    return;
}

