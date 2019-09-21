/*
 * ���ļ���Ҫ���set����CLI�ľ���ʵ��
 * ����ʱ�䣺2019.07.29
 */

#include "cli_set.h"
#define PARAMLEN 20
int CLI_SetQuit(char **argv, int argc);
int CLI_SetPalyerNumbers(char **argv, int argc);
int CLI_SetPlayerSteps(char **argv, int argc);

struct CliSet {
    char *name;                 /* set���������� */
    char *helpInfo;             /* set�����������Ϣ */
    int (*fnRun)(char **, int); /* set������ִ�з��� */
} g_setHandle[] = {
    {CLI_SET_QUIT,          CLI_SET_QUIT_INFO,          CLI_SetQuit},
    {CLI_SET_PLAYER_NUMBER, CLI_SET_PLAYER_NUMBER_INFO, CLI_SetPalyerNumbers},
    {CLI_SET_PLAYER_NAME,   CLI_SET_PLAYER_NAME_INFO,   CLI_SetPlayerSteps}
};

/* ��ȡset��������ܸ��� */
int CLI_GetSetHandleCnt(void)
{
    return (int)(sizeof(g_setHandle) / sizeof(g_setHandle[0]));
}

/* ��ʾset�����������Ϣ */
void INTF_ShowSetHelpInfo()
{
    int i;
    int num = CLI_GetSetHandleCnt();
    for (i = 0; i < num; i++) {
        printf("%s", g_setHandle[i].helpInfo);
    }
}

/* set����CLI������� */
int INTF_RunSetHandle(char **argv, int argc)
{
    int i;
    if (argv == NULL || argc == 0) {
        LOG_TRESS(TRC_LEVEL_WARN, "Parameters are invalid.\n");
        return DFW_TRUE;
    }
    for (i = 0; i < CLI_GetSetHandleCnt(); i++) {
        if (strstr(argv[0], g_setHandle[i].name) != NULL) {
            return g_setHandle[i].fnRun(argv, argc);
        }
    }
    LOG_TRESS(TRC_LEVEL_DEBUG, "Can not find the excutable set CLI command.\n");
    return DFW_TRUE;
}

/* ������Ϸ������� */
int CLI_SetPalyerNumbers(char **argv, int argc)
{
    int result;
    int val;
    char cmdVal[PARAMLEN] = {0};
    
    if (argv == NULL || argc != 1) {
        LOG_TRESS(TRC_LEVEL_WARN, "Paramters are invalid.\n");
        printf("Execute the command is failed.\n");
        return DFW_TRUE;
    }
    result = INTF_MISC_GetParamByEqual(argv[0], strlen(argv[0]), MISC_BEFORE, cmdVal, PARAMLEN);
    if (result != DFW_SUCCESS) {
        printf("Execute the command is failed.\n");
        return DFW_TRUE;
    }

    if (strcmp(cmdVal, CLI_SET_PLAYER_NUMBER) != 0) {
        LOG_TRESS(TRC_LEVEL_WARN, "Can not find the cmd {%s} to excute.\n", cmdVal);
        return DFW_TRUE;
    }

    result = INTF_MISC_GetParamByEqual(argv[0], strlen(argv[0]), MISC_AFTER, cmdVal, PARAMLEN);
    if (result != DFW_SUCCESS) {
        printf("Execute the command is failed.\n");
        return DFW_TRUE;
    }

    val = atoi(cmdVal);
    if (val < 1 || val > 3) {
        printf("Input value is invalid or usage is error.\n %s", CLI_SET_PLAYER_NUMBER_INFO);
        return DFW_TRUE;
    }
    ELM_SetPlayerNum(val);
    printf("Execute the command is success.\n");
    return DFW_TRUE;
}

/* ������Ϸ�˳� */
int CLI_SetQuit(char **argv, int argc)
{
    char ch;
    do {
    printf("Do you really want to exit? [Y/N]:");
    ch = getchar();
    fflush(stdin);
    } while (ch != 'Y' && ch != 'y' && ch != 'n' && ch != 'N');

    if (ch == 'Y' || ch == 'y') {
        printf("Execute the command is success.\n");
        return DFW_FALSE;
    }
    return DFW_TRUE;
}

/* ������������������ǰ��(����)������(����)���� */
int CLI_SetPlayerSteps(char **argv, int argc)
{
    int result;
    int val;
    int i;
    char cmdVal[PARAMLEN] = {0};
    char name[PARAMLEN] = {0};
    char step[PARAMLEN] = {0};
    ListEntry *head = NULL;
    ListEntry *tmp = NULL;
    ListEntry *cur = NULL;
    ELM_Player *player = NULL;
    
    if (argv == NULL || argc <= 1) {
        LOG_TRESS(TRC_LEVEL_WARN, "Paramters are invalid.\n");
        printf("Execute the command is failed.\n");
        return DFW_TRUE;
    }
    /* ��ȡ������� */
    for (i = 0; i < argc; i++) {
        result = INTF_MISC_GetParamByEqual(argv[i], strlen(argv[i]), MISC_BEFORE, cmdVal, PARAMLEN);
        if (result != DFW_SUCCESS) {
            printf("Execute the command is failed.\n");
            return DFW_TRUE;
        }

        if (strcmp(cmdVal, CLI_SET_PLAYER_NAME) == 0) {
            result = INTF_MISC_GetParamByEqual(argv[i], strlen(argv[i]), MISC_AFTER, cmdVal, PARAMLEN);
            if (result != DFW_SUCCESS) {
                printf("Execute the command is failed.\n");
                return DFW_TRUE;
            }
            strcpy(name, cmdVal);
            memset(cmdVal, 0, PARAMLEN);
            continue;
        } else if (strcmp(cmdVal, "step") == 0) {
            result = INTF_MISC_GetParamByEqual(argv[i], strlen(argv[i]), MISC_AFTER, cmdVal, PARAMLEN);
            if (result != DFW_SUCCESS) {
                printf("Execute the command is failed.\n");
                return DFW_TRUE;
            }
            strcpy(step, cmdVal);
            memset(cmdVal, 0, PARAMLEN);
            continue;
        }
    }

    LOG_TRESS(TRC_LEVEL_DEBUG, "Get the player{%s}, step{%d}.\n", name, atoi(step));
    head = ELM_GetPlayerInfo();
    if (head == NULL) {
        LOG_TRESS(TRC_LEVEL_ERROR, "Get internal players list failed.\n");
        return DFW_FAILED;
    }
    GetElementEachOfList(head, tmp, cur) {
        player = MapTheListEntry(ELM_Player, cur, listEntry);
        if (strcmp(player->playerName, name) == 0) {
            INTF_MAP_GetPosition(&(player->pos), atoi(step), &(player->pos));
        }
    }
    printf("Execute the command is success.\n");
    return DFW_TRUE;
}


