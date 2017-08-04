/*
 * the main bot code here
 * should be mostly functions that do the hard work
 *
 * put other stuff in header files
 *
 */

#include <libconfig.h>
#include <getopt.h>
#include <signal.h>

#include "libdiscord.h"

static int force_exit = 0;
static void interupt_handler(int signum) {
    force_exit = 1;
}

int main(int argc, char *argv[]) {
    printf("Bot2 startup: using %s v%d.%d.%d\n", LD_NAME, LD_VERSION_MAJOR, LD_VERSION_MINOR, LD_VERSION_PATCH);

    if(signal(SIGINT, interupt_handler) == SIG_ERR) {
        fprintf(stderr, "couldn't set signal handler for SIGINT\n");
        return 1;
    }

    int c;
    struct ld_configdata cfgdat;
    memset(&cfgdat, 0, sizeof(struct ld_configdata));

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
    c = ld_config_read_file(&cfgdat);
    if(c != 0){
        printf("couldn't read and parse config file\n");
        return 1;
    }

    struct ld_sessiondata *sd;
    sd = ld_init_sessiondata(&cfgdat);
    if(sd == NULL) {
        printf("error intializing session data\n");
        return 1;
    }

    printf("initialized sessiondata\n");

    //begin looping
    while(!force_exit) {
        ;
    }

    ld_close_sessiondata(sd);
    return 0;


}