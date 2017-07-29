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
            {"help", no_argument, 0, 'h'},
            {0,0,0,0}
        };

        int option_index = 0;

        c = getopt_long(argc, argv, "b:c:h", long_options, &option_index);
        if(c == -1)
            break;

        switch(c) {
            case 'b':
                cfgdat.bot_token = strdup(optarg);
                break;
            case 'c':
                cfgdat.config_path = strdup(optarg);
                break;
            case 'h':
                printf("The second libdiscord test bot.\n"
                               "%s [-b bot_token][-c config_path][-h]\n\n"
                       "Options: \n\t"
                               "-b, --bot-token [bot_token]\n\t\t"
                                   "manual override for bot token, will ignore config file token\n\t"
                               "-c, --config-path [config_path]\n\t\t"
                                   "specify config file path\n\t"
                               "-h, --help\n\t\t"
                                   "displays this help dialog\n", argv[0]);
                return 0;
            default:
                fprintf(stdout, "use --help for options\n");
                exit(1);
        }
    }
    ld_config_read_file(&cfgdat);

    //note that however we get config data, it MUST be initialized to 0.


}