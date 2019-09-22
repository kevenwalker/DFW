#include "common.h"
#include "cli_help.h"
#include "misc.h"
#include "map_base.h"
#include "map_data.h"
#include "cli_show.h"
#include "cli_set.h"

/* CLI_SHOW_PLAYER����Ľ������ */
#define ITEMLEN 32 /* ��Ŀ����Ԥ����32�ֽ� */

typedef struct {
    int itemTotalLen;   /* ��Ŀ�����ܳ��� */
    char item[ITEMLEN]; /* ��Ŀ���� */
    char spliter;       /* �ָ�� */
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

/* ��ȡshow��������ܸ��� */
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

/* ��ʾ��ͼ */
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

/* ��ӡָ����Ŀ����ͬ�ַ� */
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
 * ������ʾ����
 * info����Ҫ�������ʾ��ʽ��Ϣ
 * item����Ҫ��ʾ�ľ�������
 * itemLen�������ʾ���ݵ��п�
 * spliter���������п���Ҫ�����ַ�
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

/* ��ӡ��ʾ���� */
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

/* ��ӡ��ʾȫ��player��Ϣͷ */
void CLI_DisplayPlayerInfoHead(void)
{
    int width = 0;
    CLI_ItemTitle buf[3];

    /* ����ID��Ŀ */
    CLI_ConfigureInfoDisplay(&buf[0], "ID", 6, ' ');
    /* ����Name��Ŀ */
    CLI_ConfigureInfoDisplay(&buf[1], "Name", 12, ' ');
    /* ���õ�ǰλ�� */
    CLI_ConfigureInfoDisplay(&buf[2], "Current Position", 20, ' ');

    CLI_PrintInfoDisplay(buf, 3, &width);
    CLI_DisplaySpecficCharater(width, '-');
    return;
}

/* ��ʾ������ҵĻ�����Ϣ */
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

