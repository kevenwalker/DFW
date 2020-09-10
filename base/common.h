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
#define VERSION "1.1.0" /* 新增功能版本加1.0.0,功能bug修复版本加0.1.0,代码重构版本加0.0.1 */
#define VERSIONTIME "2020-04-16" /* 版本最后修改时间 */
enum {
    DFW_FAILED,
    DFW_SUCCESS,
    DFW_INVALID
} StatusType;

#define DFW_TRUE 1
#define DFW_FALSE 0

typedef struct {
    int xLayout; /* 横坐标 */
    int yLayout; /* 纵坐标 */
} ELM_Postion;

typedef struct {
    ListEntry stuff; /* 挂入物品节点 */
    ListEntry package; /* 挂入背包节点 */
    char stuffName[MAXLEN]; /* 物品名称 */
    int range; /* 物品作用范围 */
    int (*stuffHandle)(void *ctx); /* 物品使用方法 */
} ELM_STUFF;

typedef struct {
    int tools; /* 背包物品总数目 */
    void *owner; /* 背包拥有者 */
    ListEntry stuffHead; /* 背包物品链表头 */
} ELM_PlayerPackage;

typedef struct {
    ListEntry listEntry;
    char id[2];
    char playerName[12]; /* 玩家名字 */
    ELM_Postion pos; /* 玩家位置 */
    int index;       /* 玩家在地图上的一维坐标 */
    char maptType; /* 玩家在地图上的显示标记 */
    ELM_PlayerPackage package; /* 玩家背包 */
} ELM_Player;

enum ELM_BombType {
    BOMB_COMMON,  // 作用范围 0-4
    BOMB_MINOR,    // 作用范围 0-8
    BOMB_MAJOR,   // 作用范围 0-16
    BOMB_SUPER    // 作用范围不限
};

ListEntry* ELM_GetPlayerInfo(void);
void *ELM_BombGetHandle(int type);

#endif
