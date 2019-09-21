/*
 * 本文件主要存放辅助实现特殊功能的方法，接口。一般杂项处理都放置于此。
 * 创建时间：2019.08.24
 */

#include "common.h"
#include "misc.h"
extern char g_rwBuf[MAXLEN];
void INTF_MISC_FreeDobulePoint(char **point, int num)
{
    int i;
    if (point == NULL) {
        return;
    }
    for (i = 0; i < num; i++) {
        if (point[i] != NULL) {
            free(point[i]);
        }
    }
    free(point);
    return;
}

/* 以symbol为分割符拆分字符串，返回字串个数 */
int MISC_GetSubStringNum(char* srcStr, int srcStrLen, char symbol)
{
    int i;
    int count = 0;
    char* start = NULL;
    char* end = NULL;
    int flag = 1;
    for (i = 0; i < srcStrLen; i++) {
        if (srcStr[i] == symbol && flag == 1) {
            continue;
        }
        flag = 0;
        if (srcStr[i] != symbol) {
            start = &srcStr[i];
            end = start;
            while(*end != symbol && *end != '\0') {
                end += 1;
            }
            i = i + end - start;
            count += 1;
        }
    }
    return count;
}

/* 输入指定的字符串，输出以空格为分割符的子串，调用者负责保证空间释放 */
void MISC_ParseString(char* srcStr, char** destStr, int subStrNUM, char symbol)
{
    int i;
    int j;
    char *start = srcStr;
    char *end = srcStr;
    if (srcStr == NULL || destStr == NULL) {
        return;
    }
    for (i = 0; i < subStrNUM; i++) {
        memset(destStr[i], 0, 32);
        for (j = start - srcStr; j < strlen(srcStr); j++) {
            if (srcStr[j] != symbol) {
                start = srcStr + j;
                end = start;
            }
            while (*end != symbol && *end != '\0') {
                end += 1;
            }
            if (end > start) {
                memcpy(destStr[i], start, end - start);
                start = end;
                LOG_TRESS(TRC_LEVEL_DEBUG, "Get sub string %s of index %d.\n", destStr[i], i);
                break;
            }
        }
    }
    LOG_TRESS(TRC_LEVEL_DEBUG, "Parse the string %s success. i{%d}\n", srcStr, i);
    return;
}

/* 将目标母串按照指定的分割符进行分割,调用者需要负责进行空间释放 */
int INTF_MISC_DivideStringBySymbol(char *srcStr, char symbol, char ***destStr, int *strNum)
{
    char **str = NULL;
    int subStrNum;
    int i;
    int j;
    subStrNum = MISC_GetSubStringNum(g_rwBuf, strlen(g_rwBuf), symbol);
    if (subStrNum <= 0) {
        return DFW_FAILED;
    }
    LOG_TRESS(TRC_LEVEL_DEBUG, "Get sub string number %d.\n", subStrNum);
    str = (char**)malloc(sizeof(char*) * subStrNum);
    if (str == NULL) {
        LOG_TRESS(TRC_LEVEL_WARN, "Alloc the source space failed.\n");
        return DFW_FAILED;
    }
    for (i = 0; i < subStrNum; i++) {
        str[i] = (char*)malloc(sizeof(char) * 32);
        if (str[i] == NULL) {
            LOG_TRESS(TRC_LEVEL_DEBUG, "Get sub string number %d.\n", subStrNum);
            INTF_MISC_FreeDobulePoint(str, i);
            LOG_TRESS(TRC_LEVEL_WARN, "Alloc the sub source space failed.\n");
            return DFW_FAILED;
        }
    }
    MISC_ParseString(g_rwBuf, str, subStrNum, symbol);
    *destStr = str;
    *strNum = subStrNum;
    return DFW_SUCCESS;
}

/* 检查源字符串中是否包含有'='并且不应出现在字符串的头和尾 */
int MISC_CheckEqualIsValidInString(char *strBuf, int bufLen)
{
    int i;
    assert(strBuf != NULL);
    assert(bufLen > 0);

    for (i = 0; i < bufLen; i++) {
        if (strBuf[i] == '=') {
            break;
        }
    }

    if (i >= bufLen) {
        LOG_TRESS(TRC_LEVEL_WARN, "There is not \'=\' in the given source string.\n");
        return DFW_FAILED;
    }

    if ((i == 0) || (i == bufLen - 1)) {
        LOG_TRESS(TRC_LEVEL_WARN, "There is not paramter before or after \'=\' within string.\n");
        return DFW_FAILED;
    }
    return DFW_SUCCESS;
}

/* 
 * 获取字符串中的参数
 * strBuf：输入源字符串。
 * outParam：输出的参数，类型是字符串
 * outParamLen：输出的参数长度
 * endflag：遍历源输入字符串识别到停止遍历的字符标记
 */
