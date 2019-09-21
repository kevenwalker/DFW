#include "map_base.h"
#include "common.h"

/* 显示地图 */
void INTF_MAP_DisplayMap(MapInfo* mapInfo)
{
    int i;
    int j;
    if (mapInfo == NULL) {
        return;
    }
    for (i = 0; i < mapInfo->width; i++) {
        if (i == 0 || i == mapInfo->width - 1) {
            for (j = 0; j < mapInfo->length; j++) {
                printf("%c", mapInfo->type);
            }
            printf("\n");
        } else {
            printf("%c", mapInfo->type);
            for (j = 1; j < mapInfo->length - 1; j++) {
                printf(" ");
            }
            printf("%c\n", mapInfo->type);
        }
    }
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
