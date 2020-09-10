/*
 * 本文件主要存放set类型CLI的具体实现的对外接口
 * 创建时间：2019.07.29
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