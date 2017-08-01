//
// Created by dxing97 on 7/31/17.
//

#ifndef LIBDISCORDV0_1_HTTP_H
#define LIBDISCORDV0_1_HTTP_H

#include "libdiscord.h"
#include <ulfius.h>

enum ld_http_verb {
    LD_HTTP_ERROR = -1,
    LD_HTTP_POST = 0,
    LD_HTTP_GET = 1,
    LD_HTTP_PUT = 2,
    LD_HTTP_PATCH = 3,
    LD_HTTP_DELETE = 4
};

/*
 * prints the raw HTTP response
 */
void ld_http_print_response(const struct _u_response *rep);
/*
 * generates a HTTP request to Discord. Doesn't send the request: put it in the send queue or call ulfius directly.
 */
struct _u_request *ld_http_generate_request_string(struct ld_sessiondata *sd, enum ld_http_verb verb, const char *path, const char *body);

/* todo
 * same thing as ld_http_generate_request_string but takes a JSON instead for the body
 */
struct _u_request *ld_http_generate_request_json(enum ld_http_verb, const char *path, json_t *body);
#endif //LIBDISCORDV0_1_HTTP_H
