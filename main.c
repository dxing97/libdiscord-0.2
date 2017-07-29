/*
 * the main bot code here
 * should be mostly functions that do the hard work
 *
 * put other stuff in header files
 *
 */

#include <libconfig.h>
#include <getopt.h>
#include "config.h"

int main(int argc, char *argv[]) {
    struct ld_configdata cfgdat;
    memset(&cfgdat, 0, sizeof(struct ld_configdata));
    int c;
    while(1) {
        static struct option long_options[] = {
            {"config-path", required_argument, 0, 'c'},
            {"bot-token", required_argument, 0, 'b'},
            {0,0,0,0}
        };

        int option_index = 0;

        c = getopt_long(argc, argv, "b:c:", long_options, &option_index);
        if(c == -1)
            break;

        switch(c) {
            case 'b':
                cfgdat.bot_token = strdup(optarg);
                break;
            case 'c':
                cfgdat.config_path = strdup(optarg);
                break;
            default:
                fprintf(stderr, "ouch\n");
                exit(1);
        }
    }
    ld_config_read_file(&cfgdat);

    printf("bot token: %s\n", cfgdat.bot_token);

    
}