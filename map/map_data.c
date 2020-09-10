#include "map_data.h"
MapInfo g_defaultMap = {'#', 20, 6, NULL, 0, NULL, NULL};
MapAttributeInfo g_defaultMapAttributeInfo;

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

int MAP_PickAttributeFromMapToPlayer(MapAttribute *attrDataInfo, ELM_PlayerPackage *package)
{
    ELM_STUFF *stuff = NULL;

    stuff = (ELM_STUFF *)INTF_Zmalloc(sizeof(ELM_STUFF));
    if (stuff == NULL) {
        LOG_TRESS(TRC_LEVEL_WARN, "Pick attribute{%s} from map to player failed.\n", attrDataInfo->stuffName);
        return DFW_FAILED;
    }
    sprintf(stuff->stuffName, "%s", attrDataInfo->stuffName);
    stuff->range = attrDataInfo->range;
    stuff->stuffHandle = attrDataInfo->attrHandle;
    INTF_MISC_InsertListToTail(&stuff->package, &package->stuffHead);
    package->tools++;
    LOG_TRESS(TRC_LEVEL_INFO, "Pick attribute{%s} from map to player success.\n", stuff->stuffName);
    return DFW_SUCCESS;
}

/* 入参index是地图的一维坐标 */
void INTF_MAP_GetAttributeForPlayer(int index, ELM_PlayerPackage *package)
{
    ListEntry *listHead = &(g_defaultMap.attrInfo->attrList);
    ListEntry *tmpList = NULL;
    ListEntry *curLits = NULL;
    MapAttribute *attrDataInfo = NULL;
    char buffer[BUFFLEN] = {0};

    GetElementEachOfList(listHead, tmpList, curLits) {
        attrDataInfo = MapTheListEntry(MapAttribute, curLits, listEntry);
        if (attrDataInfo->mapPos == index && attrDataInfo->avaliable == DFW_TRUE) {
            sprintf(buffer, "Do you want to get %s rank %d ? [Y/N]", attrDataInfo->stuffName, attrDataInfo->rank);
            if (INTF_PrintfYesOrNo(buffer) == DFW_TRUE) {
                if (MAP_PickAttributeFromMapToPlayer(attrDataInfo, package) == DFW_SUCCESS) {
                    attrDataInfo->avaliable = DFW_FALSE;
                }
            }
        }
    }
}

/* 通过给定的位移获取下一个地图坐标数据 */
int INTF_MAP_GetPosition(ELM_Postion *curPos, int step, ELM_Postion *dstPos, int *posIndex)
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
    *posIndex = index;
    return DFW_SUCCESS;
}

/* 显示地图的详细信息 */
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
    printf("Map Total of Tools: %d\n", g_defaultMap.attrInfo->attrNum);
    printf("\n");
    return;
}

