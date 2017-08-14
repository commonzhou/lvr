#ifndef MESSAGEMANAGER_H
#define MESSAGEMANAGER_H
#include "prefix.h"
struct MessageNode {
    INT8 *CString;
    int size;
    int used_flag;
    struct MessageNode *next;
    MessageNode():size(0),used_flag(0) {
        CString = NULL;
    }
    ~MessageNode() {
        free(CString);
    }
};

struct subMessageList {
    struct MessageNode *pHead;
    struct MessageNode *pTail;
    struct MessageNode *present;
    struct subMessageList *next;
    subMessageList() {
        pHead = new MessageNode();
        pTail = new MessageNode();
        present = new MessageNode();
    }
    ~subMessageList() {
        MessageNode *node = pHead;
        MessageNode *ptr = node;
        while(node != NULL) {
            ptr = node->next;
            delete(node);
            node = node->next;
        }
        pHead = NULL;
        pTail = NULL;
        present = NULL;
    }
};


struct MessageList {
    int streamID;
    struct subMessageList *pSRCL; // 码控反馈信息链表
    struct subMessageList *pRRCL; // 码控接收信息链表
    struct subMessageList *pRCL; // 编码信息进度列表
    struct MessageList *next;

    MessageList():streamID(0) {
        pSRCL = new subMessageList();
        pRRCL = new subMessageList();
        pRCL = new subMessageList();
    }
    ~MessageList() {
        delete pSRCL;
        delete pRRCL;
        delete pRCL;
        pSRCL = NULL;
        pRRCL = NULL;
        pRCL = NULL;
    }
};


struct MessageManager {
    int StreamNum; // 当前节点要编码的数量, tile+音频+全图
    struct MessageList *pVHead; // 每个encoder对应一个list
    struct MessageList *pVTail;

    MessageManager():StreamNum(0) {
        pVHead = new MessageList();
        pVTail = new MessageList();
    }

    ~MessageManager() {
        delete pVHead;
        delete pVTail;
        pVHead = NULL;
        pVTail = NULL;
    }
};


int create_messageManager(MessageManager *messageMag,int StreamNum);

int create_messageNode(MessageNode *pNode);

int get_messageNode(MessageNode *pNode,subMessageList *pList);

int add_messageNode(subMessageList *pList,MessageNode *message,HANDLE *mutex);

int update_messageNode(subMessageList *pList, HANDLE *mutex);

int delete_messageManager(MessageManager *messageMag, HANDLE *mutex);


#endif