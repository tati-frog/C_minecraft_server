// TODO Create a better naming convention for packets and entities.

#ifndef _MCPROTOCOL
#define _MCPROTOCOL

#include <stdint.h>

#include <uuid/uuid.h>

#include "nbt/nbt.h"
#include "utils/buffer.h"
#include "utils/hashtable.h"

/****************************/
/*        Data types        */
/****************************/
typedef int8_t mc_boolean;
typedef int8_t mc_byte;
typedef uint8_t mc_ubyte;
typedef int16_t mc_short;
typedef uint16_t mc_ushort;
typedef int32_t mc_int;
typedef int64_t mc_long;
typedef float mc_float;
typedef double mc_double;
typedef char* mc_string;

/****************************/
/*          Entities        */
/****************************/

typedef struct {
    uuid_t uuid;
    mc_string nickname;
} Player;

/****************************/
/*          Packets         */
/****************************/
typedef struct{
    mc_int length;
    mc_int id;

    Buffer *data;
} MCPacket;

enum SessionStatus {
    HANDSHAKING = 0,
    STATUS = 1,
    LOGIN = 2,
    PLAY = 3
};


/**
 * Inbound packets
 */

enum InboundHandshakePackets {
    HANDSHAKE = 0x0
};

enum InboundStatusPackets {
    STATUS_REQUEST = 0x0,
    STATUS_PING = 0x1
};

enum InboundLoginPackets {
    LOGIN_START = 0x0,
    LOGIN_ENCRYPTION_RESPONSE = 0x1,
    LOGIN_PLUGIN_RESPONSE = 0x2
};

enum InboundPlayPackets {
    PLAY_TELEPORT_CONFIRM = 0x0,
    PLAY_QUERY_BLOCK_NBT = 0x1,
    PLAY_SET_DIFFICULTY = 0x2,
    PLAY_CHAT_MESSAGE_INBOUND = 0x3,
    PLAY_CLIENT_STATUS = 0x4,
    PLAY_CLIENT_SETTINGS = 0x5,
    PLAY_TAB_COMPLETE_INBOUND = 0x6,
    PLAY_WINDOW_CONFIRMATION_INBOUND = 0x7,
    PLAY_CLICK_WINDOW_BUTTON = 0x8,
    PLAY_CLICK_WINDOW = 0x9,
    PLAY_CLOSE_WINDOW_INBOUND = 0xA,
    PLAY_PLUGIN_MESSAGE_INBOUND = 0xB,
    PLAY_EDIT_BOOK = 0xC,
    PLAY_ENTITY_NBT_REQUEST = 0xD,
    PLAY_INTERACT_ENTITY = 0xE,
    PLAY_KEEP_ALIVE_INBOUND = 0xF,
    PLAY_LOCK_DIFFICULTY = 0x10,
    PLAY_PLAYER_POSITION = 0x11,
    PLAY_PLAYER_POSITION_AND_ROTATION = 0x12,
    PLAY_PLAYER_ROTATION = 0x13,
    PLAY_PLAYER_MOVEMENT = 0x14,
    PLAY_VEHICLE_MOVE_INBOUND = 0x15,
    PLAY_STEER_BOAT = 0x16,
    PLAY_PICK_ITEM = 0x17,
    PLAY_CRAFT_RECIPE_REQUEST = 0x18,
    PLAY_PLAYER_ABILITIES_INBOUND = 0x19,
    PLAY_PLAYER_DIGGING = 0x1A,
    PLAY_ENTITY_ACTION = 0x1B,
    PLAY_STEER_VEHICLE = 0x1C,
    PLAY_RECIPE_BOOK_DATA = 0x1D,
    PLAY_NAME_ITEM = 0x1E,
    PLAY_RESOURCE_PACK_STATUS = 0x1F,
    PLAY_ADVANCEMENT_TAB = 0x20,
    PLAY_SELECT_TRADE = 0x21,
    PLAY_SET_BEACON_EFFECT = 0x22,
    PLAY_HELD_ITEM_CHANGE_INBOUND = 0x23,
    PLAY_UPDATE_COMMAND_BLOCK = 0x24,
    PLAY_UPDATE_COMMAND_BLOCK_MINECART = 0x25,
    PLAY_CREATIVE_INVENTORY_ACTION = 0x26,
    PLAY_UPDATE_JIGSAW_BLOCK = 0x27,
    PLAY_UPDATE_STRUCTURE_BLOCK = 0x28,
    PLAY_UPDATE_SIGN = 0x29,
    PLAY_ANIMATION_INBOUND = 0x2A,
    PLAY_SPECTATE = 0x2B,
    PLAY_BLOCK_PLACEMENT = 0x2C,
    PLAY_USE_ITEM = 0x2D
};

