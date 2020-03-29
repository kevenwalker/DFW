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

static void ELM_BombCommonHandle(void *ctx)
{
    return;
}

ELM_BombOps g_BombHandles[] = {
};


