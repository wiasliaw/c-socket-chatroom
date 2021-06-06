#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "message.h"
#include "server.h"

int main(int argc, char const *argv[])
{
    // fd
    fd_set master;
    fd_set read_fds;
    int fdmax;

    // socket
    int listener;
    int newfd;
    struct sockaddr_storage remoteaddr;
    socklen_t addrlen;

    // room
    room_t *rooms = room_init();

    // read buffer
    chat_msg_t handle_msg;
    int nbytes;

    int opt = 1;
    int i, rv;

    struct addrinfo hints, *ai, *p;

    FD_ZERO(&master);
    FD_ZERO(&read_fds);

    // 給我們一個 socket，並且 bind 它
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if ((rv = getaddrinfo(NULL, argv[2], &hints, &ai)) != 0)
    {
        fprintf(stderr, "selectserver: %s\n", gai_strerror(rv));
        exit(1);
    }

    // bind
    for (p = ai; p != NULL; p = p->ai_next)
    {
        listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (listener < 0)
            continue;

        setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int));

        if (bind(listener, p->ai_addr, p->ai_addrlen) < 0)
        {
            close(listener);
            continue;
        }

        break;
    }

    if (p == NULL)
    {
        fprintf(stderr, "selectserver: failed to bind\n");
        exit(2);
    }
    freeaddrinfo(ai);

    // listen
    if (listen(listener, 10) == -1)
    {
        perror("listen");
        exit(3);
    }

    FD_SET(listener, &master);
    fdmax = listener;

    // main logic
    for (;;)
    {
        read_fds = master;

        if (select(fdmax + 1, &read_fds, NULL, NULL, NULL) == -1)
        {
            perror("select");
            exit(4);
        }

        for (i = 0; i <= fdmax; i++)
        {
            if (FD_ISSET(i, &read_fds))
            {
                if (i == listener)
                {
                    // handle new connections
                    addrlen = sizeof remoteaddr;
                    newfd = accept(listener,
                                   (struct sockaddr *)&remoteaddr,
                                   &addrlen);

                    if (newfd == -1)
                    {
                        perror("accept");
                    }
                    else
                    {
                        FD_SET(newfd, &master);
                        if (newfd > fdmax)
                            fdmax = newfd;
                    }
                }
                else
                {
                    if ((nbytes = recv(i, (chat_msg_t *)&handle_msg, sizeof(chat_msg_t), 0)) <= 0)
                    {
                        if (nbytes == 0)
                        {
                            printf("selectserver: socket %d hung up\n", i);
                        }
                        else
                        {
                            perror("recv");
                        }
                        room_delete(rooms, i);
                        close(i);
                        FD_CLR(i, &master);
                    }
                    else
                    {
                        if (handle_msg.type == CONN)
                        {
                            if (room_create_or_join(rooms, handle_msg.room, i) != 0)
                                perror("room_create_or_join");
                        }
                        else if (handle_msg.type == SEND)
                        {
                            int idx = room_get_index(rooms, handle_msg.room);
                            for (int x = 0; x < CAP; x++)
                            {
                                if ((rooms + idx)->cap[x] == i || (rooms + idx)->cap[x] == -1)
                                    continue;
                                if (send((rooms + idx)->cap[x], (chat_msg_t *)&handle_msg, sizeof(chat_msg_t), 0) == -1)
                                    perror("send");
                            }
                        }
                    }
                }
            }
        }
    }

    return 0;
}
