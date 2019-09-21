#ifndef _MAP_BASE_
#define _MAP_BASE_
#include "map_data.h"
int MAP_InitModule(void* ctx);
int MAP_FiniModule(void* ctx);
void INTF_MAP_DisplayMap(MapInfo* mapInfo);

#endif
