/*
 * 本文件主要实现炸弹道具的执行动作
 * 创建时间：2020.03.25
 */
#include "elm_bomb.h"
#define RANGE_COMMON 4
#define RANGE_MINOR  8
#define RANGE_MAJOR 16
#define RANGE_SUPER (-1)

struct ELM_BombRangeConfigure       {
    int type;
    int range;
} g_BombRangeConfig[] = {
    {BOMB_COMMON, RANGE_COMMON},
    {BOMB_MINOR, RANGE_MINOR},
    {BOMB_MAJOR, RANGE_MAJOR},
    {BOMB_SUPER, RANGE_SUPER}
};

/* 将目标玩家的当前位置后移4个位置，如果移动的4个位置中含有原点坐标，则移动到原点 */
static void ELM_BombCommonHandle(void *ctx)
{
    ELM_Player *player = NULL;

    player = (ELM_Player *)ctx;
    if (player == NULL) {
        LOG_TRESS(TRC_LEVEL_ERROR, "the input parameter is invalid.\n");
        return;
    }
    if (player->index - RANGE_COMMON < 0) {
        player->index = 0;
    } else {
        player->index -= RANGE_COMMON;
    }
    return;
}

ELM_BombOps g_BombHandles[] = {
};


