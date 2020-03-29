#ifndef _MAP_DATA_
#define _MAP_DATA_
#include "common.h"

typedef struct {
    ListEntry listEntry; /* ����MapAttributeInfo�Ľڵ� */
    char stuffName[MAXLEN]; /* Ԫ������ */
    char rank; /* Ԫ�ؼ��� */
    int range; /* Ԫ�������� */
    int mapPos; /* Ԫ���ڵ�ͼ��ԭʼһά���� */
    char avaliable; /* ��ǰԪ������Ƿ�ɻ�ȡ��1-���ԣ�0-������ */
    char display; /* �����Ƿ��ڵ�ͼ��ʾ�� 1-��ʾ��0-����ʾ */
    char disType; /* �����Ƿ��ڵ�ͼ��ʾ���ַ���ʾ */
    int (*attrHandle)(void *ctx); /* Ԫ��ʹ�÷��� */
} MapAttribute;

typedef struct {
    int attrNum; /* ��ͼԪ������Ŀ */
    ListEntry attrList; /* ��ͼԪ���б� */
} MapAttributeInfo;

typedef struct {
    char type;  /* ��ͼԪ������ */
    int length; /* ��ͼ�� */
    int width;  /* ��ͼ�� */
    ELM_Postion *Position; /* ��ͼһά���� */
    int numPos; /* ��ͼһά�������� */
    ListEntry *playerList; /* ���������������� */
    MapAttributeInfo *attrInfo; /* ��ͼԪ����Ϣ */
} MapInfo;

MapInfo* INTF_MAP_getMap(void);
void INTF_MAP_CreatePosition(void);
int INTF_MAP_GetPosition(ELM_Postion *curPos, int step, ELM_Postion *dstPos, int *posIndex);
void INTF_MAP_DisplayDataInfo(void);
MapAttributeInfo *INTF_MAP_GetAttributeInfo(void);
void INTF_MAP_FreeAttributeInfo(MapAttributeInfo *attrInfo);
void INTF_MAP_GetAttributeForPlayer(int index, ELM_PlayerPackage *package);

#endif

