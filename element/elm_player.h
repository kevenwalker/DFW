/*
 * 本文件主要存放玩家基本数据信息的对外接口
 * 创建时间：2019.08.24
 */
#ifndef __ELM_PLAYER_H__
#define __ELM_PLAYER_H__
#include "cli_help.h"
#include "common.h"
#include "misc.h"

typedef struct {
    int xLayout; /* 横坐标 */
    int yLayout; /* 纵坐标 */
} ELM_Postion;

typedef struct {
    ListEntry listEntry;
    char id[2];
    char playerName[12]; /* 玩家名字 */
    ELM_Postion pos; /* 玩家位置 */
    char maptType; /* 玩家在地图上的显示标记 */
} ELM_Player;

int ELM_GetPlayerNum(void);
void ELM_SetPlayerNum(int num);
ListEntry* ELM_GetPlayerInfo(void);
#endif