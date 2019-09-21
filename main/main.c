/*
 * ���ļ���Ҫ���������еĻ�����������ʼ����ģ����ء�ģ��ж�صȡ�
 * ����ʱ�䣺2019.07.29
 */
#include "common.h"
#include "map_base.h"
#include "cli_main.h"

/* ģ������������ݽṹ */
typedef struct {
    char moduleName[MODULENAME];  /* ģ������ */
    int (*moduleLoad)(void*);     /* ����ģ�� */
    int (*moduleUnload)(void*);   /* ж��ģ�� */
} ProcessModule; 

/* ���������򣬵�ǰ��Ҫʵ�ִ�ӡ��ӭ���� */
int Main_InitModule(void* ctx)
{
    printf("###############################\n");
    printf("#                             #\n");
    printf("#   Welcome to DFW software!  #\n");
    printf("#                             #\n");
    printf("###############################\n");
    return DFW_SUCCESS;
}

/* ж�������� */
int Main_FiniModule(void* ctx)
{
    return DFW_SUCCESS;
}

/* ��������ģ��ļ�����ж�ط�������,����˳���յڶ��д��ϵ���,ж��˳���յ����д��µ��� */
ProcessModule g_moduleHandle[] = {
    {"MainModule", Main_InitModule, Main_FiniModule},
    {"LogModule", LOG_InitModule, Log_FiniModule},
    {"MapModule",  MAP_InitModule,  MAP_FiniModule}
};

/* ����ģ����ɼ��� */
void Main_LoadModules(void)
{
    int i;
    for (i = 0; i < (sizeof(g_moduleHandle) / sizeof(ProcessModule)); i++) {
        (void)g_moduleHandle[i].moduleLoad(NULL);
    }
    LOG_TRESS(TRC_LEVEL_DEBUG, "All modules are loaded success.\n");
    return;
}

/* ����ģ�����ж�� */
void Main_UnloadModules(void)
{
    int i;
    int cnt;
    cnt = sizeof(g_moduleHandle) / sizeof(ProcessModule);
    for (i = cnt - 1; i > 0; i--) {
        (void)g_moduleHandle[i].moduleUnload(NULL);
    }
    return;
}

/* ������е������ܳ��� */
void Main_ShellProcess()
{
    do {
    }  while (CLI_INTF_main() != DFW_INVALID);
    LOG_TRESS(TRC_LEVEL_DEBUG, "Exit the shell Process.\n");
    return;
}

/* ������ */
int main()
{
    Main_LoadModules();
    Main_ShellProcess();
    Main_UnloadModules();
    return 0;
}
