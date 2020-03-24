#include "common.h"
#include "cli_help.h"
#include "misc.h"
#include "map_base.h"
#include "map_data.h"
#include "cli_show.h"
#include "cli_set.h"
#include "cli_common.h"

/* CLI_SHOW_PLAYER命令的界面设计 */
typedef struct {
    ListEntry listEntry;
    char context[CLI_CONTEXT_LEN]; // 显示的内容信息
    char spliter;                  // 显示需要的分隔符
    char endFlag;                  // 显示信息是否需要换行标志 1-换行；0-不换行
    int len;                       // 显示内容总的长度
    int offset;                    // 显示内容的偏移
} CLI_DisplayData;

static ListEntry g_displayContextList;

static void CLI_StartDisplay()
{
    ListEntry *headList = &g_displayContextList;
    ListEntry *tmpList = NULL;
    ListEntry *cur = NULL;
    CLI_DisplayData *node = NULL;
    char buffer[CLI_BUFFER_LEN] = {0};
    char *tmpBuf = buffer;

    GetElementEachOfList(headList, tmpList, cur) {
        node = MapTheListEntry(CLI_DisplayData, cur, listEntry);
        INTF_MISC_ListDelete(&node->listEntry);
        LOG_TRESS(TRC_LEVEL_DEBUG, "node{%s} offset{%d} len {%d} start to dispaly.\n",
                  node->context, node->offset, node->len);
        memset(tmpBuf, node->spliter, node->len);
        sprintf(tmpBuf + node->offset, "%s", node->context);
        *(tmpBuf + node->offset + strlen(node->context)) = node->spliter;
        if (node->endFlag) {
            *(tmpBuf + node->len) = '\n';
        }
        free(node);
        printf("%s", buffer);
        memset(buffer, 0, CLI_BUFFER_LEN);
        tmpBuf = buffer;
    }
    return;
}

static int CLI_PrepareContext(const char *context, int length, char spliter, char endFlag)
{
    int ctxlen = strlen(context);
    int idleLen;
    CLI_DisplayData *node = NULL;

    if (ctxlen > length) {
        LOG_TRESS(TRC_LEVEL_ERROR, "context length{%d} is too long than total length{%d}.\n", ctxlen, length);
        return DFW_FAILED;
    }
    /* 保持内容居中显示，显示总长度存在1个字符的调节 */
    idleLen = (length - ctxlen) % 2 ? (length - ctxlen) : (length - ctxlen) + 1;

    node = (CLI_DisplayData *)INTF_Zmalloc(sizeof(CLI_DisplayData));
    if (node == NULL) {
        LOG_TRESS(TRC_LEVEL_ERROR, "alloc node to display failed.\n");
        return DFW_FAILED;
    }
    memcpy(node->context, context, ctxlen);
    node->spliter = spliter;
    node->endFlag = endFlag;
    node->len = length;
    node->offset = idleLen / 2;
    INTF_MISC_InsertListToTail(&node->listEntry, &g_displayContextList);
    LOG_TRESS(TRC_LEVEL_DEBUG, "node{%s} is insert to list success.\n", node->context);
    return DFW_SUCCESS;
}

int CLI_showMap(char **argv, int argc);
int CLI_showHelp(char **argv, int argc);
int CLI_showVersion(char **argv, int argc);
int CLI_ShowPlayers(char **argv, int argc);

struct CliShow {
    char *name;
    char *helpInfo;
    int (*fnRun)(char **, int);
} g_showHandle[] = {
    {CLI_SHOW_MAP,       CLI_SHOW_MAP_HELP,      CLI_showMap},
    {CLI_SHOW_HELP,      CLI_SHOW_HELP_INFO,     CLI_showHelp},
    {CLI_SHOW_VERSION,   CLI_SHOW_VERSION_INFO,  CLI_showVersion},
    {CLI_SHOW_PLAYER,    CLI_SHOW_PLAYER_INFO,   CLI_ShowPlayers}
};

/* 获取show类命令的总个数 */
int CLI_GetShowHandleCnt(void)
{
    return (int)(sizeof(g_showHandle) / sizeof(g_showHandle[0]));
}

void INTF_RunShowHandle(char **argv, int argc)
{
    int i;
    if (argv == NULL) {
        LOG_TRESS(TRC_LEVEL_ERROR, "Get the handle of cli_show failed.\n");
        return;
    }
    for (i = 0; i < CLI_GetShowHandleCnt(); i++) {
        if (strcmp(g_showHandle[i].name, argv[0]) == 0) {
            (void)g_showHandle[i].fnRun(argv, argc);
            break;
        }
    }
    return;
}

/* 显示地图 */
int CLI_showMap(char **argv, int argc)
{
    int result;
    MapInfo *mapInfo = NULL;
    assert(argv != NULL);
    if (argc < 1 || argc > 2) {
        LOG_TRESS(TRC_LEVEL_ERROR, "Parameters are invalid.\n");
        return DFW_FAILED;
    }
    mapInfo = INTF_MAP_getMap();
    if (mapInfo == NULL) {
        LOG_TRESS(TRC_LEVEL_ERROR, "Get the mapInfo failed.\n");
        return DFW_FAILED;
    }
    INTF_MAP_DisplayMap(mapInfo);
    if (argc == 2 && strcmp(argv[1],"verbose") == 0) {
        INTF_MAP_DisplayDataInfo();
    }
    return DFW_SUCCESS;
}

int CLI_showVersion(char **argv, int argc)
{
   printf("DFW Version:%s\n", VERSION);
   printf("DFW Public :%s\n", VERSIONTIME);
   return DFW_SUCCESS;
}

int CLI_showHelp(char **argv, int argc)
{
    int i;
    int num = CLI_GetShowHandleCnt();
    for (i = 0; i < num; i++) {
        printf("%s", g_showHandle[i].helpInfo);
    }
    INTF_ShowSetHelpInfo();
    return DFW_SUCCESS;
}

/* 显示所有玩家的基本信息 */
int CLI_ShowPlayers(char **argv, int argc)
{
    int playerNum;
    ListEntry *head = NULL;
    ListEntry *tmp = NULL;
    ListEntry *cur = NULL;
    ELM_Player *player = NULL;
    char tmpBuf[10];
    playerNum = ELM_GetPlayerNum();
    if (playerNum == 0) {
        printf("Can not find any players.\n");
        return DFW_SUCCESS;
    }

    INTF_MISC_InitlizeHeadList(&g_displayContextList);
    CLI_PrepareContext("ID", 6, ' ', 0);
    CLI_PrepareContext("Name", 12, ' ', 0);
    CLI_PrepareContext("Current Position", 20, ' ', 1);
    CLI_PrepareContext("", 38, '-', 1);
    head = ELM_GetPlayerInfo();
    GetElementEachOfList(head, tmp, cur) {
        player = MapTheListEntry(ELM_Player, cur, listEntry);
        CLI_PrepareContext(player->id, 6, ' ', 0);
        CLI_PrepareContext(player->playerName, 12, ' ', 0);
        memset(tmpBuf, 0, 10);
        sprintf(tmpBuf, "%d %d", player->pos.xLayout, player->pos.yLayout);
        CLI_PrepareContext(tmpBuf, 20, ' ', 1);
    }
    CLI_PrepareContext("", 38, '-', 1);
    CLI_StartDisplay();
    return DFW_SUCCESS;
}

