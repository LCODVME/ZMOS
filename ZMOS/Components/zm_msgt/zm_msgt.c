/*****************************************************************
* Copyright (C) 2021 zm. All rights reserved.                    *
******************************************************************
* zm_msgt.c
*
* DESCRIPTION:
*     ZM Message Queuing Notice Transport.
* AUTHOR:
*     zm
* CREATED DATE:
*     2022/8/27
* REVISION:
*     v0.1
*
* MODIFICATION HISTORY
* --------------------
* $Log:$
*
*****************************************************************/
 
/*************************************************************************************************************************
 *                                                       INCLUDES                                                        *
 *************************************************************************************************************************/
#include "ZMOS.h"
#include <string.h>
#include "zm_msgt.h"
/*************************************************************************************************************************
 *                                                        MACROS                                                         *
 *************************************************************************************************************************/
#define ZM_MSGT_MSG_GC_EVENT          BS(3)
/*************************************************************************************************************************
 *                                                      CONSTANTS                                                        *
 *************************************************************************************************************************/
 
/*************************************************************************************************************************
 *                                                       TYPEDEFS                                                        *
 *************************************************************************************************************************/


typedef zm_uint8_t scripCnt_t;

typedef struct topicNode topicNode_t;
typedef struct msgNode msgNode_t;
typedef struct noticeNode noticeNode_t;

struct topicNode
{
    msgtTopic_t topic;
    msgNode_t *msgNodeList;
    noticeNode_t *noticeNodeList;
    topicNode_t *next;
};

struct msgNode
{
    scripCnt_t count;
    zm_msgLen_t msgLen; 
    zm_uint8_t *msg;
    msgNode_t *next;
};

struct noticeNode
{
    msgNode_t *msgNode;
    zm_msgtNoticeFun_t noticFunc;
    noticeNode_t *next;
};

typedef struct msgtClient
{
    msgtTopic_t topic;
    noticeNode_t *noticeNode;
    struct msgtClient *next;
}msgtClient_t;
/*************************************************************************************************************************
 *                                                   GLOBAL VARIABLES                                                    *
 *************************************************************************************************************************/

/*************************************************************************************************************************
 *                                                  EXTERNAL VARIABLES                                                   *
 *************************************************************************************************************************/
 
/*************************************************************************************************************************
 *                                                    LOCAL VARIABLES                                                    *
 *************************************************************************************************************************/
static topicNode_t *topicNodeListHead = NULL;
static zmos_taskHandle_t msgtTaskHandle;
/*************************************************************************************************************************
 *                                                 FUNCTION DECLARATIONS                                                 *
 *************************************************************************************************************************/
static uTaskEvent_t zm_msgtPorcessEvent(uTaskEvent_t events);
static void zm_msgtMsgGc(void);
/*************************************************************************************************************************
 *                                                   PUBLIC FUNCTIONS                                                    *
 *************************************************************************************************************************/
 
/*************************************************************************************************************************
 *                                                    LOCAL FUNCTIONS                                                    *
 *************************************************************************************************************************/

msgtReslt_t zm_msgtPublishMsg(msgtTopic_t topic, zm_uint8_t *msg, zm_msgLen_t msgLen)
{
    if(!msg || !msgLen) return MSGT_ERR_PARAM;
    topicNode_t *srchNode = topicNodeListHead;
    
    while(srchNode)
    {
        if(srchNode->topic == topic)
        {
            msgNode_t *newMsgNode = (msgNode_t *)zmos_malloc(sizeof(msgNode_t));
            if(newMsgNode)
            {
                newMsgNode->msg = (zm_uint8_t *)zmos_malloc(msgLen);
                if(newMsgNode->msg == NULL)
                {
                    zmos_free(newMsgNode);
                    return MSGT_NO_SPACE;
                }
                memcpy(newMsgNode->msg, msg, msgLen);
                newMsgNode->msgLen = msgLen;
                newMsgNode->count = 0;
                newMsgNode->next = NULL;
                while(srchNode->msgNodeList->next != NULL) srchNode->msgNodeList = srchNode->msgNodeList->next;
                srchNode->msgNodeList->next = newMsgNode;
                while(srchNode->noticeNodeList)
                {
                    if(srchNode->noticeNodeList->msgNode == NULL)
                    {
                        srchNode->noticeNodeList->msgNode = newMsgNode;
                        if(srchNode->noticeNodeList->noticFunc)
                            srchNode->noticeNodeList->noticFunc(topic, msgLen);
                    }
                    newMsgNode->count++;
                }
                return MSGT_OK;
            }
            else return MSGT_NO_SPACE;
        }
        else if(srchNode->topic > topic)
        {
            break;
        }
        srchNode = srchNode->next;
    }
    return MSGT_NO_TOPIC;
}

