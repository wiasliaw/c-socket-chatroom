#include <arpa/inet.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "client.h"
#include "message.h"

/**
 * @param ip {argv[1]} - "127.0.0.1"
 * @param port {argv[2]} - "8888"
 * @param name {argv[3]} - "Will"
 * @param room {argv[4]} - -1 (means 1 to 1) or other > 0 (join or create the room)
 */
int main(int argc, char const *argv[])
{
    // info load
    room = atoi(argv[4]);
    strncpy(name, argv[3], NAME_SIZE);
    int err_n;

    // create socket
    if ((ssocket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Fail socket()\n");
        return EXIT_FAILURE;
    }

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[2]));

    if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0)
    {
        printf("Fail inet_pton()\n");
        return EXIT_FAILURE;
    }

    if ((err_n = connect(ssocket_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) < 0)
    {
        printf("Fail connect() %d\n", err_n);
        return err_n;
    }

    init_send(ssocket_fd);
    // signal
    signal(SIGINT, catch_ctrl_c_and_exit);

    // pthread
    pthread_t send_thread, recv_thread;
    if (pthread_create(&send_thread, NULL, (void *)send_data, (void *)&ssocket_fd) != 0)
    {
        printf("ERROR: pthread\n");
        return EXIT_FAILURE;
    }
    if (pthread_create(&recv_thread, NULL, (void *)recv_data, (void *)&ssocket_fd) != 0)
    {
        printf("ERROR: pthread\n");
        return EXIT_FAILURE;
    }

    while (1)
    {
        if (flag)
        {
            printf("\nBye\n");
            break;
        }
    }
    return EXIT_SUCCESS;
}
