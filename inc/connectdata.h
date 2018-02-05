#ifndef CONNECTDATA_H
#define CONNECTDATA_H

//设计消息类型
//落子消息
#define MSG_PUTSTEP 0x01
//和棋请求消息
#define MSG_DRAW 0x02
//同意和棋消息
#define MSG_AGREE_DRAW 0x03
//拒绝和棋消息
#define MSG_REFUSE_DRAW 0x04

//设计数据包格式
typedef struct _tagMsgStruct {
    unsigned short len;
    unsigned short msgType;
    int x;
    int y;
    int color;
    char *msg;
} MSGSTRUCT;
#endif // CONNECTDATA_H
