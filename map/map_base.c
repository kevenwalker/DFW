#include "map_base.h"
#include "common.h"

/* 显示地图 */
void INTF_MAP_DisplayMap(MapInfo* mapInfo)
{
    int i;
    ListEntry *tmp = NULL;
    ListEntry *cur = NULL;
    ELM_Player *player = NULL;
    char **mapData = NULL;
    if (mapInfo == NULL) {
        return;
    }
    mapData = (char**)malloc(sizeof(char*) * mapInfo->width);
    for (i = 0; i < mapInfo->width; i++) {
        mapData[i] = (char*)malloc(sizeof(char) * mapInfo->length + 1);
        memset(mapData[i], ' ', mapInfo->length);
        mapData[i][mapInfo->length] = '\0';
    }

    for (i = 0; i < mapInfo->numPos; i++) {
        mapData[mapInfo->Position[i].xLayout][mapInfo->Position[i].yLayout] = mapInfo->type;
    }

    /* 在地图显示每位玩家标记 */
    GetElementEachOfList(mapInfo->playerList, tmp, cur) {
        player = MapTheListEntry(ELM_Player, cur, listEntry);
        mapData[player->pos.xLayout][player->pos.yLayout] = player->maptType;
    }

    for (i = 0; i < mapInfo->width; i++) {
        printf("%s\n", mapData[i]);
    }

    for (i = 0; i < mapInfo->width; i++) {
        free(mapData[i]);
    }
    free(mapData);
    return;
}

/* 初始化地图 */
int MAP_InitMap(MapInfo** mapInfo)
{
    assert(mapInfo != NULL);
    *mapInfo = INTF_MAP_getMap();
    if (*mapInfo == NULL) {
        return DFW_FAILED;
    }
    INTF_MAP_CreatePosition();
    return DFW_SUCCESS;
}

/* 释放地图数据 */
int MAP_ReleaseMap(MapInfo** mapInfo)
{
    assert(mapInfo != NULL);
    *mapInfo = INTF_MAP_getMap();
    if (*mapInfo == NULL) {
        return DFW_SUCCESS;
    }
    free((*mapInfo)->Position);
    (*mapInfo)->Position = NULL;
    return DFW_SUCCESS;
}

int MAP_InitModule(void* ctx)
{
    MapInfo* mapInfo = NULL;
	if (MAP_InitMap(&mapInfo) != DFW_SUCCESS || mapInfo == NULL) {
        LOG_TRESS(TRC_LEVEL_ERROR, "Init the map module is failed.\n");
        return DFW_FAILED;
	}
    mapInfo->playerList = ELM_GetPlayerInfo();
    LOG_TRESS(TRC_LEVEL_INFO, "Init the map module is success.\n");
    return DFW_SUCCESS;
}

int MAP_FiniModule(void* ctx)
{
    MapInfo* mapInfo = NULL;
    (void)MAP_ReleaseMap(&mapInfo);
    LOG_TRESS(TRC_LEVEL_INFO, "Exit the map module is success.\n");
	return DFW_SUCCESS;
}
