#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <uuid/uuid.h>

#include "net/handlers.h"
#include "net/server.h"
#include "net/mcprotocol.h"
#include "game.h"

void createSession(int id)
{
    SessionCtx session;
    session.status = HANDSHAKING;

    hashtableSetElement(gamestate.clientsSessions, id, &session);
}

SessionCtx* getSession(int id)
{
    SessionCtx *session = hashtableGetElement(gamestate.clientsSessions, id);
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

        MCPacket packet;;

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

        MCPacket packet;;

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

        out_JoinGamePacket joinGamePacket;
        joinGamePacket.entityId = 63;
        joinGamePacket.gamemode = 0;
        joinGamePacket.maxPlayers = 5;
        joinGamePacket.levelType = "flat";
        joinGamePacket.viewDistance = 2;
        joinGamePacket.reducedDebugInfo = 0;

        mcPacketCreate(&outPacket);

        writeJoinGame(&outPacket, &joinGamePacket);
        mcPacketWrite(connectionContext->response, &outPacket);

        session->status = PLAY;
        mcPacketDestroy(&outPacket);
    }
    }
}

void handlePlayingState(ServerCtx *ctx, ConnectionCtx *connectionContext, MCPacket *packet)
{
    switch (packet->id)
    {
    case PLAY_CLIENT_SETTINGS:
    {
        in_ClientSettingsPacket clientSettingsPacket;
        readClientSettings(packet, &clientSettingsPacket);

        SessionCtx *session = getSession(connectionContext->fd);

        session->settings.locale = clientSettingsPacket.locale;
        session->settings.viewDistance = clientSettingsPacket.viewDistance;
        session->settings.chatMode = clientSettingsPacket.chatMode;
        session->settings.chatColors = clientSettingsPacket.chatColors;
        session->settings.displayedSkinParts = clientSettingsPacket.displayedSkinParts;
        session->settings.mainHand = clientSettingsPacket.mainHand;

        out_PlayerPositionAndLook playerPositionAndLookPacket;
        playerPositionAndLookPacket.x = 0;
        playerPositionAndLookPacket.y = 0;
        playerPositionAndLookPacket.z = 0;
        playerPositionAndLookPacket.yaw = 0;
        playerPositionAndLookPacket.pitch = 0;
        playerPositionAndLookPacket.flags = 0;
        playerPositionAndLookPacket.teleportId = 1;

        MCPacket outPacket;
        mcPacketCreate(&outPacket);

        writePlayerPositionAndLook(&outPacket, &playerPositionAndLookPacket);

        mcPacketWrite(connectionContext->response, &outPacket);
        
        break;
    }
    default:
        break;
    }
}

void newDataHandler(ServerCtx *ctx, ConnectionCtx *connectionContext)
{
    int readedBytes;

    MCPacket packet;

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
    hashtableDeleteElement(gamestate.clientsSessions, connectionContext->fd);
    printf("Client disconnected\n");
}