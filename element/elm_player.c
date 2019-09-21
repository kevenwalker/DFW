/*
 * ���ļ���Ҫ�����һ���������Ϣ
 * ����ʱ�䣺2019.08.24
 */
#include "elm_player.h"
int ELM_gPlayerAmount = 0; /* ��Ϸ��ҵ������� */
ListEntry ELM_gAllPlayersList; /* ȫ����Ϸ��ҵ������� */


/* ��ȡ��Ϸ��ҵ������� */
int ELM_GetPlayerNum(void)
{
    return ELM_gPlayerAmount;
}

/* ��ȡ��Ϸ��������� */
ListEntry* ELM_GetPlayerInfo(void)
{
    return &ELM_gAllPlayersList;
}

/* ������Ϸ��� */
void ELM_CreatePlayer(void)
{
    int i;
    int num;
    ELM_Player *player = NULL;
    num = ELM_GetPlayerNum();
    if (num < 1 || num > 3) {
       return;
    }
    if (INTF_MISC_ListIsEmpty(&ELM_gAllPlayersList) != DFW_TRUE) {
        printf("Some players have been existed, can not create a new. If you must create, should be reset.\n");
        return;
    }
    INTF_MISC_InitlizeHeadList(&ELM_gAllPlayersList);
    LOG_TRESS(TRC_LEVEL_INFO, "get the number of players is %d.\n", num);
    for (i = 1; i <= num; i++) {
        player = (ELM_Player*)malloc(sizeof(ELM_Player));
        if (player == NULL) {
            LOG_TRESS(TRC_LEVEL_ERROR, "Create player datainfo failed.\n");
            break;
        }
        memset(player->playerName, 0, 12);
        sprintf(player->playerName, "Player%d", i);
        memset(player->id, 0, 2);
        sprintf(player->id, "%d", i);
        player->pos.xLayout = 0;
        player->pos.yLayout = 0;
        LOG_TRESS(TRC_LEVEL_INFO, "Store the play {0x%p} entrylist {0x%p}.\n", player, &(player->listEntry));
        INTF_MISC_InsertListToTail(&(player->listEntry), &ELM_gAllPlayersList);
    }
    LOG_TRESS(TRC_LEVEL_DEBUG, "There are %d players being created.\n", num);
    return;
}

/* ������Ϸ��ҵ������� */
void ELM_SetPlayerNum(int num)
{
    ELM_gPlayerAmount = num;
    LOG_TRESS(TRC_LEVEL_DEBUG, "Set <%s> %d.\n", CLI_SET_PLAYER_NUMBER, ELM_gPlayerAmount);
    ELM_CreatePlayer();
    return;
}


