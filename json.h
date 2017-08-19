//
// Created by dxing97 on 7/31/17.
//

#ifndef LIBDISCORDV0_1_JSON_H
#define LIBDISCORDV0_1_JSON_H

#include <jansson.h>
#include "libdiscord.h"

void ld_json_errorhandler(struct ld_sessiondata *sd, json_error_t *error, const char *context);
#endif //LIBDISCORDV0_1_JSON_H
