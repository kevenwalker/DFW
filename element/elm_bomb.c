/*
 * 本文件主要实现炸弹道具的执行动作
 * 创建时间：2020.03.25
 */
#include "elm_bomb.h"
#define RANGE_COMMON 4
#define RANGE_MINOR  8
#define RANGE_MAJOR 16
#define RANGE_SUPER (-1)

/* 将目标玩家的当前位置后移4个位置，如果移动的4个位置中含有原点坐标，则移动到原点 */
static int ELM_BombCommonHandle(void *ctx)
{
    ELM_Player *player = NULL;

    player = (ELM_Player *)ctx;
    if (player == NULL) {
        LOG_TRESS(TRC_LEVEL_ERROR, "the input parameter is invalid.\n");
        return DFW_FALSE;
    }
    if (player->index - RANGE_COMMON < 0) {
        player->index = 0;
    } else {
        player->index -= RANGE_COMMON;
    }
    return DFW_SUCCESS;
}

ELM_BombOps g_BombHandles[] = {
    {BOMB_COMMON, ELM_BombCommonHandle},
    {BOMB_MINOR, NULL},
    {BOMB_MAJOR, NULL},
    {BOMB_SUPER, NULL},
};

void *ELM_BombGetHandle(int type)
{
    int i;
    for (i = 0; i < sizeof(g_BombHandles) / sizeof(ELM_BombOps); i++) {
        if (type == g_BombHandles[i].type) {
            return g_BombHandles[i].BombHandle;
        }
    }
    LOG_TRESS(TRC_LEVEL_ERROR, "can not find any handle for bomb with type{%d}.\n", type);
    return NULL;
}


