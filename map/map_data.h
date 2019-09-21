#ifndef _MAP_DATA_
#define _MAP_DATA_
#include "Elm_player.h"
typedef struct {
    char type;  /* 地图元素类型 */
    int length; /* 地图宽 */
    int width;  /* 地图高 */
    ELM_Postion *Position; /* 地图一维坐标 */
    int numPos; /* 地图一维坐标总数 */
} MapInfo;

MapInfo* INTF_MAP_getMap(void);
void INTF_MAP_CreatePosition(void);
int INTF_MAP_GetPosition(ELM_Postion *curPos, int step, ELM_Postion *dstPos);
void INTF_MAP_DisplayDataInfo(void);

#endif

