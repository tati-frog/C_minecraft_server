#ifndef _GAME
#define _GAME

#include <stdint.h>

#include <uuid/uuid.h>

#include "utils/hashtable.h"
#include "net/mcprotocol.h"

typedef struct {
    int entityId;
    uuid_t uuid;
    char* nickname;
} Player;

typedef struct {
    char* locale;
    int8_t viewDistance;
    int chatMode;
    int8_t chatColors;
    int8_t displayedSkinParts;
    int mainHand;
} ClientConfig;

enum SessionStatus {
    HANDSHAKING = 0,
    STATUS = 1,
    LOGIN = 2,
    PLAY = 3
};

typedef struct {
    enum SessionStatus status;
    Player player;
    ClientConfig settings;
} SessionCtx;
typedef struct {
    HashTable *clientsSessions;
    int onlinePlayers;

} GameState;

extern GameState gamestate;

void newClient(int clientSessionId);

void createPlayer(Player *player, char *username);

void getJsonGameStatus(char* buf);

void startGameLoop();

#endif