//
// Created by dxing97 on 8/1/17.
//

#ifndef LIBDISCORDV0_1_ERRORHANDLERS_H
#define LIBDISCORDV0_1_ERRORHANDLERS_H

/*
 * "{object} isn't a {type}\n"
 */
void ld_error_json_type(const char *object, const char *expected_type);

/*
 * "{object} wasn't found in {location}\n"
 */
void ld_error_json_dne(const char *object, const char *location);

#endif //LIBDISCORDV0_1_ERRORHANDLERS_H
