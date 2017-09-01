//
// Created by dxing97 on 7/28/17.
//

/*
 * high level prototypes and typedefs
 * library-specific implementations like jansson and lws shouldn't be visible here
 */

#ifndef LIBDISCORDV0_1_LIBDISCORD_H
#define LIBDISCORDV0_1_LIBDISCORD_H

#define LD_API_BASE_URL "https://discordapp.com/api/v6"

#include <sys/queue.h>
/*
 * a struct containing all information needed to maintain a bot connection to discord
 * bot_token:               string containing our bot token
 * gateway_url:             string containing the complete URL of the gateway returned by GET /gateway
 * gateway_shard_url:       string containing the complete URL of the gateway returned by GET /gateway/bot
 * shard_number:            number of shards returned by GET /gateway/bot
 * last_gateway_connect:    the time at which we last connected to a gateway (max. is LD_GATEWAY_CONNECTION_LIMIT)
 *
 */

#include "websocket.h"


struct ld_sessiondata {
    char *bot_token;
    char *gateway_url;
    char *gateway_shard_url;
    int shard_number;
    TAILQ_HEAD(, ld_hsq_entry) ld_hsq_head;
    long last_gateway_connect;
    unsigned int log_level;
    struct ld_lws_sessiondata *gsd;
};
#include "json.h"
#include "http.h"
#include "websocket.h"
#include "config.h"

#include "logging.h"

#include "libdiscordConfig.h"
#include "libdiscord_private.h"


/*
 * makes a GET request to /gateway for the gateway URL (non-sharded)
 */
struct ld_sessiondata *ld_init_gateway(struct ld_sessiondata *sd);

/*
 * reads configdata and initializes some parameters needed to get the bot going, including:
 * verifying the validity of our bot token using the GET /gateway/bot endpoint
 * allocate memory for our global sessiondata variable
 * initialize send queue for websockets and HTTP
 */
struct ld_sessiondata *ld_init_sessiondata(struct ld_configdata *cfgdat);

/*
 * sends GET request to /gateway/bot and checks for valid response
 * sessiondata must already have the bot token initialized
 */
struct ld_sessiondata * ld_init_gateway_bot(struct ld_sessiondata *sd);

/*
 * frees sessiondata and components, most significantly the lws context
 */
void ld_close_sessiondata(struct ld_sessiondata *sd);

/*
 * opens the websocket connection
 * starts the HSQ send thread
 */
int ld_begin(struct ld_sessiondata *sd);

/*
 * sends anything in the HSQ and WSQ, reads new WSS information
 */
void ld_service(struct ld_sessiondata *sd);

#endif //LIBDISCORDV0_1_LIBDISCORD_H
