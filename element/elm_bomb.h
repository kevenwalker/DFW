/*
 * ���ļ���Ҫ���ը������ʵ�ֵĶ���ӿ�
 * ����ʱ�䣺2020.03.25
 */
#ifndef __ELM_BOMB_H__
#define __ELM_BOMB_H__
#include "common.h"
typedef struct {
    char type;    // ը������
    int range;    // ը����������Ч��Χ
    int owner;    // ը����ӵ���߱��
    void (*BombHandle)(void *ctx); // ը����Ҫ��һЩ˽�д������
} ELM_Bomb;

typedef struct {
    int type;
    void (*BombHandle)(void *ctx);
} ELM_BombOps;

#endif

