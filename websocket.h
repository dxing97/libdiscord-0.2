//
// Created by dxing97 on 8/3/17.
//

#ifndef LIBDISCORDV0_1_WEBSOCKET_H
#define LIBDISCORDV0_1_WEBSOCKET_H
#define LD_MAX_DISCORD_PAYLOAD 4096
#include "libdiscord.h"
#include <libwebsockets.h>
/*
 * lws callback
 */
enum connectionstate {
    LD_GATEWAY_UNCONNECTED = 0,
    LD_GATEWAY_DISCONNECTED = 1,
    LD_GATEWAY_CONNECTING = 2,
    LD_GATEWAY_CONNECTED = 3
};

enum ld_opcode {
    //discord dispatch opcodes as of API v6
            LD_OPCODE_NO_OP = -1,
    LD_OPCODE_DISPATCH = 0,
    LD_OPCODE_HEARTBEAT = 1,
    LD_OPCODE_IDENTIFY = 2,
    LD_OPCODE_STATUS_UPDATE = 3,
    LD_OPCODE_VOICE_STATE_UPDATE = 4,
    LD_OPCODE_VOICE_SERVER_PING = 5,
    LD_OPCODE_RESUME = 6,
    LD_OPCODE_RECONNECT = 7,
    LD_OPCODE_REQUEST_GUILD_MEMBERS = 8,
    LD_OPCODE_INVALID_SESSION = 9,
    LD_OPCODE_HELLO = 10,
    LD_OPCODE_HEARTBEAT_AK = 11
};

struct ld_lws_sessiondata {
    enum connectionstate state;
    unsigned char buf[LWS_PRE + LD_MAX_DISCORD_PAYLOAD];
    unsigned int close_code;
    struct lws *websocket;
    struct lws_context *context;
    char *payload;
};

extern struct lws_protocols protocols[];

extern struct lws_extension exts[];

/*
 * the lws user callback function
 * depending on the callback reason:
 *      writable:
 *          writes whatever is in the gsd->payload, then frees the payload there's no error
 *      receive:
 *          for any incoming message, it places the char payload into the receive queue
 *      peer_init_close:
 *          save the close code and the close reason in the gsd, then terminate the session
 *          the WSQ isn't cleared, but the current gsd->payload is copied to the front of the WSQ then freed
 *      client_connection
 *
 */
int _ld_ws_callback(struct lws *wsi, enum lws_callback_reasons reason,
                    void *user, void *in, size_t len);

/*
 * Initializes libwebsocket parameters for our connection to Discord
 */
struct ld_sessiondata *ld_init_lws(struct ld_sessiondata *sd);

/*
 * calls lws_client_context_via_info to connect to Discord gateway
 */
int ld_gateway_connect(struct ld_sessiondata *sd);

/*
 * while there are payloads in the queue:
 *      put them in the gsd->payload and call ld_gateway_write
 * calls lws_service for reading and writing from the gateway
 */
int ld_service_gateway(struct ld_sessiondata *sd);

/*
 * calls lws_callback_on_writable
 */
int ld_gateway_write(struct ld_sessiondata *sd);

/*
 * queues an identify payload into the gateway send queue at the beginning of the queue
 */
int ld_gateway_identify(struct ld_sessiondata *sd);

/*
 * appends the char buffer payload to the end of the queue
 * todo: consider adding options to place something at the beginning/end of the queue
 */
int ld_gateway_queuewrite(struct ld_sessiondata *sd, char *payload);

#endif //LIBDISCORDV0_1_WEBSOCKET_H
