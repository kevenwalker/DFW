/*
 * 本文件主要存放软件涉及的CLI功能的总入口处理逻辑(包括set类、show类)，实现CLI按类型分类功能
 * 创建时间：2019.07.29
 */
 
#include "cli_main.h"

/* CLI总处理接口，会实现按照CLI类型进行分类执行 */
int CLI_Handle(void)
{
    char **argv = NULL;
    int argc;
    int result;
    if (INTF_MISC_DivideStringBySymbol(g_rwBuf, ' ', &argv, &argc) != DFW_SUCCESS) {
        return DFW_SUCCESS;
    }
    LOG_TRESS(TRC_LEVEL_DEBUG, "Get the CLI type is %s.\n", argv[0]);
    /* 分类show型CLI */
    if (strcmp("show", argv[0]) == 0) {
        if (argc < 2) {
            LOG_TRESS(TRC_LEVEL_WARN, 
                "There are 2 parameters at least for cli_show, but %d in reality.\n", argc);
            INTF_MISC_FreeDobulePoint(argv, argc);
            return DFW_SUCCESS;
        }
        INTF_RunShowHandle(argv + 1, argc - 1);
        INTF_MISC_FreeDobulePoint(argv, argc);
        return DFW_SUCCESS;
    }

    /* 分类set型CLI */
    if (strcmp("set", argv[0]) == 0) {
        if (argc < 2) {
            LOG_TRESS(TRC_LEVEL_WARN, 
                "There are 2 parameters at least for cli_set, but %d in reality.\n", argc);
            INTF_MISC_FreeDobulePoint(argv, argc);
            return DFW_SUCCESS;
        }
        result = INTF_RunSetHandle(argv + 1, argc - 1);
        INTF_MISC_FreeDobulePoint(argv, argc);
        return (result == DFW_FALSE ? DFW_INVALID : DFW_SUCCESS);
    }
    return DFW_SUCCESS;
}

/* CLI命令执行的总入口,通过轮询方式接收来自用户的命令输入 */
int CLI_INTF_main(void)
{
    memset(g_rwBuf, 0, MAXLEN);
    printf("<Command/show help>");
    fgets(g_rwBuf, MAXLEN - 1, stdin);
    return CLI_Handle();
}

