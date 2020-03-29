/*
 * 本文件主要存放炸弹道具实现的对外接口
 * 创建时间：2020.03.25
 */
#ifndef __ELM_BOMB_H__
#define __ELM_BOMB_H__
#include "common.h"
typedef struct {
    char type;    // 炸弹类型
    int range;    // 炸弹攻击的有效范围
    int owner;    // 炸弹的拥有者编号
    void (*BombHandle)(void *ctx); // 炸弹需要的一些私有处理操作
} ELM_Bomb;

typedef struct {
    int type;
    void (*BombHandle)(void *ctx);
} ELM_BombOps;

#endif

