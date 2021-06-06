#ifndef SERVER_H
#define SERVER_H
#include <stdio.h>
#include <stdlib.h>
#include "message.h"
#define ROOM_SIZE 30
#define CAP 10

typedef struct __room
{
    int limit;
    int id;
    int cap[CAP];
} room_t;

static room_t *room_init()
{
    room_t *ptr = (room_t *)malloc(sizeof(room_t) * ROOM_SIZE);
    for (int i = 0; i < ROOM_SIZE; i++)
    {
        (ptr + i)->id = -1;
        for (int j = 0; j < CAP; j++)
            (ptr + i)->cap[j] = -1;
    }
    return ptr;
}

static int room_create_or_join(room_t *rooms, int roomID, int client_fd)
{
    int hasID = -1, tarID = ROOM_SIZE;
    for (int i = 0; i < ROOM_SIZE; i++)
    {
        room_t *curr = (rooms + i);
        tarID = (curr->id == -1 && i < tarID) ? i : tarID;
        if ((rooms + i)->id == roomID)
        {
            hasID = i;
            break;
        }
    }
    // create room or join
    if (hasID == -1)
    {
        (rooms + tarID)->id = roomID;
        (rooms + tarID)->limit = 0;
        (rooms + tarID)->cap[0] = client_fd;
    }
    else
    {
        for (int j = 0; j < CAP; j++)
        {
            if ((rooms + hasID)->cap[j] == -1) {
                (rooms + hasID)->cap[j] = client_fd;
                break;
            }
        }
    }
    return 0;
}

static int room_get_index(room_t *rooms, int roomID)
{
    for (int i = 0; i < ROOM_SIZE; i++)
    {
        if ((rooms + i)->id == roomID)
            return i;
    }
    return -1;
}

static void room_delete(room_t *rooms, int client_fd)
{
    for (int i = 0; i < ROOM_SIZE; i++)
    {
        for (int j = 0; j < CAP; j++)
            if ((rooms + i)->cap[j] == client_fd)
            {
                (rooms + i)->cap[j] = -1;
            }
    }
}

#endif