msgtReslt_t zm_msgtSubscribeTopic(zm_msgtClient_t *client, msgtTopic_t topic, zm_msgtNoticeFun_t noticFunc)
{
    topicNode_t *srchNode = topicNodeListHead;
    topicNode_t *prevNode = NULL;
    
    while(srchNode)
    {
        if(srchNode->topic >= topic)
        {
            break;
        }
        prevNode = srchNode;
        srchNode = srchNode->next;
    }
    if(!srchNode || srchNode->topic > topic)
    {
        topicNode_t *newNode = (topicNode_t *)zmos_malloc(sizeof(topicNode_t));
        if(newNode)
        {
            newNode->noticeNodeList = (noticeNode_t *)zmos_malloc(sizeof(noticeNode_t));
            if(!newNode->noticeNodeList)
            {
                zmos_free(newNode);
                return MSGT_NO_SPACE;
            }
            newNode->topic = topic;
            newNode->msgNodeList = NULL;
            newNode->next = NULL;
            newNode->noticeNodeList->msgNode = NULL;
            newNode->noticeNodeList->noticFunc = noticFunc;
            newNode->noticeNodeList->next = NULL;
            if(client->node)
            {
                msgtClient_t *pClient = (msgtClient_t *)client->node;
                while(pClient->next != NULL) pClient = pClient->next;
                pClient->topic = topic;
                pClient->noticeNode = newNode->noticeNodeList;
                pClient->next = NULL;
            }
            else
            {
                msgtClient_t *pClient = (msgtClient_t *)zmos_malloc(sizeof(msgtClient_t));
                if(!pClient)
                {
                    zmos_free(newNode->noticeNodeList);
                    zmos_free(newNode);
                    return MSGT_NO_SPACE;
                }
                pClient->topic = topic;
                pClient->noticeNode = newNode->noticeNodeList;
                pClient->next = NULL;
                client->node = pClient;
            }
            if(prevNode)
            {
                newNode->next = prevNode->next;
                prevNode->next = newNode;
            }
            else
            {
                topicNodeListHead = newNode;
            }
        }
        else return MSGT_NO_SPACE;
    }
    else
    {
        msgtClient_t *pClient;
        msgtClient_t *prevClient = NULL;
        if(client->node)
        {
            pClient = (msgtClient_t *)client->node;
            while(pClient)
            {
                if(pClient->topic == topic)
                {
                    return MSGT_OK;
                }
                prevClient = pClient;
                pClient = pClient->next;
            }
        }
        noticeNode_t *pNoticeNode = (noticeNode_t *)zmos_malloc(sizeof(noticeNode_t));
        if(pNoticeNode)
        {
            pClient = (msgtClient_t *)zmos_malloc(sizeof(msgtClient_t));
            if(pClient)
            {
                pClient->topic = topic;
                pClient->noticeNode = pNoticeNode;
                pClient->next = NULL;
                if(prevClient) prevClient->next = pClient;
                else client->node = pClient;
            }
            else
            {
                zmos_free(pNoticeNode);
                return MSGT_NO_SPACE;
            }
            pNoticeNode->next = NULL;
            pNoticeNode->msgNode = NULL;
            pNoticeNode->noticFunc = noticFunc;
            noticeNode_t *srcNoticeNode = srchNode->noticeNodeList;
            while(srcNoticeNode->next != NULL) srcNoticeNode = srcNoticeNode->next;
            srcNoticeNode->next = pNoticeNode;
        }
        else return MSGT_NO_SPACE;
    }
    return MSGT_OK;
}

