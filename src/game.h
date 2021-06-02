#ifndef _GAME
#define _GAME

#include "net/mcprotocol.h"

typedef struct {
    HashTable *clientsSessions;
    int onlinePlayers;

} GameState;

extern GameState gamestate;

void getJsonGameStatus(mc_string buf);

void startGameLoop();

#endif