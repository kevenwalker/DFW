/*
 * ���ļ���Ҫ���set����CLI�ľ���ʵ�ֵĶ���ӿ�
 * ����ʱ�䣺2019.07.29
 */
#ifndef __CLI_SET_H__
#define __CLI_SET_H__
#include "common.h"
#include "cli_help.h"
#include "misc.h"
#include "elm_player.h"
#include "map_data.h"
#include "cli_common.h"
int INTF_RunSetHandle(char **argv, int argc);
void INTF_ShowSetHelpInfo(void);
#endif