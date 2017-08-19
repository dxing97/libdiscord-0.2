//
// Created by dxing97 on 8/1/17.
//

#ifndef LIBDISCORDV0_1_ERRORHANDLERS_H
#define LIBDISCORDV0_1_ERRORHANDLERS_H

#include "libdiscord.h"
#include <stdio.h>
#include <stdarg.h>
#include <time.h>

enum ld_loglevel {
    ld_error =     1<<0, //something really bad happened and I'm probably going to exit now
    ld_warning =   1<<1, //something bad is about to happen
    ld_info =      1<<2, //ysk this happened, it might be important
    ld_notice =    1<<3, //this happened
    ld_debug =     1<<4
};

const char *ld_loglevel_string(enum ld_loglevel ll);


void ld_log(enum ld_loglevel ll, struct ld_sessiondata *sd, const char *log_message, ...);

/*
 * "{object} isn't a {type}\n"
 */
void ld_error_json_type(struct ld_sessiondata *sd, const char *object, const char *expected_type);

/*
 * "{object} wasn't found in {location}\n"
 */
void ld_error_json_dne(struct ld_sessiondata *sd, const char *object, const char *location);

#endif //LIBDISCORDV0_1_ERRORHANDLERS_H
