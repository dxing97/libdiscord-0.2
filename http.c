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

        char *body[rep->binary_body_length + 1];
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