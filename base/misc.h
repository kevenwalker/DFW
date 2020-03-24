/*
 * 本文件主要存放辅助实现特殊功能的方法，接口头文件。一般杂项处理对外声明的接口都放置于此。
 * 创建时间：2019.08.24
 */

#ifndef _MISC_H_
#define _MISC_H_
enum {
    MISC_BEFORE,
    MISC_AFTER,
    MISC_TURNLEFT,
    MISC_TURNRIGHT
} Direction;

typedef struct tag_ListEntry {
    struct tag_ListEntry *prev;
    struct tag_ListEntry *next;
} ListEntry;

#define GetElementEachOfList(_head, _list, _cur) \
    for ((_list) = (_head)->next, (_cur) = (_list), (_list) = (_list)->next; (_cur) != (_head); \
        (_cur) = (_list), (_list) = (_list)->next)

#define MapTheListEntry(type, entry, member) \
    ((type*)((entry) - (unsigned long long)(&((type*)(0))->member)))

void INTF_MISC_FreeDobulePoint(char **point, int num);
int INTF_MISC_DivideStringBySymbol(char *srcStr, char symbol, char ***destStr, int *strNum);
int INTF_MISC_GetParamByEqual(char *strBuf, int bufLen, int direct, char *outParam, int outParamLen);
int INTF_MISC_GetStringByCharacter(char *srcStr, int strLen, int direct, char ctx, char *outParam);
void INTF_MISC_InsertListToTail(ListEntry *dst, ListEntry *head);
void INTF_MISC_InitlizeHeadList(ListEntry *head);
int INTF_MISC_ListIsEmpty(ListEntry *head);
void INTF_MISC_ListDelete(ListEntry *head);
void *INTF_Zmalloc(int byte);



#endif
