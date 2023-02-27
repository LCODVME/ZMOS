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
/*****************************************************************
* FUNCTION: zm_msgtPublishMsg
*
* DESCRIPTION:
*     This function to publish message at topic.
* INPUTS:
*     topic : Which topic to publish.
*     msg : The message to publish.
*     msgLen : message length.
* RETURNS:
*     publish result (@ref msgtResult_t).
* NOTE:
*     null
*****************************************************************/
msgtResult_t zm_msgtPublishMsg(msgtTopic_t topic, zm_uint8_t *msg, zm_msgLen_t msgLen)
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
                
                msgNode_t *pMsgNode = srchNode->msgNodeList;
                if(pMsgNode)
                {
                    while(pMsgNode->next != NULL) pMsgNode = pMsgNode->next;
                    pMsgNode->next = newMsgNode;
                }
                else srchNode->msgNodeList = newMsgNode;
                
                noticeNode_t *pNnoticNode = srchNode->noticeNodeList;
                while(pNnoticNode)
                {
                    newMsgNode->count++;
                    if(pNnoticNode->msgNode == NULL)
                    {
                        pNnoticNode->msgNode = newMsgNode;
                        if(pNnoticNode->noticFunc)
                            pNnoticNode->noticFunc(topic, msgLen);
                    }
                    pNnoticNode = pNnoticNode->next;
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
/*****************************************************************
* FUNCTION: zm_msgtSubscribeTopic
*
* DESCRIPTION:
*     This function to subscribe topic.
* INPUTS:
*     client : Which client to subscribe.
*     topic : Which topic to subscribe.
*     noticFunc : The client is notified by this callback 
*                 function when a message is published for 
*                 the subscribed topic.
* RETURNS:
*     subscribe result (@ref msgtResult_t).
* NOTE:
*     null
*****************************************************************/
msgtResult_t zm_msgtSubscribeTopic(zm_msgtClient_t *client, msgtTopic_t topic, zm_msgtNoticeFun_t noticFunc)
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
            msgtClient_t *newClient = (msgtClient_t *)zmos_malloc(sizeof(msgtClient_t));
            if(!newNode->noticeNodeList || !newClient)
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
            
            newClient->topic = topic;
            newClient->noticeNode = newNode->noticeNodeList;
            newClient->next = NULL;
            if(client->node)
            {
                msgtClient_t *pClient = (msgtClient_t *)client->node;
                while(pClient->next != NULL) pClient = pClient->next;
                pClient->next = newClient;
            }
            else client->node = newClient;
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
            noticeNode_t *srchNoticeNode = srchNode->noticeNodeList;
            while(srchNoticeNode->next != NULL) srchNoticeNode = srchNoticeNode->next;
            srchNoticeNode->next = pNoticeNode;
        }
        else return MSGT_NO_SPACE;
    }
    return MSGT_OK;
}
/*****************************************************************
* FUNCTION: zm_msgtUnsubscribeTopic
*
* DESCRIPTION:
*     This function to unsubscribe topic.
* INPUTS:
*     client : Which client to unsubscribe.
*     topic :  Unsubscribe topic.
* RETURNS:
*     subscribe result (@ref msgtResult_t).
* NOTE:
*     null
*****************************************************************/
msgtResult_t zm_msgtUnsubscribeTopic(zm_msgtClient_t *client, msgtTopic_t topic)
{
    topicNode_t *srchNode = topicNodeListHead;
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
                    }
                    while(delNoticeNode->msgNode)
                    {
                        delNoticeNode->msgNode->count--;
                        delNoticeNode->msgNode = delNoticeNode->msgNode->next;
                    }
                    zmos_setTaskEvent(msgtTaskHandle, ZM_MSGT_MSG_GC_EVENT);
                    
                    zmos_free(delNoticeNode);
                    break;
                }
                prevNoticeNode = delNoticeNode;
                delNoticeNode = delNoticeNode->next;
            }
            zmos_free(pClient);
            break;
        }
        srchNode = srchNode->next;
        prevClient = pClient;
        pClient = pClient->next;
    }
    return MSGT_OK;
}
/*****************************************************************
* FUNCTION: zm_msgtGetMsg
*
* DESCRIPTION:
*     This function to get client message.
* INPUTS:
*     client : Which client to get.
* RETURNS:
*     Message (@ref msgtMsg_t).
*     If message len is 0, the client no have new message.
* NOTE:
*     null
*****************************************************************/
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
        pClient = pClient->next;
    }
    return pMsg;
}
/*****************************************************************
* FUNCTION: zm_msgtGetTopicMsg
*
* DESCRIPTION:
*     Gets information about the specified topic.
* INPUTS:
*     client : Which client to get.
*     topic : Which topic.
* RETURNS:
*     Message (@ref msgtMsg_t).
*     If message len is 0, the client no have new message at topic.
* NOTE:
*     null
*****************************************************************/
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
        pClient = pClient->next;
    }
    return pMsg;
}
/*****************************************************************
* FUNCTION: zm_msgtInit
*
* DESCRIPTION:
*     Msgt initialize.
* INPUTS:
*     null
* RETURNS:
*     null
* NOTE:
*     If enable ZMOS_INIT_SECTION,The user should not call it.
*****************************************************************/
void zm_msgtInit(void)
{
    zmos_taskThreadRegister(&msgtTaskHandle, zm_msgtPorcessEvent);
}
/*****************************************************************
* FUNCTION: zm_msgtPorcessEvent
*
* DESCRIPTION:
*     ZMOS task, msgt event porcess.
* INPUTS:
*     
* RETURNS:
*     
* NOTE:
*     null
*****************************************************************/
static uTaskEvent_t zm_msgtPorcessEvent(uTaskEvent_t events)
{
    if(events & ZM_MSGT_MSG_GC_EVENT)
    {
        zm_msgtMsgGc();
        return events ^ ZM_MSGT_MSG_GC_EVENT;
    }
    return 0;
}
/*****************************************************************
* FUNCTION: zm_msgtMsgGc
*
* DESCRIPTION:
*     Msgt message garbage collection.
* INPUTS:
*     
* RETURNS:
*     
* NOTE:
*     null
*****************************************************************/
static void zm_msgtMsgGc(void)
{
    topicNode_t *srchNode = topicNodeListHead;
    topicNode_t *prevNode = NULL;
    msgNode_t *prevMsgNode;
    msgNode_t *delMsgNode;
    while(srchNode)
    {
        if(srchNode->noticeNodeList == NULL)
        {
            topicNode_t *delNode = srchNode;
            
            srchNode = srchNode->next;
            while(delNode->msgNodeList)
            {
                delMsgNode = delNode->msgNodeList;
                delNode->msgNodeList = delNode->msgNodeList->next;
                if(delMsgNode->msg) zmos_free(delMsgNode->msg);
                zmos_free(delMsgNode);
            }
            if(prevNode)
            {
                prevNode->next = delNode->next;
            }
            else topicNodeListHead = topicNodeListHead->next;
            zmos_free(delNode);
            continue;
        }
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
                srchNode->msgNodeList = srchNode->msgNodeList->next;
                if(delMsgNode->msg) zmos_free(delMsgNode->msg);
                zmos_free(delMsgNode);
            }
            else
            {
                prevMsgNode = srchNode->msgNodeList;
                srchNode->msgNodeList = srchNode->msgNodeList->next;
            }
        }
        prevNode = srchNode;
        srchNode = srchNode->next;
    }
}

#if (defined ZMOS_INIT_SECTION) && (ZMOS_INIT_SECTION)
ZMOS_INIT_COMPONENT_REGISTER(zm_msgtInit);
#endif
/****************************************************** END OF FILE ******************************************************/
