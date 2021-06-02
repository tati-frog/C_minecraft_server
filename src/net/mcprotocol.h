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
    PLAY_JOIN_GAME = 0x24,
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
    mc_boolean isHardcore;
    mc_ubyte gamemode;
    mc_byte previousGamemode;
    mc_int worldCount;
    mc_string* worldNames;
    NBT_Tag dimensionCodec;
    NBT_Tag dimension;
    mc_string worldName;
    mc_long hashedSeed;
    mc_int maxPlayers;
    mc_int viewDistance;
    mc_boolean reducedDebugInfo;
    mc_boolean enableRespawnScreen;
    mc_boolean isDeubg;
    mc_boolean isFlat;
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