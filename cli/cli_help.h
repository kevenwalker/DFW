/*
 * 本文件主要存放所有CLI涉及的帮助信息
 * 创建时间：2019.09.14
 */
#ifndef _CLI_HELP_
#define _CLI_HELP_

/* show类命令 */
#define CLI_SHOW_MAP "map"
#define CLI_SHOW_HELP "help"
#define CLI_SHOW_VERSION "version"
#define CLI_SHOW_PLAYER "player"
/* show类命令帮助 */
#define CLI_SHOW_MAP_HELP       "show map [verbose]               -- print the map data info.\n" \
                                "                                 -- verbose is optional.\n"
#define CLI_SHOW_HELP_INFO      "show help                        -- print the help info.\n"
#define CLI_SHOW_VERSION_INFO   "show version                     -- print the version.\n"
#define CLI_SHOW_PLAYER_INFO    "show player                      -- print all the players info.\n"
/* set类命令 */
#define CLI_SET_QUIT "quit"
#define CLI_SET_PLAYER_NUMBER "player_count"
#define CLI_SET_PLAYER_NAME "player_name"
/* set类命令帮助 */
#define CLI_SET_QUIT_INFO       "set quit                         -- exit the DFW software.\n"
#define CLI_SET_PLAYER_NUMBER_INFO \
                                "set player_count={cnt}           -- set the number of players.\n" \
                                "                                 -- the cnt range[1-3].\n"
#define CLI_SET_PLAYER_NAME_INFO \
                             "set player_name={name} step={cnt}-- set the specific player steps.\n"
#endif