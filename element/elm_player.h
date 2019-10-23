/*
 * ���ļ���Ҫ�����һ���������Ϣ�Ķ���ӿ�
 * ����ʱ�䣺2019.08.24
 */
#ifndef __ELM_PLAYER_H__
#define __ELM_PLAYER_H__
#include "cli_help.h"
#include "common.h"
#include "misc.h"

typedef struct {
    int xLayout; /* ������ */
    int yLayout; /* ������ */
} ELM_Postion;

typedef struct {
    ListEntry listEntry;
    char id[2];
    char playerName[12]; /* ������� */
    ELM_Postion pos; /* ���λ�� */
    char maptType; /* ����ڵ�ͼ�ϵ���ʾ��� */
} ELM_Player;

int ELM_GetPlayerNum(void);
void ELM_SetPlayerNum(int num);
ListEntry* ELM_GetPlayerInfo(void);
#endif