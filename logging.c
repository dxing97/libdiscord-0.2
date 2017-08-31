//
// Created by dxing97 on 8/1/17.
//


#include "logging.h"

const char *ld_loglevel_string(enum ld_loglevel ll) {
    switch(ll) {
        case ld_error:
            return "ERROR";
        case ld_warning:
            return "WARNING";
        case ld_info:
            return "INFO";
        case ld_notice:
            return "NOTICE";
        case ld_debug:
            return "DEBUG";
        default:
            return "UNKNOWN(?)";
    }
}

void ld_log(enum ld_loglevel ll, struct ld_sessiondata *sd, const char *log_message, ... ) {
    /*
     * determine if we have the log level set to print this message
     * construct the log string
     *      [timestamp] LD_{log type}: {log message}\n
     * print the log string
     *
     * ld_log(ld_error, sd, "Recieved non-200(%ld) response from Discord\n:", rep.status);
     * [Fri Aug 18 22:39:18 2017] LD_ERROR: "Recieved non-200(403) response from Discord
     *
     */
    if((ll & sd->log_level) == 0) {
        return;
    }

    time_t rawtime;
    struct tm *currenttime;
    char *message, *time_string;
    //[2017/08/18 19:19:18]

    //time part
    time_string = malloc(30 * sizeof(char));
    time(&rawtime);
    currenttime = localtime(&rawtime);
    asctime_r(currenttime, time_string);
    time_string[24] = '\0';

    //message part
    char msg[4096];
    int ret;
    va_list aptr;

    va_start(aptr, log_message);
    ret = vsprintf(msg, log_message, aptr);
    va_end(aptr);
    if(ret < 0){
        perror("couldn't create log message");
        return;
    }

    message = malloc(strlen(time_string) + strlen(ld_loglevel_string(ll)) + strlen(msg) + 15);
    sprintf(message, "[%s] LD_%s: %s\n", time_string, ld_loglevel_string(ll), msg);
    fprintf(stderr, "%s\n", message);


    free(message);
};

void ld_error_json_type(struct ld_sessiondata *sd, const char *object, const char *expected_type) {
    ld_log(ld_error, sd, "%s isn't of type %s\n", object, expected_type);
//    fprintf(stderr, "%s isn't of type %s\n", object, expected_type);
}

void ld_error_json_dne(struct ld_sessiondata *sd, const char *object, const char *location) {
    ld_log(ld_error, sd, "%s wasn't found at %s\n", object, location);
//    fprintf(stderr, "%s wasn't found at %s\n", object, location);
}