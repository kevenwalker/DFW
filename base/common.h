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
#define VERSION "0.0.0" /* �������ܰ汾��1.0.0,����bug�޸��汾��0.1.0,�����ع��汾��0.0.1 */
#define VERSIONTIME "2019-08-08" /* �汾����޸�ʱ�� */
enum {
    DFW_FAILED,
    DFW_SUCCESS,
    DFW_INVALID
} StatusType;

#define DFW_TRUE 1
#define DFW_FALSE 0
#endif