#ifndef COMMON_H
#define COMMON_H
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include "log.h"
#include "misc.h"
#define MODULENAME 32
#define MUDULETOTALNUM 1
#define MAXLEN 128
#define BUFFLEN 256
#define VERSION "1.1.0" /* �������ܰ汾��1.0.0,����bug�޸��汾��0.1.0,�����ع��汾��0.0.1 */
#define VERSIONTIME "2020-04-16" /* �汾����޸�ʱ�� */
enum {
    DFW_FAILED,
    DFW_SUCCESS,
    DFW_INVALID
} StatusType;

#define DFW_TRUE 1
#define DFW_FALSE 0

typedef struct {
    int xLayout; /* ������ */
    int yLayout; /* ������ */
} ELM_Postion;

typedef struct {
    ListEntry stuff; /* ������Ʒ�ڵ� */
    ListEntry package; /* ���뱳���ڵ� */
    char stuffName[MAXLEN]; /* ��Ʒ���� */
    int range; /* ��Ʒ���÷�Χ */
    int (*stuffHandle)(void *ctx); /* ��Ʒʹ�÷��� */
} ELM_STUFF;

typedef struct {
    int tools; /* ������Ʒ����Ŀ */
    void *owner; /* ����ӵ���� */
    ListEntry stuffHead; /* ������Ʒ����ͷ */
} ELM_PlayerPackage;

typedef struct {
    ListEntry listEntry;
    char id[2];
    char playerName[12]; /* ������� */
    ELM_Postion pos; /* ���λ�� */
    int index;       /* ����ڵ�ͼ�ϵ�һά���� */
    char maptType; /* ����ڵ�ͼ�ϵ���ʾ��� */
    ELM_PlayerPackage package; /* ��ұ��� */
} ELM_Player;

enum ELM_BombType {
    BOMB_COMMON,  // ���÷�Χ 0-4
    BOMB_MINOR,    // ���÷�Χ 0-8
    BOMB_MAJOR,   // ���÷�Χ 0-16
    BOMB_SUPER    // ���÷�Χ����
};

ListEntry* ELM_GetPlayerInfo(void);
void *ELM_BombGetHandle(int type);

#endif