// This packet its sent by the client to start a connection with the server.
typedef struct{
    mc_int protocolVersion; // Minecraft version
    mc_string serverAddress; // IP used to connect
    mc_ushort serverPort; // Port used to connect
    mc_int nextState; // Change status of connection, 1 for status and 2 for login
} in_HandshakePacket;

// This packet its sent by the client for checking the status of the server, its size is zero.
typedef struct{} in_RequestStatusPacket;

// This packet its sent by the client for measure the ping between him and the server.
typedef struct{
    mc_long payload; // Any value.
} in_PingStatusPacket;

typedef struct {
    mc_string name;
} in_LoginStartPacket;

/**
 * Outbound packets
 */

enum OutboundStatusPackets {
    STATUS_RESPONSE = 0x0,
    STATUS_PONG = 0x1
};

enum OutboundLoginPacket {
    LOGIN_DISCONNECT = 0x0,
    LOGIN_ENCRYPTION_REQUEST = 0x1,
    LOGIN_SUCCESS = 0x2,
    SET_COMPRESSION = 0x3,
    LOGIN_PLUGIN_REQUEST = 0x4
};

enum OutboundPlayPacket {
    PLAY_SPAWN_OBJECT = 0x0,
    PLAY_SPAWN_EXPERIENCE_ORB = 0x1,
    PLAY_SPAWN_GLOBAL_ENTITY = 0x2,
    PLAY_SPAWN_MOB = 0x3,
    PLAY_SPAWN_PAINTING = 0x4,
    PLAY_SPAWN_PLAYER = 0x5,
    PLAY_ENTITY_ANIMATION_OUTBOUND = 0x6,
    PLAY_STATISTICS = 0x7,
    PLAY_BLOCK_BREAK_ANIMATION = 0x8,
    PLAY_UPDATE_BLOCK_ENTITY = 0x9,
    PLAY_BLOCK_ACTION = 0xA,
    PLAY_BLOCK_CHANGE = 0xB,
    PLAY_BOSS_BAR = 0xC,
    PLAY_SERVER_DIFFICULTY = 0xD,
    PLAY_CHAT_MESSAGE_OUTBOUND = 0xE,
    PLAY_MULTIBLOCK_CHANGE = 0xF,
    PLAY_TAB_COMPLETE_OUTBOUND = 0x10,
    PLAY_DECLARE_COMMANDS = 0x11,
    PLAY_WINDOW_CONFIRMATION_OUTBOUND = 0x12,
    PLAY_CLOSE_WINDOW = 0x13,
    PLAY_WINDOW_ITEMS = 0x14,
    PLAY_WINDOW_PROPERTY = 0x15,
    PLAY_SET_SLOT = 0x16,
    PLAY_SET_COOLDOWN = 0x17,
    PLAY_PLUGIN_MESSAGE_OUTBOUND = 0x18,
    PLAY_NAMED_SOUND_EFFECT = 0x19,
    PLAY_DISCONNECT = 0x1A,
    PLAY_ENTITY_STATUS = 0x1B,
    PLAY_EXPLOSION = 0x1C,
    PLAY_UNLOAD_CHUNK = 0x1D,
    PLAY_CHANGE_GAME_STATE = 0x1E,
    PLAY_OPEN_HORSE_WINDOW = 0x1F,
    PLAY_KEEP_ALIVE_OUTBOUND = 0x20,
    PLAY_CHUNK_DATA = 0x21,
    PLAY_EFFECT = 0x22,
    PLAY_PARTICLE = 0x23,
    PLAY_UPDATE_LIGHT = 0x24,
    PLAY_JOIN_GAME = 0x25,
    PLAY_MAP_DATA = 0x26,
    PLAY_TRADE_LIST = 0x27,
    PLAY_ENTITY_POSITION = 0x28,
    PLAY_ENTITY_POSITION_AND_ROTATION = 0x29,
    PLAY_ENTITY_ROTATION = 0x2A,
    PLAY_ENTITY_MOVEMENT = 0x2B,
    PLAY_VEHICLE_MOVE_OUTBOUND = 0x2C,
    PLAY_OPEN_BOOK = 0x2D,
    PLAY_OPEN_WINDOW = 0x2E,
    PLAY_OPEN_SIGN_EDITOR = 0x2F,
    PLAY_CRAFT_RECIPE_RESPONSE = 0x30,
    PLAY_PLAYER_ABILITIES_OUTBOUND = 0x31,
    PLAY_COMBAT_EVENT = 0x32,
    PLAY_PLAYER_INFO = 0x33,
    PLAY_FACE_PLAYER = 0x34,
    PLAY_PLAYER_POSITION_AND_LOOK_OUTBOUND = 0x35,
    PLAY_UNLOCK_RECIPES = 0x36,
    PLAY_DESTROY_ENTITIES = 0x37,
    PLAY_REMOVE_ENTITY_EFFECT = 0x38,
    PLAY_RESOURCE_PACK_SEND = 0x39,
    PLAY_RESPAWN = 0x3A,
    PLAY_ENTITY_HEAD_LOCK = 0x3B,
    PLAY_SELECT_ADVANCEMENT_TAB = 0x3C,
    PLAY_WORLD_BORDER = 0x3D,
    PLAY_CAMERA = 0x3E,
    PLAY_HELD_ITEM_CHANGE_OUTBOUND = 0x3F,
    PLAY_UPDATE_VIEW_POSITION = 0x40,
    PLAY_UPDATE_VIEW_DISTANCE = 0x41,
    PLAY_DISPLAY_SCOREBOARD = 0x42,
    PLAY_ENTITY_METADATA = 0x43,
    PLAY_ATTACH_ENTITY = 0x44,
    PLAY_ENTITY_VELOCITY = 0x45,
    PLAY_ENTITY_EQUIPMENT = 0X46,
    PLAY_SET_EXPERIENCE = 0x47,
    PLAY_UDPATE_HEALTH = 0x48,
    PLAY_SCOREBOARD_OBJECTIVE = 0x49,
    PLAY_SET_PASSENGERS = 0x4A,
    PLAY_TEAMS = 0x4B,
    PLAY_UPDATE_SCORE = 0x4C,
    PLAY_SPAWN_POSITION = 0x4D,
    PLAY_TIME_UPDATE = 0x4E,
    PLAY_TITLE = 0x4F,
    PLAY_ENTITY_SOUND_EFFECT = 0x50,
    PLAY_SOUND_EFFECT = 0x51,
    PLAY_STOP_SOUND = 0x52,
    PLAY_PLAYER_LIST_HEADER_AND_FOOTER = 0x53,
    PLAY_NBT_QUERY_RESPONSE = 0x54,
    PLAY_COLLECT_ITEM = 0x55,
    PLAY_ENTITY_TELEPORT = 0x56,
    PLAY_ADVANCEMENTS = 0x57,
    PLAY_ENTITY_PROPERTIES = 0x58,
    PLAY_ENTITY_EFFECT = 0x59,
    PLAY_DECLARE_RECIPES = 0x5A,
    PLAY_TAGS = 0x5B,
    PLAY_ACKNOWLEDGE_PLAYER_DIGGING = 0x5C,
};

