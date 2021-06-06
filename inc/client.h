#ifndef CLIENT_H
#define CLIENT_H
#include <unistd.h>
#include <signal.h>
#include "message.h"
// global info
static char name[NAME_SIZE];
static int room;
static int ssocket_fd;
static sig_atomic_t flag = 0;

// static function
static void str_trim_lf(char *arr, int length)
{
    int i;
    for (i = 0; i < length; i++)
    {
        if (arr[i] == '\n')
        {
            arr[i] = '\0';
            break;
        }
    }
}

static void catch_ctrl_c_and_exit(int sig)
{
    flag = 1;
}

static void init_send(int user)
{
    chat_msg_t *msg = (chat_msg_t *)malloc(sizeof(chat_msg_t));
    bzero(msg, sizeof(chat_msg_t));
    msg->room = room;
    msg->type = CONN;
    strncpy(msg->name, name, NAME_SIZE);
    send(user, (chat_msg_t *)msg, sizeof(chat_msg_t), 0);
    free(msg);
}

static void *send_data(void *user)
{
    int ssock_fd = *(int *)user;
    char message[DATA_SIZE] = {};
    chat_msg_t *send_msg = (chat_msg_t *)malloc(sizeof(chat_msg_t));

    // loop
    while (1)
    {
        // input
        fgets(message, DATA_SIZE, stdin);
        str_trim_lf(message, DATA_SIZE);
        // format message
        send_msg->type = SEND;
        send_msg->room = room;
        strncpy(send_msg->name, name, NAME_SIZE);
        strncpy(send_msg->data, message, DATA_SIZE);
        // send
        send(ssock_fd, (chat_msg_t *)send_msg, sizeof(chat_msg_t), 0);
        bzero(message, DATA_SIZE);
        bzero(send_msg, sizeof(chat_msg_t));
    }
    return NULL;
}

void *recv_data(void *user)
{
    int ssock_fd = *(int *)user;
    chat_msg_t *recv_msg = (chat_msg_t *)malloc(sizeof(chat_msg_t));
    while (1)
    {
        read(ssock_fd, (chat_msg_t *)recv_msg, sizeof(chat_msg_t));
        printf("[room:%d][%s]: %s\n", recv_msg->room, recv_msg->name, recv_msg->data);
    }
    return NULL;
}
#endif
