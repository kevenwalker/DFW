#ifndef _MAP_DATA_
#define _MAP_DATA_
#include "Elm_player.h"
typedef struct {
    char type;  /* ��ͼԪ������ */
    int length; /* ��ͼ�� */
    int width;  /* ��ͼ�� */
    ELM_Postion *Position; /* ��ͼһά���� */
    int numPos; /* ��ͼһά�������� */
} MapInfo;

MapInfo* INTF_MAP_getMap(void);
void INTF_MAP_CreatePosition(void);
int INTF_MAP_GetPosition(ELM_Postion *curPos, int step, ELM_Postion *dstPos);
void INTF_MAP_DisplayDataInfo(void);

#endif

