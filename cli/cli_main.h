/*
 * ���ļ���Ҫ�������漰��CLI���ܵ�����ڴ����߼�(����set�ࡢshow��)�������ṩ�Ľӿڡ�������
 * ����ʱ�䣺2019.07.29
 */
#ifndef __CLI_MAIN_H__
#define __CLI_MAIN_H__
#include "common.h"
#include "cli_show.h"
#include "cli_set.h"
#include "misc.h"
char g_rwBuf[MAXLEN]; /* ���������û�������ַ�������Ϣ */
int CLI_INTF_main(void);
#endif