//
// Created by dxing97 on 8/1/17.
//

#include <stdio.h>
#include "errorhandlers.h"

void ld_error_json_type(const char *object, const char *expected_type) {
    fprintf(stderr, "%s isn't of type %s\n", object, expected_type);
}

void ld_error_json_dne(const char *object, const char *location) {
    fprintf(stderr, "%s wasn't found at %s\n", object, location);
}