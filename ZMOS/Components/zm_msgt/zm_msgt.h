/*****************************************************************
* Copyright (C) 2022 zm. All rights reserved.                    *
******************************************************************
* zm_msgt.h
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
#ifndef __ZM_MSGT_H__
#define __ZM_MSGT_H__
 
#ifdef __cplusplus
extern "C"
{
#endif
/*************************************************************************************************************************
 *                                                        MACROS                                                         *
 *************************************************************************************************************************/
 
/*************************************************************************************************************************
 *                                                      CONSTANTS                                                        *
 *************************************************************************************************************************/
 
/*************************************************************************************************************************
 *                                                       TYPEDEFS                                                        *
 *************************************************************************************************************************/
/**
 * Msgt topic type.
 */
typedef zm_uint8_t msgtTopic_t;
/**
 * Msgt message length type
 */
typedef zm_uint16_t zm_msgLen_t;
/**
 * Msgt result type
 */
typedef enum
{
    MSGT_OK = 0,            //Msgt success.
    MSGT_NO_TOPIC,          //Publish topic no client subscribe.
    MSGT_NO_SPACE,          //Run out of memory.
    MSGT_ERR_PARAM          // Error parameters.
}msgtResult_t;
/**
 * Msgt message type.
 */
typedef struct
{
    msgtTopic_t topic;      //Msgt message topic.
    zm_msgLen_t len;        //Msgt message len.
    zm_uint8_t *msg;        //Point to message.
}msgtMsg_t;
/**
 * Msgt notice callback function type.
 *
 * @param topic : Which topic have new message.
 * @param msgLen : New message length.
 *
 */
typedef void (*zm_msgtNoticeFun_t)(msgtTopic_t topic, zm_msgLen_t msgLen);
/**
 * Msgt client type.
 */
typedef struct
{
    void *node;
}zm_msgtClient_t;
/*************************************************************************************************************************
 *                                                   PUBLIC FUNCTIONS                                                    *
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
msgtResult_t zm_msgtPublishMsg(msgtTopic_t topic, zm_uint8_t *msg, zm_msgLen_t msgLen);
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
msgtResult_t zm_msgtSubscribeTopic(zm_msgtClient_t *client, msgtTopic_t topic, zm_msgtNoticeFun_t noticFunc);
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
msgtResult_t zm_msgtUnsubscribeTopic(zm_msgtClient_t *client, msgtTopic_t topic);
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
msgtMsg_t zm_msgtGetMsg(zm_msgtClient_t client);
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
msgtMsg_t zm_msgtGetTopicMsg(zm_msgtClient_t client, msgtTopic_t topic);
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
void zm_msgtInit(void);



#ifdef __cplusplus
}
#endif
#endif /* zm_msgt.h */
