#include "log.h"
#define STRLEN 64
#define CTXLEN 512
#define DISLEN 1024

#define MESGFILE "./messages.log" /* messages��־�����ڿ�ִ���ļ�ͬ��Ŀ¼�� */
char g_displayBuf[DISLEN] = {0};
FILE *g_mesg = NULL;

/* ��ʼ����־ģ��,���messages�ļ��Ĵ��� */
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

/* ��־ģ���˳��ӿڣ����messages�ļ�����ر� */
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

/* Ϊ��־��Ϣ��ӡ��ȡϵͳ�ı���ʱ��,ʹ��ϵͳtime�ӿ�ͨ��localtimeת���õ� */
void LOG_GetTime(void)
{
    char *tmpStr = NULL;
    time_t t;
    struct tm * lt = NULL;
    tmpStr = g_displayBuf;
    tmpStr += strlen(g_displayBuf);
    time (&t); /* ��ȡUnixʱ��� */
    lt = localtime (&t); /* תΪʱ��ṹ */
    sprintf(tmpStr, "[%d-%d-%d %d:%d:%d]",
        lt->tm_year + 1900, /* ���սӿ����, ��ʼ��ݴ�1900��ʼ */
        lt->tm_mon + 1,
        lt->tm_mday,
        lt->tm_hour,
        lt->tm_min,
        lt->tm_sec);
    return;
}

/* ����ָ������־����(DEBUG INFO WARN ERRO)���и�ʽʶ�� */
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

/* ��¼��־ */
void INTF_LOG_RecordInfo(int level, ...)
{
    char *fileNameTmp = NULL;
    char fileName[STRLEN] = {0};
    char funcName[STRLEN] = {0};
    int lineNumber;
    char context[CTXLEN] = {0};
    char *tmpStr = NULL;
    char *format = NULL;
    
    va_list ap; /* ��������б� */
    va_start(ap, level); /* ��ʼ�������б� */
    fileNameTmp = va_arg(ap, char*); /* ��ȡ��ӡ��־�����ļ� */
    if (INTF_MISC_GetStringByCharacter(fileNameTmp, strlen(fileNameTmp), MISC_TURNLEFT,
        '/', fileName) != DFW_SUCCESS) {
        strcpy(fileName, fileNameTmp); 
    }
    strcpy(funcName, va_arg(ap, char*)); /* ��ȡ��ӡ��־�������� */
    lineNumber = va_arg(ap, int);  /* ��ȡ��ӡ��־�������� */
    format = va_arg(ap, char*);
    vsprintf(context, format, ap); /* ��ȡ��ӡ��־ʵ������ʵ�� */
    
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
    va_end(ap); /* �رղ����б� */
    return;
}