int MISC_GetParamter(char *strBuf, char *outParam, int outParamLen, char endflag)
{
    int i;
    int j = 0;
    if (strBuf == NULL || outParam == NULL) {
        LOG_TRESS(TRC_LEVEL_WARN, "Paramters are invalid.\n");
        return DFW_FAILED;
    }
    
    for (i = 0; strBuf[i] != endflag; i++) {
        if (j >= outParamLen - 1) {
            LOG_TRESS(TRC_LEVEL_WARN, "There is not enough size for output buffer.\n");
            return DFW_FAILED;
        }
        outParam[j] = strBuf[i];
        j++;
    }
    outParam[j] = '\0';
    return DFW_SUCCESS;
}

/* 
 * 获取字符串中'='前后的参数。
 * strBuf：输入源字符串。
 * bufLen：输入源字符串长度。
 * direct：方向。MISC_BEFORE-取'='前面的参数；MISC_AFTER-取'='后面的参数 
 * outParam：输出的参数，类型是字符串
 * outParamLen：输出的参数长度
 */
int INTF_MISC_GetParamByEqual(char *strBuf, int bufLen, int direct, char *outParam, int outParamLen)
{
    int i;
    int j = 0;
    if (strBuf == NULL || outParam == NULL) {
        LOG_TRESS(TRC_LEVEL_WARN, "Paramters are invalid.\n");
        return DFW_FAILED;
    }
    /* 检查源字符串中是否包含有'='并且不应出现在字符串的头和尾 */
    if (MISC_CheckEqualIsValidInString(strBuf, bufLen) != DFW_SUCCESS) {
        return DFW_FAILED;
    }

    /* 取'='前面的参数 */
    if (direct == MISC_BEFORE) {
        return MISC_GetParamter(strBuf, outParam, outParamLen, '=');
    }

    /* 取'='后面的参数 */
    if (direct == MISC_AFTER) {
        while (*strBuf != '=') {
            strBuf++;
        }
        return MISC_GetParamter(strBuf + 1, outParam, outParamLen, '\0');
    }
    LOG_TRESS(TRC_LEVEL_WARN, "Can not discern direction {%d} handle.\n", direct);
    return DFW_FAILED;
}

/* 从右至左开始按照指定的分割符分割源字符串 */
int MISC_TokenFromRightToLeft(char *srcStr, int strLen, char ctx, char *outParam)
{
    int i;
    assert(srcStr != NULL);
    assert(strLen > 0);
    assert(outParam != NULL);

    i = strLen;
    while (srcStr[i] != ctx) {
        i--;
    }

    if (i == 0) {
        LOG_TRESS(TRC_LEVEL_WARN, "There is not a \"%c\" in the source String.\n", ctx);
        return DFW_FAILED;
    }

    if (i + 1 > strLen) {
        LOG_TRESS(TRC_LEVEL_WARN, "Can not find the target string except the \"%c\".\n", ctx);
        return DFW_FAILED;
    }

    strcpy(outParam, &srcStr[i + 1]);
    return DFW_SUCCESS;
}

/* 
 * 按照指定的字符在源字符串中，根据指定的遍历方向遇到指定字符后截取源字符串
 * strBuf：输入源字符串。
 * strLen：输入源字符串长度。
 * direct：方向。MISC_TURNLEFT-字符串从右至左；MISC_TURNRIGHT-字符从左至右
 * ctx：指定的分割符
 * outParam：输出的参数，类型是字符串
 */
int INTF_MISC_GetStringByCharacter(char *srcStr, int strLen, int direct, char ctx, char *outParam)
{
    int i;
    if (srcStr == NULL || outParam == NULL) {
        LOG_TRESS(TRC_LEVEL_WARN, "Paramters are invalid.\n");
        return DFW_FAILED;
    }

    /*
     * 获取字符串中按照指定字符从右至左遇到的第一个指定字符及其右侧的所有内容
     * 例如：字符串/abc/bcd/cbd，指定字符为'/',则最终获取的字符串为cdb
     */
    if (direct == MISC_TURNLEFT) {
        return MISC_TokenFromRightToLeft(srcStr, strLen, ctx, outParam);
    }

    if (direct == MISC_TURNRIGHT) {
        // TODO: 后续需求支持
    }
    
    LOG_TRESS(TRC_LEVEL_WARN, "Can not discern direction {%d} handle.\n", direct);
    return DFW_FAILED;
}

/* 将目标元素插入到链表尾 */
void INTF_MISC_InsertListToTail(ListEntry *dst, ListEntry *head)
{
    ListEntry *tmp = NULL;
    if (dst == NULL || head == NULL) {
        LOG_TRESS(TRC_LEVEL_ERROR, "Load the dst %p insert head %p failed.\n", dst, head);
        return;
    }

    tmp = head->prev;
    tmp->next = dst;
    dst->prev = tmp;
    dst->next = head;
    head->prev = dst;
    return;
}

/* 链表初始化 */
void INTF_MISC_InitlizeHeadList(ListEntry *head)
{
    if (head == NULL) {
        return;
    }
    head->next = head;
    head->prev = head;
    return;
}

int INTF_MISC_ListIsEmpty(ListEntry *head)
{
    if (head == NULL) {
        return DFW_TRUE;
    }
    if (head->next == head->prev) {
        return DFW_TRUE;
    }
    return DFW_FALSE;
}

