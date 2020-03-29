#ifndef _MAP_DATA_
#define _MAP_DATA_
#include "common.h"

typedef struct {
    ListEntry listEntry; /* 挂入MapAttributeInfo的节点 */
    char stuffName[MAXLEN]; /* 元素名称 */
    char rank; /* 元素级别 */
    int range; /* 元素作用域 */
    int mapPos; /* 元素在地图的原始一维坐标 */
    char avaliable; /* 当前元素玩家是否可获取，1-可以，0-不可以 */
    char display; /* 属性是否在地图显示， 1-显示，0-不显示 */
    char disType; /* 属性是否在地图显示的字符表示 */
    int (*attrHandle)(void *ctx); /* 元素使用方法 */
} MapAttribute;

typedef struct {
    int attrNum; /* 地图元素总数目 */
    ListEntry attrList; /* 地图元素列表 */
} MapAttributeInfo;

typedef struct {
    char type;  /* 地图元素类型 */
    int length; /* 地图宽 */
    int width;  /* 地图高 */
    ELM_Postion *Position; /* 地图一维坐标 */
    int numPos; /* 地图一维坐标总数 */
    ListEntry *playerList; /* 玩家总链表数据入口 */
    MapAttributeInfo *attrInfo; /* 地图元素信息 */
} MapInfo;

MapInfo* INTF_MAP_getMap(void);
void INTF_MAP_CreatePosition(void);
int INTF_MAP_GetPosition(ELM_Postion *curPos, int step, ELM_Postion *dstPos, int *posIndex);
void INTF_MAP_DisplayDataInfo(void);
MapAttributeInfo *INTF_MAP_GetAttributeInfo(void);
void INTF_MAP_FreeAttributeInfo(MapAttributeInfo *attrInfo);
void INTF_MAP_GetAttributeForPlayer(int index, ELM_PlayerPackage *package);

#endif

