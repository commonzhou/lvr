#ifndef AVFRAMEMANAGER_H
#define AVFRAMEMANAGER_H
#include "prefix.h"

struct AVFrameNode {
    UINT8 *Yframe;
    UINT8 *Uframe;
    UINT8 *Vframe;
    int width;
    int height;
    UINT8 used_flag;

    //这8bit的数据按照如下的格式进行安排
    //备用-备用-备用-备用-码控完成flag-全局映射完成flag-Tile划分完成flag-编码完成flag
    UINT8 used_mask;
    struct AVFrameNode *next;

    AVFrameNode():width(0),height(0),used_flag(0),used_mask(0){
        Yframe = (UINT8 *)malloc(sizeof(UINT8));
        Uframe = (UINT8 *)malloc(sizeof(UINT8));
        Vframe = (UINT8 *)malloc(sizeof(UINT8));
        memset(Yframe, sizeof(UINT8), 0);
        memset(Vframe, sizeof(UINT8), 0);
        memset(Uframe, sizeof(UINT8), 0);
        next = NULL;
    }
    ~AVFrameNode() {
        free(Yframe);
        free(Uframe);
        free(Vframe);
        Yframe = NULL;
        Uframe = NULL;
        Vframe = NULL;
        next = NULL;
    }
};

struct AVFrameList {
    MEDIAType type;
    int tileID;
    struct AVFrameNode *pVhead;
    struct AVFrameNode *pVTail;
    struct AVFrameNode *present; 
    struct AVFrameList *next;
    AVFrameList():tileID(0) {
        type = MEDIA_NONE;
        pVhead = new AVFrameNode();
        pVTail = pVhead;
        present = NULL;
        next = NULL;
    }
    ~AVFrameList() {
        AVFrameNode *node = pVhead;
        AVFrameNode *ptr = node;
        while (node != NULL) {
            ptr = node->next;
            delete (node);
            node = ptr;
        }
        pVhead = NULL;
        pVTail = NULL;
        present = NULL;
    }
};


struct subPicManager {
    int subpicNum;
    struct AVFrameList *pVHead;
    struct AVFrameList *pVTail;
    subPicManager():subpicNum(0) {
        pVHead = new AVFrameList();
        pVTail = new AVFrameList();
    }
    ~subPicManager() {
        delete pVHead;
        delete pVTail;
        pVTail = NULL;
        pVHead = NULL;
    }
};

struct AVFrameManager {
  struct AVFrameList *psourceVList;
  struct AVFrameList *psourceAList;
  struct AVFrameList *pProjectList;
  struct subPicManager *pSPM;

  AVFrameManager() {
      psourceVList = new AVFrameList();
      psourceAList = new AVFrameList();
      pProjectList = new AVFrameList();
      pSPM = new subPicManager();
  }
  ~AVFrameManager() {
      delete pSPM;
      delete psourceAList;
      delete psourceVList;
      delete pProjectList;
      pSPM = NULL;
      psourceVList = NULL;
      psourceAList = NULL;
      pProjectList = NULL;
  }
};

int create_AVFrameManager(AVFrameManager*& AVFrameMag, int subpicNum);

int create_AVFrameNode(AVFrameNode*& pNode);

int get_AVFrameNode(AVFrameNode*& pNode, AVFrameList *pList);

int add_AVFrameNode(AVFrameList *pList,AVFrameNode *pNode,HANDLE *mutex);

int update_AVFrameList(AVFrameList *pList,UINT8 used_mask,HANDLE *mutex);

int delete_AVFrameManager(AVFrameManager*& AVFrameMag, HANDLE *mutex);
#endif