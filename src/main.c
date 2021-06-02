// TODO Documentation

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "net/server.h"
#include "net/mcprotocol.h"
#include "utils/buffer.h"
#include "net/handlers.h"
#include "game.h"

#define SERVER_PORT 25565

HashTable* clientsSessions;

int main()
{
    int err;
    ServerCtx *ctx = createServerContext();

    clientsSessions = hashtableCreate(10, sizeof(SessionCtx));

    setNewConnectionHandler(ctx, &newConnectionHandler);
    setDisconnectHandler(ctx, &disconnectionHandler);
    setInputDataHandler(ctx, &newDataHandler);

    err = bindServer(ctx, SERVER_PORT, "127.0.0.1");
    if(err == -1)
    {
        perror("Error binding server");
        exit(-1);
    }
    startListening(ctx, 10);

    startEventLoop(ctx);

    startGameLoop();

    stopServer(ctx);
    exit(0);
}