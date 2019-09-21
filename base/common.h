#ifndef COMMON_H
#define COMMON_H
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include "log.h"
#define MODULENAME 32
#define MUDULETOTALNUM 1
#define MAXLEN 128
#define VERSION "0.0.0" /* 新增功能版本加1.0.0,功能bug修复版本加0.1.0,代码重构版本加0.0.1 */
#define VERSIONTIME "2019-08-08" /* 版本最后修改时间 */
enum {
    DFW_FAILED,
    DFW_SUCCESS,
    DFW_INVALID
} StatusType;

#define DFW_TRUE 1
#define DFW_FALSE 0
#endif