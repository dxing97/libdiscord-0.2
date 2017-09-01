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
