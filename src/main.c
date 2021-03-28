#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "net/server.h"
#include "net/mcprotocol.h"
#include "utils/buffer.h"
#include "handlers.h"

#define SERVER_PORT 25566

int main()
{
    int err;
    ServerCtx *ctx = createServerContext();

    setNewConnectionHandler(ctx, &newConnectionHandler);
    setDisconnectHandler(ctx, &disconnectionHandler);
    setInputDataHandler(ctx, &newDataHandler);

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