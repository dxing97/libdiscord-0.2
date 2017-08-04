//
// Created by dxing97 on 7/31/17.
//

#include "libdiscord.h"
struct ld_sessiondata *ld_init_gateway(struct ld_sessiondata *sd) {
    struct _u_response rep;
    struct _u_request *req;
    int ret;
    req = malloc(sizeof(struct _u_request));
    ulfius_init_response(&rep);
    ulfius_init_request(req);

    req = ld_http_generate_request_string(sd, LD_HTTP_GET, "/gateway", NULL);

    ret = ulfius_send_http_request(req, &rep);
    if(ret != U_OK || &rep == NULL) {
        fprintf(stderr, "couldn't send HTTP GET request to /gateway/bot: ulfius return code %d\n", ret);
        return NULL;
    }

    ld_http_print_response(&rep);
    if(rep.status != 200) {
        fprintf(stderr, "Recieved non-200(%ld) response from Discord\n:", rep.status);
        return NULL;
    }

    json_t *url, *body;
    json_error_t error;

    body = json_loadb(rep.binary_body, rep.binary_body_length, 0, &error);
    if(body == NULL) {
        ld_json_errorhandler(&error, "GET /gateway");
        return NULL;
    }
    url = json_object_get(body, "url");
    if(url == NULL) {
        ld_error_json_dne("gateway URL", "GET /gateway response body");
        return NULL;
    }

    sd->gateway_url = strdup(json_string_value(url));
    if(sd->gateway_url == NULL) {
        ld_error_json_type("gateway URL", "string");
        return NULL;
    }


    return sd;
}

struct ld_sessiondata *ld_init_gateway_bot(struct ld_sessiondata *sd) {
    /*
     * send a GET request to /gateway/bot and determine:
     *  bot token validity
     *  gateway URI
     *  shard number
     */

    TAILQ_HEAD(, ld_hsq_entry) hsq_head;
    struct _u_request *req;
    struct _u_response rep;
    int ret;

    ulfius_init_response(&rep);
    req = ld_http_generate_request_string(sd, LD_HTTP_GET, "/gateway/bot", NULL);

    ret = ulfius_send_http_request(req, &rep);
    if(ret != U_OK || &rep == NULL) {
        fprintf(stderr, "Couldn't send HTTP GET request to /gateway/bot: ulfius return code %d\n", ret);
        return NULL;
    }

    ld_http_print_response(&rep);

    if(rep.status > 299) {
        fprintf(stderr, "Recieved non-200(%ld) response from Discord: is the bot token valid?\n", rep.status);
        return NULL;
    }

    //the bot token is valid

    //parse raw binary data into JSON
    json_t *jbody, *key;
    json_error_t error;
    jbody = json_loadb(rep.binary_body, rep.binary_body_length, 0, &error);
    if(jbody == NULL) {
        ld_json_errorhandler(&error, "GET /gateway/bot");
        return NULL;
    }

    key = json_object_get(jbody, "url");
    if(key == NULL) {
        ld_error_json_dne("gateway bot URL", "/gateway/bot response body");
        return NULL;
    }

    sd->gateway_shard_url = strdup(json_string_value(key));
    if(sd->gateway_shard_url == NULL) {
        ld_error_json_type("gateway bot URL", "string");
        return NULL;
    }

    key = json_object_get(jbody, "shards");
    if(key == NULL) {
        ld_error_json_dne("gateway shard number", "/gateway/bot response body");
        return NULL;
    }

    sd->shard_number = (int) json_integer_value(key);
    if(sd->shard_number == 0) {
        //while Discord _could_ return 0 for shard number, that wouldn't make any sense to do so
        ld_error_json_type("gateway shard number", "integer");
        return NULL;
    }

    //Initialize the HTTP Send Queue (HSQ)
    TAILQ_INIT(&hsq_head);

    return sd;
}

struct ld_sessiondata *ld_init_sessiondata(struct ld_configdata *cfgdat) {
    /*
     * determine validity of bot token, retrieve the gateway URI and shard number.
     */
    memset(&_sd, 0, sizeof(struct ld_sessiondata));

    if(cfgdat->bot_token == NULL) {
        fprintf(stderr, "bot token not specified\n");
        return NULL;
    }

    _sd.bot_token = strdup(cfgdat->bot_token);
    if(ld_init_gateway_bot(&_sd) == NULL) {
        //token isn't valid or something went wrong with the API response
        fprintf(stderr, "bad response from /gateway/bot\n");
        return NULL;
    }

    if(ld_init_gateway(&_sd) == NULL) {
        fprintf(stderr, "bad response from /gateway\n");
        return NULL;
    }

    if(ld_init_lws(&_sd) == NULL) {
        fprintf(stderr, "couldn't initialize lws\n");
        return NULL;
    }

    return &_sd;
}
int ld_begin(struct ld_sessiondata *sd) {
    //open the websocket connection and return the lws
    ;
}
void ld_close_sessiondata(struct ld_sessiondata *sd) {
    ;
}