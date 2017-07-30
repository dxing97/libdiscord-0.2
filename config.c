//
// Created by dxing97 on 7/28/17.
//
#include "config.h"
int ld_config_read_file(struct ld_configdata *cfg) {
    config_t conf;
    config_init(&conf);
    char yesno;
    char *path;
    char *default_path = "bot.cfg";
    const char *bot_token;

    if(cfg->config_path == NULL) {
        printf("config path not specified, using default (bot.cfg)\n");
        path = default_path;
    } else {
        path = cfg->config_path;
    }

    if (config_read_file(&conf, path) == CONFIG_FALSE) {
        perror("error opening config");
        config_error_text(&conf);
        config_error_type(&conf);

        if(access(path, F_OK) == -1) {
            printf("create template config file at %s?[Y/n]:", path);
            scanf("%c", &yesno);
            if (yesno == 'Y') {
                ld_config_create_default_config(cfg);
            } else if (yesno == 'n') {
                exit(0);
            } else {
                printf("invalid option\n");
                exit(1);
            }
        }


        //don't ask the user for a new path: specify it through options instead
        return 1;
    }
    if(cfg->bot_token == NULL) {
        if(config_lookup_string(&conf, "bot_token", &bot_token) == CONFIG_FALSE) {
            fprintf(stderr, "couldn't find bot token in config\n");
            return 1;
        }
        cfg->bot_token = strdup(bot_token);
    } else {
        printf("using command line argument bot token, ignoring config file bot token\n");
    }

    return 0;
}


int ld_config_create_default_config(struct ld_configdata *cfg) {
    cfg->bot_token = NULL;
    printf("this doesn't work yet\n");
    return 0;
}