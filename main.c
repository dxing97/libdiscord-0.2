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

    cfgdat.log_level = 31;

    while(1) {
        static struct option long_options[] = {
            {"config-path", required_argument, 0, 'c'},
            {"bot-token", required_argument, 0, 'b'},
            {"help", no_argument, 0, 'h'},
            {"log-level", required_argument, 0, 'l'},
            {0,0,0,0}
        };

        int option_index = 0;

        c = getopt_long(argc, argv, "b:c:hl:", long_options, &option_index);
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
            case 'l':
                cfgdat.log_level = (unsigned) atoi(optarg);
                printf("using log level %u\n", cfgdat.log_level);
                break;
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
    printf("log level in cfgdat before init_sessiondata: %d\n", cfgdat.log_level);
    sd = ld_init_sessiondata(&cfgdat);
    if(sd == NULL) {
        printf("error intializing session data\n");
        return 1;
    }

    printf("initialized sessiondata\n");

    //begin looping
    while(!force_exit) {
        /*
        switch(sd.gsd.state) {
            case LD_GATEWAY_UNCONNECTED:
                start a connection
            case LD_GATEWAY_DISCONNECTED:
                depending on the context, start a connection, wait, or something else
            case  LD_GATEWAY_CONNECTING:
                do nothing(?)
            case LD_GATEWAY_CONNECTED:
                write, read, heartbeat, etc.
            default
                hueh? probably exit
        }
         */
        sleep(5);
        ld_log(ld_notice, sd, "slept for 5 seconds, exiting");
        break;
    }

    ld_close_sessiondata(sd);
    return 0;


}