// This packet its sent by the server as a response to the request status packet.
typedef struct{
    mc_string jsonResponse; // Response with the server banner and extra information.
} out_ResponseStatusPacket;

// This packet its sent by the server as a response to the ping request.
typedef struct{
    mc_long payload; // The same payload sent by the client.
} out_PongStatusPacket;

typedef struct {
    mc_string reason;
} out_LoginDisconnectPacket;

typedef struct {
    char uuid[37]; 
    mc_string username;
} out_LoginSuccessPacket;


typedef struct {
    mc_int entityId;
    mc_ubyte gamemode;
    mc_int dimension;
    mc_int maxPlayers;
    mc_string levelType;
    mc_int viewDistance;
    mc_boolean reducedDebugInfo;
} out_JoinGamePacket;


typedef struct {
    enum SessionStatus status;
    Player player;
} SessionCtx;

void createPlayer(Player *player, char *username);

/**
 * Serialization functions
 */

// Read a varint from a file descriptor.
int mcVarintRead(Buffer *buffer, mc_int *buf);
// Serialize a value into a varint and save the byte array into buf.
int mcVarintWrite(mc_int value, char *buf);

// Read a string from a file descriptor. The string is saved on the heap.
int mcStringRead(Buffer *buffer, mc_string *buf);
// Write a string into a file descriptor.
void mcStringWrite(Buffer *buffer, char *string);

// Initialize a packet structure.
void mcPacketCreate(MCPacket *packet);
// Read a packet from a file descriptor. The data is allocated in the heap.
int mcPacketRead(Buffer *buffer, MCPacket *buf);
// Write a packet into a file descriptor.
void mcPacketWrite(Buffer *buffer, MCPacket *packet);
// Release resources of a packet.
void mcPacketDestroy(MCPacket *packet);

/**
 * Serialization and deserialization of packets
 */
void readHandshakingPacket(MCPacket *inputPacket, in_HandshakePacket *packet);


void readPingPacket(MCPacket *inputPacket, in_PingStatusPacket *packet);

void writePongPacket(MCPacket *packet, out_PongStatusPacket *pongPacket);

void writeStatusResponsePacket(MCPacket *packet, out_ResponseStatusPacket *statusResponse);


void readLoginStart(MCPacket *inputPacket, in_LoginStartPacket *packet);

void writeLoginSuccess(MCPacket *packet, out_LoginSuccessPacket *loginSuccessPacket);


void writeJoinGame(MCPacket* packet, out_JoinGamePacket* joinGamePacket);
#endif