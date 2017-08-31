//
// Created by dxing97 on 8/3/17.
//

#include "websocket.h"

int _ld_ws_callback(struct lws *wsi, enum lws_callback_reasons reason,
                    void *user, void *in, size_t len) {
    struct ld_sessiondata *sd = user;

    switch(reason) {
        default:
            ld_log(ld_debug, sd, "unhandled lws callback reason: %d", reason);
            break;
    }

    return 0;
}

struct lws_protocols protocols[] = {
        {
                "DiscordAPIv6",
                _ld_ws_callback,
                sizeof(struct ld_lws_sessiondata),
                LD_MAX_DISCORD_PAYLOAD,
        },
        {
                NULL,NULL,0 /* end of list */
        }
};

struct lws_extension exts[] = {
        {
                "permessage-deflate",
                lws_extension_callback_pm_deflate,
                "permessage-deflate; client_no_context_takeover; client_max_window_bits"
        },
        {
                NULL ,NULL, NULL /*terminator*/
        }
};

struct ld_sessiondata *ld_init_lws(struct ld_sessiondata *sd) {
    struct lws_context *ret;
    struct lws_context_creation_info info;
    memset(&info, 0, sizeof(info));

    lws_set_log_level(sd->log_level, NULL);

    info.port = CONTEXT_PORT_NO_LISTEN;
    info.protocols = protocols;
    info.extensions = exts;
    info.gid = -1;
    info.uid = -1;
    info.options =
        0 |
        LWS_SERVER_OPTION_VALIDATE_UTF8 |
        LWS_SERVER_OPTION_DO_SSL_GLOBAL_INIT;

    ret = lws_create_context(&info);
    if(ret == NULL) {
        ld_log(ld_error, sd, "couldn't create LWS context\n");
        return NULL;
    }
    sd->ws_context = ret;

    return sd;
}