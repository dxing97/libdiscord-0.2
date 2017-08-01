//
// Created by dxing97 on 7/31/17.
//

#include "libdiscord.h"

struct ld_sessiondata *ld_init_gateway_bot(struct ld_sessiondata *sd, const char *bot_token) {
    /*
     * send a GET request to /gateway/bot and determine:
     *  bot token validity
     *  gateway URI
     *  shard number
     */
    struct _u_request *req;
    struct _u_response rep;
    int ret;
    req = ld_http_generate_request_string(LD_HTTP_GET, "/gateway/bot", NULL);

    ret = ulfius_send_http_request(req, &rep);
    if(ret != U_OK || &rep == NULL) {
        perror("Couldn't send HTTP GET request to /gateway/bot");
        return NULL;
    }

    ld_http_print_response(&rep);

    if(rep.status != 200) {
        fprintf(stderr, "Recieved non-200(%ld) response from Discord: is the bot token valid?\n", rep.status);
        return NULL;
    }

    //the bot token is valid
    sd->bot_token = strdup(bot_token);

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
        fprintf(stderr, "couldn't find gateway URL in /gateway/bot response body!n");
        return NULL;
    }

    sd.





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

    if(ld_init_gateway_bot(&_sd, cfgdat->bot_token) == NULL) {
        //token isn't valid
    }
}