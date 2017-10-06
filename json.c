//
// Created by dxing97 on 7/31/17.
//
#include "json.h"

void ld_json_errorhandler(struct ld_sessiondata *sd, json_error_t *error, const char *context) {
    if(error == NULL) {
        perror("errorhandler called but no error\n");
        return;
    }
    ld_log(ld_warning, sd, "JSON error from %s: %s on line %d column %d (position %d)"
            "source: %s\n", context, error->text, error->line, error->column, error->position, error->source);

}

json_t* ld_json_create_identify(struct ld_sessiondata *sd) {
    json_t *payload, *data, *properties, *presence, *game;
    char lv[128];
    sprintf(lv, "%s v%d.%d.%d", LD_NAME, LD_VERSION_MAJOR, LD_VERSION_MINOR, LD_VERSION_PATCH);

    payload = json_object();
    if (payload == NULL) {
        ld_log(ld_error, sd, "couldn't create payload JSON object for IDENTIFY payload\n");
        return NULL;
    }

    data = json_object();
    if (data == NULL) {
        ld_log(ld_error, sd, "couldn't create data JSON object for IDENTIFY payload\n");
        return NULL;
    }

    properties = json_object();
    if (properties == NULL) {
        ld_log(ld_error, sd, "couldn't create properties JSON object for IDENTIFY payload\n");
        return NULL;
    }

    presence = json_object();
    if(presence == NULL) {
        ld_log(ld_error, sd, "couldn't create presence JSON object for IDENTIFY payload\n");
    }

    game = json_object();
    if(game == NULL) {
        ld_log(ld_error, sd, "couldn't create game JSON object for IDENTIFY payload\n");
    }

    json_object_set_new(payload, "op", json_integer(LD_OPCODE_IDENTIFY));
    json_object_set(payload, "d", data);

    json_object_set_new(data, "token", json_string(sd->bot_token));
    json_object_set_new(data, "compress", json_boolean(0));
    json_object_set_new(data, "large_threshold", json_integer(250));
    json_object_set(data, "properties", properties);
    json_object_set(data, "presence", presence);

    json_object_set_new(properties, "$os", json_string("linux"));
    json_object_set_new(properties, "$browser", json_string(lv));
    json_object_set_new(properties, "$device", json_string(lv));
    json_object_set_new(properties, "$referrer", json_string(""));
    json_object_set_new(properties, "$referring_domain", json_string(""));

    json_object_set_new(presence, "status", json_string("online"));
    json_object_set_new(presence, "since", json_null());
    json_object_set_new(presence, "afk", json_false());
    json_object_set(presence, "game", game);

    json_object_set_new(game, "name", json_string(LD_NAME));
    json_object_set_new(game, "type", json_integer(1));
    json_object_set_new(game, "url", json_null());

    return payload;
}

enum ld_opcode ld_json_get_opcode_string(struct ld_sessiondata *sd, const char *payload_string) {
    json_t *payload;
    json_error_t *jerror;
    int ret;
    payload = malloc(sizeof(json_t));
    jerror = malloc(sizeof(json_error_t));

    payload = json_loads(payload_string, 0, jerror);
    if(payload == NULL){
        ld_json_errorhandler(sd, jerror, "loading gateway payload in ld_json_get_opcode_buffer");
        return LD_OPCODE_NO_OP;
    }
    ret = ld_json_get_opcode(sd, payload);

    free(payload);
    free(jerror);
    return ret;
}

enum ld_opcode ld_json_get_opcode_buffer(struct ld_sessiondata *sd, const char *payload_buffer, size_t payload_length) {
    json_t *payload;
    json_error_t *jerror;
    int ret;
    payload = malloc(sizeof(json_t));
    jerror = malloc(sizeof(json_error_t));

    payload = json_loadb(payload_buffer, payload_length, 0, jerror);
    if(payload == NULL){
        ld_json_errorhandler(sd, jerror, "loading gateway payload in ld_json_get_opcode_buffer");
        return LD_OPCODE_NO_OP;
    }
    ret = ld_json_get_opcode(sd, payload);

    free(payload);
    free(jerror);
    return ret;
}

enum ld_opcode ld_json_get_opcode(struct ld_sessiondata *sd, json_t *payload) {
    json_t *opcode;
    opcode = json_object_get(payload, "op");
    if(opcode == NULL || (json_is_integer(opcode) == 0))
        return LD_OPCODE_NO_OP;
    return (int) json_integer_value(opcode);
}

/*
 * returns 1 if buffer JSON payload contains a discord message "ayy"
 * returns 0 otherwise
 */
int ld_json_opcode_ayydetect(struct ld_sessiondata *sd, char *payload_buffer, size_t payload_length){
    json_t *payload, *data, *content;
    json_error_t *jerror;
//    int ret;
    payload = malloc(sizeof(json_t));
    data = malloc(sizeof(json_t));
    content = malloc(sizeof(json_t));
    jerror = malloc(sizeof(json_error_t));

    payload = json_loadb(payload_buffer, payload_length, 0, jerror);
    if(payload == NULL){
        ld_json_errorhandler(sd, jerror, "loading DISPATCH(0) payload in ld_json_get_opcode_buffer");
        return LD_OPCODE_NO_OP;
    }
//    ret = ld_json_get_opcode(sd, payload);
    data = json_object_get(payload, "d");
    if(data == NULL || (json_is_object(data) == 0))
        return LD_OPCODE_NO_OP;

    content = json_object_get(data, "content");
    if(content == NULL || (json_is_string(content) == 0))
        return LD_OPCODE_NO_OP;
    if(strcmp(json_string_value(content), "ayy") != 0) {
        return LD_OPCODE_NO_OP;
    }

    ld_log(ld_info, sd, "detected \"ayy\" message");

    free(content);
    free(data);
    free(payload);
    free(jerror);
    return 0;
}