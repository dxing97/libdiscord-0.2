//
// Created by dxing97 on 7/31/17.
//

#include "http.h"
/*
 * function derived from Ulfius example program: request_example/client.c
 */
char * print_map(const struct _u_map * map) {
    char * line, * to_return = NULL;
    const char **keys;
    int len, i;
    if (map != NULL) {
        keys = u_map_enum_keys(map);
        for (i=0; keys[i] != NULL; i++) {
            len = snprintf(NULL, 0, "key is %s, value is %s\n", keys[i], u_map_get(map, keys[i]));
            line = o_malloc((len+1)*sizeof(char));
            snprintf(line, ((size_t) len+1), "key is %s, value is %s\n", keys[i], u_map_get(map, keys[i]));
            if (to_return != NULL) {
                len = (int) (strlen(to_return) + strlen(line) + 1);
                to_return = o_realloc(to_return, (len+1)*sizeof(char));
            } else {
                to_return = o_malloc((strlen(line) + 1)*sizeof(char));
                to_return[0] = 0;
            }
            strcat(to_return, line);
            o_free(line);
        }
        return to_return;
    }
    return NULL;

}

void ld_http_print_response(const struct _u_response *rep) {
    if(rep != NULL) {
        char *headers = print_map(rep->map_header);

        char body[rep->binary_body_length + 1];
        strncpy(body, rep->binary_body, rep->binary_body_length);
        body[rep->binary_body_length] = '\0';

        printf("HTTP RESPONSE:\n"
                       "%s\n\n"
                       "%s\n\n"
                       "%s\n"
        , rep->protocol, headers, body);

        free(headers);
    }
}

struct _u_request *ld_http_generate_request_string(struct ld_sessiondata *sd, enum ld_http_verb verb, const char *path, const char *body) {
    struct _u_request *req;
    char *tmp;
    struct _u_map headers;

    req = malloc(sizeof(struct _u_request));
    ulfius_init_request(req);
    if(req == NULL) {
        perror("couldn't allocate memory for request");
        return NULL;
    }
    switch (verb) {
        case LD_HTTP_POST:
            req->http_verb = strdup("VERB");
            break;
        case LD_HTTP_GET:
            req->http_verb = strdup("GET");
            break;
        case LD_HTTP_PUT:
            req->http_verb = strdup("PUT");
            break;
        case LD_HTTP_PATCH:
            req->http_verb = strdup("PATCH");
            break;
        case LD_HTTP_DELETE:
            req->http_verb = strdup("DELETE");
            break;
        case LD_HTTP_ERROR:
        default:
            fprintf(stderr, "invalid HTTP verb (%d)", verb);
            return NULL;
    }

    if(path == NULL) {
        fprintf(stderr, "path not specified\n");
        return NULL;
    }
    tmp = malloc(strlen(path) + strlen(LD_API_BASE_URL) + 1);
    strcpy(tmp, LD_API_BASE_URL);
    strcat(tmp, path);
    req->http_url = strdup(tmp);
    free(tmp);

    req->timeout = 20;

    u_map_init(&headers);
    tmp = malloc(strlen("Bot ") + strlen(sd->bot_token) + 1);
    tmp = strcpy(tmp, "Bot ");
    tmp = strcat(tmp, sd->bot_token);
    u_map_put(&headers, "Authorization", tmp);
    tmp = malloc(1000 * sizeof(char));
    snprintf(tmp, 1000, "DiscordBot ($%s, $v%d.%d.%d)", LD_META, LD_VERSION_MAJOR, LD_VERSION_MINOR, LD_VERSION_PATCH);
    u_map_put(&headers, "User-Agent", tmp);
    free(tmp);

    u_map_copy_into(req->map_header, &headers);

    if(body != NULL && (verb == LD_HTTP_GET || verb == LD_HTTP_DELETE)) {
        fprintf(stderr, "request verb (%d) given unexpected HTTP body to send\n", verb);
    }
    //the body MUST be encoded in UTF-8 for this to work
    if (body != NULL) {
        req->binary_body = strdup(body);
        req->binary_body_length = strlen(body);
    }

//    printf("headers: %s\n", print_map(req->map_header));

    return req;
}