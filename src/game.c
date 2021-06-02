#include <stdio.h>
#include <stdlib.h>

#include "game.h"

GameState gamestate;

void getJsonGameStatus(mc_string buf)
{
    mc_string outfmt = "{"
                        "\"version\": {"
                            "\"name\": \"1.14.4\","
                            "\"protocol\": 498"
                        "},"
                        "\"players\": {"
                            "\"max\": 100,"
                            "\"online\": %d,"
                            "\"sample\": []"
                        "},"
                        "\"description\": {"
                            "\"text\": \"A Minecraft server\""
                        "},"
                        "\"favicon\": \"data:image/png;base64,<data>\""
                    "}";

    sprintf(buf, outfmt, gamestate.onlinePlayers);
}

void startGameLoop()
{
    for (;;)
    {
        printf("Press q to exit.\n");
        if (getc(stdin) == 'q')
        {
            return;
        }
    }
}