//
// Created by dxing97 on 7/31/17.
//

#include <jansson.h>
#include "json.h"
#include "logging.h"

void ld_json_errorhandler(struct ld_sessiondata *sd, json_error_t *error, const char *context) {
    if(error == NULL) {
        perror("errorhandler called but no error\n");
        return;
    }
    ld_log(ld_warning, sd, "JSON error from %s: %s on line %d column %d (position %d)\n"
            "source: %s\n", context, error->text, error->line, error->column, error->position, error->source);

}