MapAttributeInfo *INTF_MAP_GetAttributeInfo(void)
{
    MapAttribute *attr = NULL;
    g_defaultMapAttributeInfo.attrNum = 0;
    INTF_MISC_InitlizeHeadList(&g_defaultMapAttributeInfo.attrList);

    /* 第一个为BOMB_COMMON */
    attr = (MapAttribute *)INTF_Zmalloc(sizeof(MapAttribute));
    if (attr == NULL) {
        LOG_TRESS(TRC_LEVEL_WARN, "creat a BOMB_COMMON failed.\n")
    }
    attr->avaliable = DFW_TRUE;
    attr->display = DFW_FALSE;
    attr->mapPos = 17;
    attr->range = 4;
    attr->rank = BOMB_COMMON;
    attr->attrHandle = ELM_BombGetHandle(BOMB_COMMON);
    sprintf(attr->stuffName, "%s", "Bomb");
    INTF_MISC_InsertListToTail(&attr->listEntry, &g_defaultMapAttributeInfo.attrList);
    g_defaultMapAttributeInfo.attrNum++;

    /* 第二个为BOMB_MINOR */
    attr = (MapAttribute *)INTF_Zmalloc(sizeof(MapAttribute));
    if (attr == NULL) {
        LOG_TRESS(TRC_LEVEL_WARN, "creat a BOMB_MINOR failed.\n")
    }
    attr->avaliable = DFW_TRUE;
    attr->display = DFW_FALSE;
    attr->mapPos = 23;
    attr->range = 8;
    attr->rank = BOMB_MINOR;
    attr->attrHandle = ELM_BombGetHandle(BOMB_MINOR);
    sprintf(attr->stuffName, "%s", "Bomb");
    INTF_MISC_InsertListToTail(&attr->listEntry, &g_defaultMapAttributeInfo.attrList);
    g_defaultMapAttributeInfo.attrNum++;

    /* 第三个为BOMB_MAJOR */
    attr = (MapAttribute *)INTF_Zmalloc(sizeof(MapAttribute));
    if (attr == NULL) {
        LOG_TRESS(TRC_LEVEL_WARN, "creat a BOMB_MAJOR failed.\n")
    }
    attr->avaliable = DFW_TRUE;
    attr->display = DFW_FALSE;
    attr->mapPos = 37;
    attr->range = 16;
    attr->rank = BOMB_MAJOR;
    attr->attrHandle = ELM_BombGetHandle(BOMB_MAJOR);
    sprintf(attr->stuffName, "%s", "Bomb");
    INTF_MISC_InsertListToTail(&attr->listEntry, &g_defaultMapAttributeInfo.attrList);
    g_defaultMapAttributeInfo.attrNum++;

    /* 第四个为BOMB_SUPER */
    attr = (MapAttribute *)INTF_Zmalloc(sizeof(MapAttribute));
    if (attr == NULL) {
        LOG_TRESS(TRC_LEVEL_WARN, "creat a BOMB_SUPER failed.\n")
    }
    attr->avaliable = DFW_TRUE;
    attr->display = DFW_FALSE;
    attr->mapPos = 42;
    attr->range = -1;
    attr->rank = BOMB_SUPER;
    attr->attrHandle = ELM_BombGetHandle(BOMB_SUPER);
    sprintf(attr->stuffName, "%s", "Bomb");
    INTF_MISC_InsertListToTail(&attr->listEntry, &g_defaultMapAttributeInfo.attrList);
    g_defaultMapAttributeInfo.attrNum++;

    /* 第五个为BOMB_COMMON */
    attr = (MapAttribute *)INTF_Zmalloc(sizeof(MapAttribute));
    if (attr == NULL) {
        LOG_TRESS(TRC_LEVEL_WARN, "creat a BOMB_COMMON failed.\n")
    }
    attr->avaliable = DFW_TRUE;
    attr->display = DFW_FALSE;
    attr->mapPos = 7;
    attr->range = 4;
    attr->rank = BOMB_COMMON;
    attr->attrHandle = ELM_BombGetHandle(BOMB_COMMON);
    sprintf(attr->stuffName, "%s", "Bomb");
    INTF_MISC_InsertListToTail(&attr->listEntry, &g_defaultMapAttributeInfo.attrList);
    g_defaultMapAttributeInfo.attrNum++;

    LOG_TRESS(TRC_LEVEL_INFO, "init map attribute {%d} success.\n", g_defaultMapAttributeInfo.attrNum);
    return &g_defaultMapAttributeInfo;
}

void INTF_MAP_FreeAttributeInfo(MapAttributeInfo *attrInfo)
{
    ListEntry *headList = NULL;
    ListEntry *tmpList = NULL;
    ListEntry *cur = NULL;
    MapAttribute *node = NULL;
    if (attrInfo == NULL) {
        return;
    }

    headList = &attrInfo->attrList;
    GetElementEachOfList(headList, tmpList, cur) {
        node = MapTheListEntry(MapAttribute, cur, listEntry);
        INTF_MISC_ListDelete(&node->listEntry);
        free(node);
    }
    LOG_TRESS(TRC_LEVEL_INFO, "destory map attribute success.\n");
    return;
}


