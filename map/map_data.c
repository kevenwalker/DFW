#include "map_data.h"
MapInfo g_defaultMap = {'#', 20, 6, NULL, 0};

void INTF_MAP_CreatePosition(void)
{
    int x;
    int y;
    int i = 0;
    int totalPos;
    ELM_Postion *mapPosition = NULL;
    totalPos = g_defaultMap.length * 2 + (g_defaultMap.width - 2) * 2;

    mapPosition = (ELM_Postion*)malloc(sizeof(ELM_Postion) * totalPos);
    if (mapPosition == NULL) {
        LOG_TRESS(TRC_LEVEL_ERROR, "Alloc the map postition failed.\n");
        return;
    }
    memset(mapPosition, 0, totalPos);
    /* 地图第一行 */
    for (x = 0, y = 0; y < g_defaultMap.length; y++) {
        mapPosition[i].xLayout = x;
        mapPosition[i].yLayout = y;
        i++;
    }

    /* 地图右侧跑道 */
    for (x = 1, y = g_defaultMap.length - 1; x < g_defaultMap.width - 1; x++) {
        mapPosition[i].xLayout = x;
        mapPosition[i].yLayout = y;
        i++;
    }

    /* 地图最后一行 */
    for (x = g_defaultMap.width - 1, y = g_defaultMap.length - 1; y >= 0; y--) {
        mapPosition[i].xLayout = x;
        mapPosition[i].yLayout = y;
        i++;
    }

    /* 地图左侧跑道 */
    for (x = g_defaultMap.width - 2, y = 0; x >= 0; x--) {
        mapPosition[i].xLayout = x;
        mapPosition[i].yLayout = y;
        i++;
    }
    
    g_defaultMap.Position = mapPosition;
    g_defaultMap.numPos = totalPos;
    LOG_TRESS(TRC_LEVEL_INFO, "Configure the map position num %d size %d.\n",
        totalPos, totalPos * sizeof(ELM_Postion));
    return;
}

/* 获取地图数据 */
MapInfo* INTF_MAP_getMap(void)
{
    return &g_defaultMap;
}

/* 获取二维地图坐标的一维坐标索引 */
int MAP_GetPositionIndex(ELM_Postion *curPos)
{
    int i;
    assert(curPos != NULL);
    assert(g_defaultMap.Position != NULL);
    for (i = 0; i < g_defaultMap.numPos; i++) {
        if (g_defaultMap.Position[i].xLayout == curPos->xLayout &&
            g_defaultMap.Position[i].yLayout == curPos->yLayout) {
            return i;
        }
    }
    // TODO:这个分支理论上走不到，即使走到了默认是0索引坐标
    return 0;
}

/* 通过给定的位移获取下一个地图坐标数据 */
int INTF_MAP_GetPosition(ELM_Postion *curPos, int step, ELM_Postion *dstPos)
{
    int index;
    if (curPos == NULL || dstPos == NULL) {
        return DFW_FAILED;
    }
    index = MAP_GetPositionIndex(curPos);
    LOG_TRESS(TRC_LEVEL_DEBUG, "Get the index{%d}, pos{%d,%d}.\n", index, curPos->xLayout, curPos->yLayout);
    if (step < 0 && index + step < 0) {
        index = (index + step + g_defaultMap.numPos) % g_defaultMap.numPos;
    } else {
        index = (index + step) % g_defaultMap.numPos;
    }
    dstPos->xLayout = g_defaultMap.Position[index].xLayout;
    dstPos->yLayout = g_defaultMap.Position[index].yLayout;
    return DFW_SUCCESS;
}

void INTF_MAP_DisplayDataInfo(void)
{
    int i;
    int j = 1;
    printf("Map Type: %c\n", g_defaultMap.type);
    printf("Map Size: width %d length %d\n", g_defaultMap.width, g_defaultMap.length);
    printf("Map Total Position: %d\n", g_defaultMap.numPos);
    printf("Map Position: [index]{xLayout,yLayout}\n");
    for (i = 0; i < g_defaultMap.numPos; i++) {
        if ((j % 6) == 0) { /* 每行打印6组坐标 */
            printf("[%2d]{%2d,%2d}\n", i, g_defaultMap.Position[i].xLayout, g_defaultMap.Position[i].yLayout);
        } else {
            printf("[%2d]{%2d,%2d} ", i, g_defaultMap.Position[i].xLayout, g_defaultMap.Position[i].yLayout);
        }
        j++;
    }
    printf("\n");
    return;
}


