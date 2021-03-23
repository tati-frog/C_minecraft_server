#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "networking.h"

#define SERVER_PORT 25566

void inputHandler(ServerCtx *ctx, int fd)
{
    char buffer[50];
    memset(buffer, 0, 50);
    recv(fd, buffer, 49, 0);
    printf("New data: %s\n", buffer);

    sendData(fd, buffer, 50);
}

void newConnectionHandler(ServerCtx *ctx, int fd)
{
    printf("New connection!\n");
}

void disconnectionHandler(ServerCtx *ctx, int fd)
{
    printf("Client disconnected\n");
}

int main()
{
    int err;
    ServerCtx *ctx = createServerContext();

    setNewConnectionHandler(ctx, &newConnectionHandler);
    setDisconnectHandler(ctx, &disconnectionHandler);
    setInputDataHandler(ctx, &inputHandler);

    err = bindServer(ctx, 25565, "127.0.0.1");
    if(err == -1)
    {
        perror("Error binding server");
        exit(-1);
    }
    startListening(ctx, 10);

    startEventLoop(ctx);

    for(;;)
    {
        printf("Press q to exit.\n");
        if(getc(stdin) == 'q')
        {
            stopServer(ctx);
            exit(0);
        }
    }
}