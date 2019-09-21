/*
 * 本文件主要存放软件运行的基本操作：初始化、模块加载、模块卸载等。
 * 创建时间：2019.07.29
 */
#include "common.h"
#include "map_base.h"
#include "cli_main.h"

/* 模块操作基本数据结构 */
typedef struct {
    char moduleName[MODULENAME];  /* 模块名字 */
    int (*moduleLoad)(void*);     /* 加载模块 */
    int (*moduleUnload)(void*);   /* 卸载模块 */
} ProcessModule; 

/* 加载主程序，当前主要实现打印欢迎界面 */
int Main_InitModule(void* ctx)
{
    printf("###############################\n");
    printf("#                             #\n");
    printf("#   Welcome to DFW software!  #\n");
    printf("#                             #\n");
    printf("###############################\n");
    return DFW_SUCCESS;
}

/* 卸载主程序 */
int Main_FiniModule(void* ctx)
{
    return DFW_SUCCESS;
}

/* 程序所有模块的加载与卸载方法定义,加载顺序按照第二列从上到下,卸载顺序按照第三列从下到上 */
ProcessModule g_moduleHandle[] = {
    {"MainModule", Main_InitModule, Main_FiniModule},
    {"LogModule", LOG_InitModule, Log_FiniModule},
    {"MapModule",  MAP_InitModule,  MAP_FiniModule}
};

/* 所有模块完成加载 */
void Main_LoadModules(void)
{
    int i;
    for (i = 0; i < (sizeof(g_moduleHandle) / sizeof(ProcessModule)); i++) {
        (void)g_moduleHandle[i].moduleLoad(NULL);
    }
    LOG_TRESS(TRC_LEVEL_DEBUG, "All modules are loaded success.\n");
    return;
}

/* 所有模块完成卸载 */
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

/* 软件运行的主功能程序 */
void Main_ShellProcess()
{
    do {
    }  while (CLI_INTF_main() != DFW_INVALID);
    LOG_TRESS(TRC_LEVEL_DEBUG, "Exit the shell Process.\n");
    return;
}

/* 主程序 */
int main()
{
    Main_LoadModules();
    Main_ShellProcess();
    Main_UnloadModules();
    return 0;
}
