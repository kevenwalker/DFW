/*
 * ���ļ���Ҫ��Ÿ���ʵ�����⹦�ܵķ������ӿڡ�һ������������ڴˡ�
 * ����ʱ�䣺2019.08.24
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

/* ��symbolΪ�ָ������ַ����������ִ����� */
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

/* ����ָ�����ַ���������Կո�Ϊ�ָ�����Ӵ��������߸���֤�ռ��ͷ� */
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

/* ��Ŀ��ĸ������ָ���ķָ�����зָ�,��������Ҫ������пռ��ͷ� */
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

/* ���Դ�ַ������Ƿ������'='���Ҳ�Ӧ�������ַ�����ͷ��β */
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
 * ��ȡ�ַ����еĲ���
 * strBuf������Դ�ַ�����
 * outParam������Ĳ������������ַ���
 * outParamLen������Ĳ�������
 * endflag������Դ�����ַ���ʶ��ֹͣ�������ַ����
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
 * ��ȡ�ַ�����'='ǰ��Ĳ�����
 * strBuf������Դ�ַ�����
 * bufLen������Դ�ַ������ȡ�
 * direct������MISC_BEFORE-ȡ'='ǰ��Ĳ�����MISC_AFTER-ȡ'='����Ĳ��� 
 * outParam������Ĳ������������ַ���
 * outParamLen������Ĳ�������
 */
int INTF_MISC_GetParamByEqual(char *strBuf, int bufLen, int direct, char *outParam, int outParamLen)
{
    int i;
    int j = 0;
    if (strBuf == NULL || outParam == NULL) {
        LOG_TRESS(TRC_LEVEL_WARN, "Paramters are invalid.\n");
        return DFW_FAILED;
    }
    /* ���Դ�ַ������Ƿ������'='���Ҳ�Ӧ�������ַ�����ͷ��β */
    if (MISC_CheckEqualIsValidInString(strBuf, bufLen) != DFW_SUCCESS) {
        return DFW_FAILED;
    }

    /* ȡ'='ǰ��Ĳ��� */
    if (direct == MISC_BEFORE) {
        return MISC_GetParamter(strBuf, outParam, outParamLen, '=');
    }

    /* ȡ'='����Ĳ��� */
    if (direct == MISC_AFTER) {
        while (*strBuf != '=') {
            strBuf++;
        }
        return MISC_GetParamter(strBuf + 1, outParam, outParamLen, '\0');
    }
    LOG_TRESS(TRC_LEVEL_WARN, "Can not discern direction {%d} handle.\n", direct);
    return DFW_FAILED;
}

/* ��������ʼ����ָ���ķָ���ָ�Դ�ַ��� */
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
 * ����ָ�����ַ���Դ�ַ����У�����ָ���ı�����������ָ���ַ����ȡԴ�ַ���
 * strBuf������Դ�ַ�����
 * strLen������Դ�ַ������ȡ�
 * direct������MISC_TURNLEFT-�ַ�����������MISC_TURNRIGHT-�ַ���������
 * ctx��ָ���ķָ��
 * outParam������Ĳ������������ַ���
 */
int INTF_MISC_GetStringByCharacter(char *srcStr, int strLen, int direct, char ctx, char *outParam)
{
    int i;
    if (srcStr == NULL || outParam == NULL) {
        LOG_TRESS(TRC_LEVEL_WARN, "Paramters are invalid.\n");
        return DFW_FAILED;
    }

    /*
     * ��ȡ�ַ����а���ָ���ַ��������������ĵ�һ��ָ���ַ������Ҳ����������
     * ���磺�ַ���/abc/bcd/cbd��ָ���ַ�Ϊ'/',�����ջ�ȡ���ַ���Ϊcdb
     */
    if (direct == MISC_TURNLEFT) {
        return MISC_TokenFromRightToLeft(srcStr, strLen, ctx, outParam);
    }

    if (direct == MISC_TURNRIGHT) {
        // TODO: ��������֧��
    }
    
    LOG_TRESS(TRC_LEVEL_WARN, "Can not discern direction {%d} handle.\n", direct);
    return DFW_FAILED;
}

/* ��Ŀ��Ԫ�ز��뵽����β */
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

/* �����ʼ�� */
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

