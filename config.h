//
// Created by dxing97 on 7/28/17.
//

#ifndef LIBDISCORDV0_1_CONFIG_H
#define LIBDISCORDV0_1_CONFIG_H


#include <libconfig.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"

struct ld_configdata {
    char *bot_token;
    char *config_path;
    //more later
};

int ld_config_read_file(struct ld_configdata *cfg);

//todo: create the ld_config_create_default_config function
int ld_config_create_default_config(struct ld_configdata *cfg);

#endif //LIBDISCORDV0_1_CONFIG_H
