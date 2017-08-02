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
#include "http.h"

struct ld_sessiondata {
    char *bot_token;
    char *gateway_url;
    char *gateway_shard_url;
    int shard_number;
    TAILQ_HEAD(, ld_hsq_entry) ld_hsq_head;
    //HSQ (HTTP send queue)
};

#include "config.h"

#include "json.h"
#include "errorhandlers.h"

#include "libdiscordConfig.h"
#include "libdiscord_private.h"
/*
 * a struct containing all information needed to maintain a bot connection to discord
 */


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


#endif //LIBDISCORDV0_1_LIBDISCORD_H
