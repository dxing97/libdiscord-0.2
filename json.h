//
// Created by dxing97 on 7/31/17.
//

#ifndef LIBDISCORDV0_1_JSON_H
#define LIBDISCORDV0_1_JSON_H

#include <jansson.h>
#include "libdiscord.h"


json_t *ld_json_create_identify(struct ld_sessiondata *sd);

void ld_json_errorhandler(struct ld_sessiondata *sd, json_error_t *error, const char *context);
#endif //LIBDISCORDV0_1_JSON_H

/*
 * takes a payload string and returns the opcode
 */
enum ld_opcode ld_json_get_opcode_string(struct ld_sessiondata *sd, const char *payload_string);

/*
 * takes a payload buffer of length payload_length and returns the opcode
 */
enum ld_opcode ld_json_get_opcode_buffer(struct ld_sessiondata *sd, const char *payload_buffer, size_t payload_length);

/*
 * takes a payload buffer encoded in a json_t and returns the opcode
 */
enum ld_opcode ld_json_get_opcode(struct ld_sessiondata *sd, json_t *payload);

/*
 * returns 1 if key "content" in key "d" contains "ayy"
 * returns 0 otherwise
 */
int ld_json_opcode_ayydetect(struct ld_sessiondata *sd, char *payload_buffer, size_t payload_length);