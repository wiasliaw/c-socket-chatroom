#ifndef MSG_H
#define MSG_H
#define DATA_SIZE 1024
#define NAME_SIZE 10
typedef enum
{
    CONN,
    SEND,
    CLOSE,
} msg_t;

typedef struct __chat_msg
{
    msg_t type;
    int room;
    char name[NAME_SIZE];
    char data[DATA_SIZE];
} chat_msg_t;
#endif
