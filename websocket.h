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

struct ld_lws_sessiondata {
    enum connectionstate state;
    unsigned char buf[LWS_PRE + LD_MAX_DISCORD_PAYLOAD];
};

extern struct lws_protocols protocols[];

extern struct lws_extension exts[];

int _ld_ws_callback(struct lws *wsi, enum lws_callback_reasons reason,
                    void *user, void *in, size_t len);

struct ld_sessiondata *ld_init_lws(struct ld_sessiondata *sd);

#endif //LIBDISCORDV0_1_WEBSOCKET_H
