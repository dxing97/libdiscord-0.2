//
// Created by dxing97 on 8/3/17.
//

#include "websocket.h"

int _ld_ws_callback(struct lws *wsi, enum lws_callback_reasons reason,
                    void *user, void *in, size_t len) {
    struct ld_sessiondata *sd = _sd;
    int i = 10;
    char *payload = user;
    switch(reason) {
        case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
            ld_log(ld_error, sd, "error trying to connect to the gateway: %.*s", len, in);
            sd->gsd->state = LD_GATEWAY_DISCONNECTED;
            break;
        case LWS_CALLBACK_CLIENT_FILTER_PRE_ESTABLISH:
            ld_log(ld_info, sd, "received handshake from Discord gateway");
//            ld_log(ld_debug, sd, "header information:\n%.*s", len, in);
            break;
        case LWS_CALLBACK_CLIENT_ESTABLISHED:
            ld_log(ld_info, sd, "LWS callback: connected to Discord gateway");
            sd->gsd->state = LD_GATEWAY_CONNECTING;
            break;
        case LWS_CALLBACK_CLOSED:
            ld_log(ld_warning, sd, "Discord gateway connection closed");
            sd->gsd->state = LD_GATEWAY_DISCONNECTED;
            break;
        case LWS_CALLBACK_CLIENT_RECEIVE:
            ld_log(ld_info, sd, "received payload from Gateway");
            ld_log(ld_debug, sd, "gateway recieved payload:\n%.*s", len, in);
            ld_log(ld_debug, sd, "payload opcode: %d", ld_json_get_opcode_buffer(sd, in, len));
            switch(ld_json_get_opcode_buffer(sd, in, len)) {
                case LD_OPCODE_NO_OP:
                    break;
                case LD_OPCODE_HELLO:
                    break;
                case LD_OPCODE_DISPATCH:
                    /*
                     * if the JSON d(object) contains object keyvalue "content" with value "ayy"
                     *      send HTTP POST Message in Proving Grounds/#singular-one  with message content "lmao"
                     */
                    if(ld_json_opcode_ayydetect(sd, in, len) != -1) {
                        struct _u_request *req;
                        struct _u_response *rep;

                        rep = malloc(sizeof(struct _u_response));
//                        req = malloc(sizeof(struct _u_request));
                        req = ld_http_generate_request_string(sd, LD_HTTP_POST,
                                                              "/channels/342013131121229824/messages",
                                                              "{\"attachments\":[],\"tts\":false,\"embeds\":[],"
                                                                      "\"timestamp\":\"2017-07-11T17:27:07.299000+00:00\","
                                                                      "\"mention_everyone\":false,\"id\":\"334385199974967042\","
                                                                      "\"pinned\":false,\"edited_timestamp\":null,"
                                                                      "\"author\":{\"username\":\"Daxe\","
                                                                      "\"discriminator\":\"1955\",\"id\":\"213084722123767809\","
                                                                      "\"avatar\":\"a_bab14f271d565501444b2ca3be944b25\"},"
                                                                      "\"mention_roles\":[],"
                                                                      "\"content\":\"lmao\","
                                                                      "\"channel_id\":\"290926798999357250\",\"mentions\":[],\"type\":0}"

                        );

//                        usleep(500000);
                        ulfius_send_http_request(req, rep);

                        ld_http_print_response(sd, rep);
//                    free(req);
//                    free(rep);
                    }

                    break;
                default:
                    break;
            }
            /*
             * case: recieved payload type
             *     dispatch (opcode 0)
             *          if it's a message object and object field "content" is "ayy"
             *              send "lmao" to that channel
            *           save the sequence number (s)
             *
             */
            break;
        case LWS_CALLBACK_CLIENT_WRITEABLE:
            ld_log(ld_info, sd, "socket writable");
            //payload is in sd->gsd->payload
            if(sd->gsd->payload == NULL) {
                ld_log(ld_warning, sd, "LWS_CALLBACK_CLIENT_WRITABLE: nothing in sd->gsd->payload to send");
                break;
            }

            i = sprintf(payload + LWS_PRE, "%s", sd->gsd->payload); //todo: segfault here
            if(i <= 0) {
                ld_log(ld_error, sd, "couldn't write JSON payload to buffer");
                return -1;
            }
            lwsl_notice("TX: %s\n", payload + LWS_PRE);
            i = lws_write(wsi, (unsigned char *) (payload + LWS_PRE), strlen(sd->gsd->payload), LWS_WRITE_TEXT);
            if(i < 0) {
                lwsl_err("ERROR %d writing to socket, hanging up\n", i);
                return -1;
            }
            if(i < strlen(sd->gsd->payload)) {
                lwsl_err("Partial write\n");
                return -1;
            }
            free(sd->gsd->payload);
            sd->gsd->payload = NULL;
            break;
        case LWS_CALLBACK_WS_PEER_INITIATED_CLOSE:
            ld_log(ld_warning, sd, "peer initiated close!");
            ld_log(ld_debug, sd, "peer init close: close code: %u\nCONTEXT:\n%s", (unsigned int) (( unsigned char *)in)[0] << 8 | (( unsigned char *)in)[1], in+2);
            sd->gsd->close_code = (unsigned int) (( unsigned char *)in)[0] << 8 | (( unsigned char *)in)[1];
            break;
        default:
//            ld_log(ld_debug, sd, "unhandled lws callback reason: %d", reason);
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
    sd->gsd = malloc(sizeof(struct ld_lws_sessiondata));
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
    sd->gsd->context = ret;

    return sd;
}

int ld_gateway_connect(struct ld_sessiondata *sd) {
    /*
     * connect to the gateway and do initial gateway stuff
     * recieve hello, send identify, send heartbeat
     */
    //open the websocket connection and return the lws
    sd->gsd->state = LD_GATEWAY_CONNECTING;
    struct lws_client_connect_info *info;
    info = malloc(sizeof(struct lws_client_connect_info));
    memset(info, 0, sizeof(struct lws_client_connect_info));

    //todo: finish connection info
    info->context  = sd->gsd->context;

    if(strspn(sd->gateway_shard_url, "wss://") == 6)
        info->address = strdup(sd->gateway_shard_url + 6);
    else
        info->address = strdup(sd->gateway_shard_url);

    char *ads_port;
    ads_port = malloc((strlen(info->address) + 10) * sizeof(char));
    sprintf(ads_port, "%s:%u", info->address, 443&65535);
    info->port = 443;
    info->ssl_connection = 1;
    info->path = "/?v=" LD_GATEWAY_VERSION "&encoding=" LD_GATEWAY_ENCODING;
    info->host = ads_port;
    info->origin = ads_port;
    info->protocol = protocols[0].name;

    struct lws *ret;
    ret = lws_client_connect_via_info(info);
    if(ret == NULL)
        return 1;
    sd->gsd->websocket = ret;
//    sd->gsd->state = LD_GATEWAY_CONNECTING;

//    lws_client_connect_via_info()

    return 0;
}

int ld_service_gateway(struct ld_sessiondata *sd) {
    lws_service(sd->gsd->context, 20);
    return 0;
}

int ld_gateway_write(struct ld_sessiondata *sd) {
    lws_callback_on_writable(sd->gsd->websocket);
    return 0;
}

int ld_gateway_identify(struct ld_sessiondata *sd) {
    return 0;
}

