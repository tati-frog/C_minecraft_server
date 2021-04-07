#ifndef __HANDLERS
#define __HANDLERS

#include "net/server.h"
#include "net/mcprotocol.h"
void newDataHandler(ServerCtx *ctx, ConnectionCtx* connectionContext);

void newConnectionHandler(ServerCtx *ctx, ConnectionCtx* connectionContext);

void disconnectionHandler(ServerCtx *ctx, ConnectionCtx* connectionContext);

#endif
