//
// Created by dxing97 on 7/31/17.
//

#include "libdiscord.h"

struct ld_sessiondata *ld_init_gateway_bot(struct ld_sessiondata *sd) {
    /*
     * send a GET request to /gateway/bot and determine:
     *  bot token validity
     *  gateway URI
     *  shard number
     */
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

    if(rep.status != 200) {
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
        ld_error_json_dne("gateway bot URL", "string");
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
}