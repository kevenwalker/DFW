/*
 * 本文件主要存放软件涉及的CLI功能的总入口处理逻辑(包括set类、show类)，向外提供的接口、参数等
 * 创建时间：2019.07.29
 */
#ifndef __CLI_MAIN_H__
#define __CLI_MAIN_H__
#include "common.h"
#include "cli_show.h"
#include "cli_set.h"
#include "misc.h"
char g_rwBuf[MAXLEN]; /* 接收来自用户输入的字符数据信息 */
int CLI_INTF_main(void);
#endif