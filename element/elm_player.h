/*
 * 本文件主要存放玩家基本数据信息的对外接口
 * 创建时间：2019.08.24
 */
#ifndef __ELM_PLAYER_H__
#define __ELM_PLAYER_H__
#include "cli_help.h"
#include "common.h"
#include "misc.h"
#include "elm_bomb.h"

int ELM_GetPlayerNum(void);
void ELM_SetPlayerNum(int num);
#endif