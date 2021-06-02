#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <uuid/uuid.h>

#include "net/handlers.h"
#include "net/server.h"
#include "net/mcprotocol.h"
#include "game.h"

extern HashTable *clientsSessions;

void createSession(int id)
{
    SessionCtx session;
    session.status = HANDSHAKING;

    hashtableSetElement(clientsSessions, id, &session);
}

SessionCtx* getSession(int id)
{
    SessionCtx *session = hashtableGetElement(clientsSessions, id);
    return session;
}

void handleHandshakingState(ServerCtx *ctx, ConnectionCtx *connectionContext, MCPacket *packet)
{
    switch (packet->id)
    {
    case HANDSHAKE:
    {
        in_HandshakePacket handshakePacket;
        readHandshakingPacket(packet, &handshakePacket);

        printf("Handshake packet received. Next state: %d\n", handshakePacket.nextState);
        SessionCtx *session = getSession(connectionContext->fd);

        session->status = handshakePacket.nextState;
        free(handshakePacket.serverAddress);
    }
    break;

    default:
        break;
    }
}

void handleStatusState(ServerCtx *ctx, ConnectionCtx *connectionContext, MCPacket *packet)
{
    switch (packet->id)
    {
    case STATUS_REQUEST:
    {
        printf("Status request packet received.\n");

        out_ResponseStatusPacket responsePacket;

        char jsonStatus[500];
        getJsonGameStatus(jsonStatus);

        responsePacket.jsonResponse = jsonStatus;

        MCPacket packet;
        mcPacketCreate(&packet);

        writeStatusResponsePacket(&packet, &responsePacket);
        mcPacketWrite(connectionContext->response, &packet);

        mcPacketDestroy(&packet);
    }
    break;

    case STATUS_PING:
    {
        in_PingStatusPacket pingPacket;
        readPingPacket(packet, &pingPacket);

        printf("Ping request received with payload %ld.\n", pingPacket.payload);

        out_PongStatusPacket pongPacket;
        pongPacket.payload = pingPacket.payload;

        MCPacket packet;
        mcPacketCreate(&packet);

        writePongPacket(&packet, &pongPacket);
        mcPacketWrite(connectionContext->response, &packet);
        mcPacketDestroy(&packet);
    }
    break;
    }
}

void handleLoginState(ServerCtx *ctx, ConnectionCtx *connectionContext, MCPacket *packet)
{
    switch (packet->id)
    {
    case LOGIN_START:
    {
        in_LoginStartPacket loginStartPacket;
        readLoginStart(packet, &loginStartPacket);

        SessionCtx *session = getSession(connectionContext->fd);

        createPlayer(&session->player, loginStartPacket.name);

        printf("New user wants to log in with username %s\n", loginStartPacket.name);

        out_LoginSuccessPacket loginSuccessPacket;
        loginSuccessPacket.username = session->player.nickname;
        uuid_unparse(session->player.uuid, loginSuccessPacket.uuid);

        MCPacket outPacket;
        mcPacketCreate(&outPacket);

        writeLoginSuccess(&outPacket, &loginSuccessPacket);
        mcPacketWrite(connectionContext->response, &outPacket);

        free(loginStartPacket.name);
        mcPacketDestroy(&outPacket);
    }
    }
}

void handlePlayingState(ServerCtx *ctx, ConnectionCtx *connectionContext, MCPacket *packet)
{
}

void newDataHandler(ServerCtx *ctx, ConnectionCtx *connectionContext)
{
    int readedBytes;

    MCPacket packet;
    mcPacketCreate(&packet);

    readedBytes = mcPacketRead(connectionContext->data, &packet);
    if (readedBytes == -1)
    {
        releaseBufferData(connectionContext->data);
        printf("Invalid packet\n");
        return;
    }

    SessionCtx *session = getSession(connectionContext->fd);

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

    case PLAY:
        handlePlayingState(ctx, connectionContext, &packet);
        break;

    default:
        break;
    }

    mcPacketDestroy(&packet);
}

void newConnectionHandler(ServerCtx *ctx, ConnectionCtx *connectionContext)
{
    createSession(connectionContext->fd); // For now just use the socket file descriptor as session id

    printf("New connection!\n");
}

void disconnectionHandler(ServerCtx *ctx, ConnectionCtx *connectionContext)
{
    hashtableDeleteElement(clientsSessions, connectionContext->fd);
    printf("Client disconnected\n");
}