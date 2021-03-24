#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "net/server.h"
#include "net/mcprotocol.h"
#include "net/utils/buffer.h"

#define SERVER_PORT 25566

void inputHandler(ServerCtx *ctx, int fd, Buffer *data)
{
    MCPacket packet;
    readPacket(data, &packet);

    struct sockaddr_in clientAddr;
    socklen_t addrlen = sizeof(clientAddr);
    memset(&clientAddr, 0, sizeof(clientAddr));

    getpeername(fd, (struct sockaddr*)&clientAddr, &addrlen);
    printf("New packet with ID %d of %d bytes from %s:%d.\n", packet.id, packet.lenght, inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

    MCPacket responsePacket;
    responsePacket.id = 255;
    responsePacket.lenght = 10000;
    responsePacket.dataSize = 12;
    responsePacket.data = "Hello world";

    Buffer *netPacket = createBuffer();
    writePacket(netPacket, &responsePacket);

    send(fd, netPacket->data, netPacket->size, 0);

    releaseBuffer(netPacket);
    releaseBuffer(data);
    free(packet.data);
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