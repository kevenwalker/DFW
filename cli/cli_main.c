/*
 * ���ļ���Ҫ�������漰��CLI���ܵ�����ڴ����߼�(����set�ࡢshow��)��ʵ��CLI�����ͷ��๦��
 * ����ʱ�䣺2019.07.29
 */
 
#include "cli_main.h"

/* CLI�ܴ���ӿڣ���ʵ�ְ���CLI���ͽ��з���ִ�� */
int CLI_Handle(void)
{
    char **argv = NULL;
    int argc;
    int result;
    if (INTF_MISC_DivideStringBySymbol(g_rwBuf, ' ', &argv, &argc) != DFW_SUCCESS) {
        return DFW_SUCCESS;
    }
    LOG_TRESS(TRC_LEVEL_DEBUG, "Get the CLI type is %s.\n", argv[0]);
    /* ����show��CLI */
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

    /* ����set��CLI */
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

/* CLI����ִ�е������,ͨ����ѯ��ʽ���������û����������� */
int CLI_INTF_main(void)
{
    memset(g_rwBuf, 0, MAXLEN);
    printf("<Command/show help>");
    fgets(g_rwBuf, MAXLEN - 1, stdin);
    return CLI_Handle();
}