msgtReslt_t zm_msgtUnsubscribeTopic(zm_msgtClient_t *client, msgtTopic_t topic)
{
    topicNode_t *srchNode = topicNodeListHead;
    topicNode_t *prevTopicNode = NULL;
    msgtClient_t *pClient = (msgtClient_t *)client->node;
    msgtClient_t *prevClient = NULL;
    while(srchNode && pClient)
    {
        if(srchNode->topic == topic && pClient->topic == topic)
        {
            if(prevClient)
            {
                prevClient->next = pClient->next;
            }
            else
            {
                client->node = pClient->next;
            }
            noticeNode_t *delNoticeNode = srchNode->noticeNodeList;
            noticeNode_t *prevNoticeNode = NULL;
            while(delNoticeNode)
            {
                if(delNoticeNode == pClient->noticeNode)
                {
                    if(prevNoticeNode)
                    {
                        prevNoticeNode->next = delNoticeNode->next;
                    }
                    else
                    {
                        srchNode->noticeNodeList = srchNode->noticeNodeList->next;
                        if(srchNode->noticeNodeList->next == NULL)
                        {
                            msgNode_t *delMsgNode;
                            while(srchNode->msgNodeList)
                            {
                                delMsgNode = srchNode->msgNodeList;
                                srchNode->msgNodeList = srchNode->msgNodeList->next;
                                if(delMsgNode->msg) zmos_free(delMsgNode->msg);
                                zmos_free(delMsgNode);
                            }
                            if(prevTopicNode)
                            {
                                prevTopicNode->next = srchNode->next;
                            }
                            else topicNodeListHead = topicNodeListHead->next;
                            zmos_free(srchNode);
                        }
                    }
                    zmos_free(delNoticeNode);
                    break;
                }
                prevNoticeNode = delNoticeNode;
                delNoticeNode = delNoticeNode->next;
            }
            zmos_free(pClient);
            break;
        }
        prevTopicNode = srchNode;
        srchNode = srchNode->next;
        prevClient = pClient;
        pClient = pClient->next;
    }
    return MSGT_OK;
}

msgtMsg_t zm_msgtGetMsg(zm_msgtClient_t client)
{
    msgtMsg_t pMsg;
    pMsg.len = 0;
    pMsg.msg = NULL;
    msgtClient_t *pClient = (msgtClient_t *)client.node;
    while(pClient)
    {
        if(pClient->noticeNode->msgNode)
        {
            pMsg.topic = pClient->topic;
            pMsg.len = pClient->noticeNode->msgNode->msgLen;
            pMsg.msg = pClient->noticeNode->msgNode->msg;
            pClient->noticeNode->msgNode->count--;
            pClient->noticeNode->msgNode = pClient->noticeNode->msgNode->next;
            zmos_setTaskEvent(msgtTaskHandle, ZM_MSGT_MSG_GC_EVENT);
            break;
        }
    }
    return pMsg;
}

msgtMsg_t zm_msgtGetTopicMsg(zm_msgtClient_t client, msgtTopic_t topic)
{
    msgtMsg_t pMsg;
    pMsg.len = 0;
    pMsg.msg = NULL;
    msgtClient_t *pClient = (msgtClient_t *)client.node;
    while(pClient)
    {
        if(pClient->topic == topic)
        {
            if(pClient->noticeNode->msgNode)
            {
                pMsg.topic = pClient->topic;
                pMsg.len = pClient->noticeNode->msgNode->msgLen;
                pMsg.msg = pClient->noticeNode->msgNode->msg;
                pClient->noticeNode->msgNode->count--;
                pClient->noticeNode->msgNode = pClient->noticeNode->msgNode->next;
                zmos_setTaskEvent(msgtTaskHandle, ZM_MSGT_MSG_GC_EVENT);
            }
            break;
        }
    }
    return pMsg;
}

void zm_msgtInit(void)
{
    zmos_taskThreadRegister(&msgtTaskHandle, zm_msgtPorcessEvent);
}

static uTaskEvent_t zm_msgtPorcessEvent(uTaskEvent_t events)
{
    if(events & ZM_MSGT_MSG_GC_EVENT)
    {
        zm_msgtMsgGc();
        return events ^ ZM_MSGT_MSG_GC_EVENT;
    }
    return 0;
}

static void zm_msgtMsgGc(void)
{
    topicNode_t *srchNode = topicNodeListHead;
    msgNode_t *prevMsgNode;
    msgNode_t *delMsgNode;
    while(srchNode)
    {
        prevMsgNode = NULL;
        while(srchNode->msgNodeList)
        {
            if(srchNode->msgNodeList->count == 0)
            {
                delMsgNode = srchNode->msgNodeList;
                if(prevMsgNode)
                {
                    prevMsgNode->next = delMsgNode->next;
                }
                else
                {
                    srchNode->msgNodeList = srchNode->msgNodeList->next;
                }
                zmos_free(delMsgNode);
            }
            prevMsgNode = srchNode->msgNodeList;
            srchNode->msgNodeList = srchNode->msgNodeList->next;
        }
        srchNode = srchNode->next;
    }
}

#if (defined ZMOS_INIT_SECTION) && (ZMOS_INIT_SECTION)
ZNOS_INIT_REGISTER(zm_msgtInit);
#endif
/****************************************************** END OF FILE ******************************************************/
