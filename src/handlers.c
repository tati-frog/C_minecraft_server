#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <uuid/uuid.h>

#include "handlers.h"
#include "net/server.h"
#include "net/mcprotocol.h"

void handleHandshakingState(ServerCtx *ctx, ConnectionCtx* connectionContext, MCPacket *packet)
{
    switch (packet->id)
    {
    case HANDSHAKE:
    {
        in_HandshakePacket handshakePacket;
        readHandshakingPacket(packet, &handshakePacket);

        printf("Handshake packet received. Next state: %d\n", handshakePacket.nextState);
        SessionCtx *session = connectionContext->contextData;
        session->status = handshakePacket.nextState;

        free(handshakePacket.serverAddress);
    }break;
    
    default:
        break;
    }
}

void handleStatusState(ServerCtx *ctx, ConnectionCtx* connectionContext, MCPacket *packet)
{
    switch(packet->id)
    {
    case STATUS_REQUEST:
    {
        printf("Status request packet received.\n");

        mc_string jsonTest = "{\"version\": {\"name\": \"1.14.4\",\"protocol\": 498},\"players\": {\"max\": 100,\"online\": 5,\"sample\": [{\"name\": \"thinkofdeath\",\"id\": \"4566e69f-c907-48ee-8d71-d7ba5aa00d20\"}]},\"description\": {\"text\": \"Hello world\"},\"favicon\": \"data:image/png;base64,<data>\"}";

        out_ResponseStatusPacket responsePacket;
        responsePacket.jsonResponse = jsonTest;

        MCPacket packet;
        createPacket(&packet);

        writeStatusResponsePacket(&packet, &responsePacket);
        writePacket(connectionContext->response, &packet);

        releasePacket(&packet);
    }break;

    case STATUS_PING:
    {
        in_PingStatusPacket pingPacket;
        readPingPacket(packet, &pingPacket);

        printf("Ping request received with payload %ld.\n", pingPacket.payload);

        out_PongStatusPacket pongPacket;
        pongPacket.payload = pingPacket.payload;

        MCPacket packet;
        createPacket(&packet);

        writePongPacket(&packet, &pongPacket);
        writePacket(connectionContext->response, &packet);
        releasePacket(&packet);
    }break;
    }
}

void handleLoginState(ServerCtx *ctx, ConnectionCtx* connectionContext, MCPacket *packet)
{
    switch(packet->id)
    {
    case LOGIN_START:
    {
        in_LoginStartPacket loginStartPacket;
        readLoginStart(packet, &loginStartPacket);

        SessionCtx *session = (SessionCtx *)connectionContext->contextData;

        createPlayer(&session->player, loginStartPacket.name);

        printf("New user wants to log in with username %s\n", loginStartPacket.name);

        out_LoginSuccessPacket loginSuccessPacket;
        loginSuccessPacket.username = session->player.nickname;
        uuid_unparse(session->player.uuid, loginSuccessPacket.uuid);

        MCPacket outPacket;
        createPacket(&outPacket);

        writeLoginSuccess(&outPacket, &loginSuccessPacket);
        writePacket(connectionContext->response, &outPacket);

        free(loginStartPacket.name);
        releasePacket(&outPacket);
    }
    }
}

void newDataHandler(ServerCtx *ctx, ConnectionCtx* connectionContext)
{
    int readedBytes;

    MCPacket packet;
    createPacket(&packet);

    readedBytes = readPacket(connectionContext->data, &packet);
    if(readedBytes == -1)
    {
        printf("Invalid packet\n");
        return;
    }

    SessionCtx *session = connectionContext->contextData;
    switch (session->status)
    {
    case HANDSHAKING:
        handleHandshakingState(ctx, connectionContext, &packet);
        break;

    case STATUS:
        handleStatusState(ctx, connectionContext, &packet);
        break;

    case LOGIN:
        handleLoginState(ctx, connectionContext, &packet);
        break;
    
    default:
        break;
    }

    releasePacket(&packet);
}

void newConnectionHandler(ServerCtx *ctx, ConnectionCtx* connectionContext)
{
    connectionContext->contextData = realloc(connectionContext->contextData, sizeof(SessionCtx));
    SessionCtx *session = connectionContext->contextData;
    session->status = HANDSHAKING;

    printf("New connection!\n");
}

void disconnectionHandler(ServerCtx *ctx, ConnectionCtx* connectionContext)
{
    printf("Client disconnected\n");
}