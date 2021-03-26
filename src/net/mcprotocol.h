#ifndef _MCPROTOCOL
#define _MCPROTOCOL

#include <stdint.h>
#include "utils/buffer.h"

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
    REQUEST = 0x0,
    PING = 0x1
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


/**
 * Outbound packets
 */

enum OutboundStatusPackets {
    RESPONSE = 0x0,
    PONG = 0x1
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
    enum SessionStatus status;
} SessionCtx;

/**
 * Serialization functions
 */

// Read a varint from a file descriptor.
int readVarint(Buffer *buffer, mc_int *buf);
// Serialize a value into a varint and save the byte array into buf.
int serializeIntoVarint(mc_int value, char *buf);

// Read a string from a file descriptor. The string is saved on the heap.
int readString(Buffer *buffer, mc_string buf);
// Write a string into a file descriptor.
int writeString(Buffer *buffer, char *string);

// Initialize a packet structure.
int createPacket(MCPacket *packet);
// Read a packet from a file descriptor. The data is allocated in the heap.
int readPacket(Buffer *buffer, MCPacket *buf);
// Write a packet into a file descriptor.
int writePacket(Buffer *buffer, MCPacket *packet);
// Release resources of a packet.
void releasePacket(MCPacket *packet);

/**
 * Serialization and deserialization of packets
 */
int readHandshakingPacket(MCPacket *inputPacket, in_HandshakePacket *packet);


int readPingPacket(MCPacket *inputPacket, in_PingStatusPacket *packet);

int writePongPacket(MCPacket *packet, out_PongStatusPacket *pongPacket);

int writeStatusResponsePacket(MCPacket *packet, out_ResponseStatusPacket *statusResponse);

#endif