//
// Created by dxing97 on 7/28/17.
//

/*
 * high level prototypes and typedefs
 * library-specific implementations like jansson and lws shouldn't be visible here
 */

#ifndef LIBDISCORDV0_1_LIBDISCORD_H
#define LIBDISCORDV0_1_LIBDISCORD_H

#include "libdiscordConfig.h"

/*
 * a struct containing all information needed to maintain a bot connection to discord
 */
struct ld_sessiondata {
    char *bot_token;
    char *
};


/*
 * reads configdata and initializes some parameters needed to get the bot going, including:
 * verifying the validity of our bot token using the GET /gateway/bot endpoint
 * allocate memory for our global sessiondata variable
 * initialize send queue for websockets and HTTP
 */
struct ld_sessiondata *ld_init_session(struct ld_configdata);

#endif //LIBDISCORDV0_1_LIBDISCORD_H
