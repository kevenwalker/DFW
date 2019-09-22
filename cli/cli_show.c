#include "common.h"
#include "cli_help.h"
#include "misc.h"
#include "map_base.h"
#include "map_data.h"
#include "cli_show.h"
#include "cli_set.h"

/* CLI_SHOW_PLAYER命令的界面设计 */
#define ITEMLEN 32 /* 条目内容预分配32字节 */

typedef struct {
    int itemTotalLen;   /* 条目名称总长度 */
    char item[ITEMLEN]; /* 条目名称 */
    char spliter;       /* 分割符 */
} CLI_ItemTitle;

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

void CLI_DisplayContext(CLI_ItemTitle *buffer, char *curPos, int *offset)
{
    int margin;
    assert(buffer != NULL);
    assert(curPos != NULL);    
    assert(offset != NULL);

    if (strlen(buffer->item) > buffer->itemTotalLen) {
        LOG_TRESS(TRC_LEVEL_WARN, "The source length {%d} is not enough for target length{%d}.\n",
            buffer->itemTotalLen, buffer->item);
        return;
    }
    if ((buffer->itemTotalLen - strlen(buffer->item)) % 2) {
       buffer->itemTotalLen += 1;
    }
    margin = (buffer->itemTotalLen - strlen(buffer->item)) / 2 - 1;
    memset(curPos, buffer->spliter, buffer->itemTotalLen);
    sprintf(curPos + margin, "%s", buffer->item);
    curPos[margin + strlen(buffer->item)] = buffer->spliter;
    curPos[buffer->itemTotalLen] = '\0';
    *offset = buffer->itemTotalLen;
    return;
}

/* 打印指定数目的相同字符 */
void CLI_DisplaySpecficCharater(int num, char charater)
{
    int i = num;
    while (i > 0) {
        printf("%c", charater);
        i--;
    }
    printf("\n");
    return;
}

/* 
 * 配置显示内容
 * info：需要输出的显示格式信息
 * item：需要显示的具体内容
 * itemLen：设计显示内容的行宽
 * spliter：不足总行宽需要填充的字符
 */
void CLI_ConfigureInfoDisplay(CLI_ItemTitle *info, char *item, int itemLen, char spliter)
{
    assert(info != NULL);
    assert(item != NULL);
    
    strcpy(info->item, item);
    info->itemTotalLen = itemLen;
    info->spliter = spliter;
    return;
}

/* 打印显示内容 */
void CLI_PrintInfoDisplay(CLI_ItemTitle *info, int cnt, int *width)
{
    char *disBuf = NULL;
    char *tmp = NULL;
    int i;
    int size = 0;
    int offset = 0;
    assert(info != NULL);

    for (i = 0; i < cnt; i++) {
        size += info[i].itemTotalLen;
    }
    size += cnt;

    disBuf = (char*)malloc(sizeof(char) * size);
    if (disBuf == NULL) {
        LOG_TRESS(TRC_LEVEL_ERROR, "Alloc the buffer for display context failed.\n");
        return;
    }

    tmp = disBuf;
    for (i = 0; i < cnt; i++) {
        CLI_DisplayContext(&info[i], tmp, &offset);
        tmp = tmp + offset;
    }
    if (width != NULL) {
        *width = tmp - disBuf;
    }
    printf("%s\n", disBuf);
    free(disBuf);
    return;
}

/* 打印显示全部player信息头 */
void CLI_DisplayPlayerInfoHead(void)
{
    int width = 0;
    CLI_ItemTitle buf[3];

    /* 配置ID条目 */
    CLI_ConfigureInfoDisplay(&buf[0], "ID", 6, ' ');
    /* 配置Name条目 */
    CLI_ConfigureInfoDisplay(&buf[1], "Name", 12, ' ');
    /* 配置当前位置 */
    CLI_ConfigureInfoDisplay(&buf[2], "Current Position", 20, ' ');

    CLI_PrintInfoDisplay(buf, 3, &width);
    CLI_DisplaySpecficCharater(width, '-');
    return;
}

/* 显示所有玩家的基本信息 */
int CLI_ShowPlayers(char **argv, int argc)
{
    int playerNum;
    ListEntry *head = NULL;
    ListEntry *tmp = NULL;
    ListEntry *cur = NULL;
    ELM_Player *player = NULL;
    int i = 0;
    CLI_ItemTitle buf[3];
    char tmpBuf[10];
    playerNum = ELM_GetPlayerNum();
    if (playerNum == 0) {
        printf("Can not find any players.\n");
        return DFW_SUCCESS;
    }
    CLI_DisplayPlayerInfoHead();
    head = ELM_GetPlayerInfo();
    if (head == NULL) {
        LOG_TRESS(TRC_LEVEL_ERROR, "Get internal players list failed.\n");
        return DFW_FAILED;
    }
    GetElementEachOfList(head, tmp, cur) {
        player = MapTheListEntry(ELM_Player, cur, listEntry);
        CLI_ConfigureInfoDisplay(&buf[0], player->id, 6, ' ');
        CLI_ConfigureInfoDisplay(&buf[1], player->playerName, 12, ' ');
        memset(tmpBuf, 0, 10);
        sprintf(tmpBuf, "%d %d", player->pos.xLayout, player->pos.yLayout);
        CLI_ConfigureInfoDisplay(&buf[2], tmpBuf, 20, ' ');
        CLI_PrintInfoDisplay(buf, 3, NULL);
    }
    return DFW_SUCCESS;
}

