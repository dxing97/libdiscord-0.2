# Bot 2
This bot will connect to the Discord gateway, remain connected, 
    and have it's current game set to the name of this library. (libdiscord v0.1.0 or whatever is in CMakeLists.txt)
It will then respond to any messages in channel #singular-one with a magic string (respond) with the 
current time and library information.

Note that is bot is only compatible with Discord API v6.
## scopes and stuff

### library-defined information:
- library version and name (libdiscord v0.1.0)
  - from CMake
- API details (Discord API v6) such as what port to use, endpoints, base URLs, etc
     

### user defined information:
- config file location (optional)
- bot token (required, read from config file/command line option)

### Discord defined information
(parameters queried/recieved from discord)
- URI(s) for gateways
- heartbeat intervals

## how this bot should work
- get command line options using `getopt`
- 
- read config file from specified file location (if not specified, will assume a default location)
- use libconfig to read config file
- initialize ld_sessiondata for session specific parameters
    - import data from ld_configdata
    - check validity of the bot token here
        - intialize some rate limit parameters as well 
    - initialize the HTTP send queue
    - initialize the websocket send queue
    - if bot token is invalid, kill the bot (~~maybe ask the user to input a new token?~~ notify the user that the bot 
    token is invalid)
    - if _ANYTHING_ is bad/invalid/not what we expected, kill the bot (with appropriate error message)
- create the websocket connection
    - if we can't connect to the gateway, kill the bot (after some error messages)
    - receive the HELLO payload
    - send identify payload
        - if can't identify (i.e. Discord closes the websocket), then let the user decide 
        (why would Discord not accept an identify from a valid bot token?)
    - begin heartbeating, and storing (SQL?)/ignoring recieved payloads
    
## configdata
struct containing all the possible fields in the config file
memset this to 0 before using
- bot_token (optional, if it's not here then it MUST be in the command line args. Can't Discord without a ticket.)
- config_path - path to the config file. 
    - if NULL, it'll look in the default location: current directory, bot.cfg (./bot.cfg)
- more later maybe

# structs 
## ratelimits
there is a WSS and a HTTP rate limit.


## sessiondata
parameters that stay (more or less) constant or persist throughout the lifetime of the bot
- bot token (user specified)
- shard number (Discord specified)
- heartbeat interval (Discord specified)
- websocket gateway URI (Discord specified)
- websocket connection state (library determined)
    - unconnected (start here)
    - connecting
    - connected 
        - not identified
        - identified
    - disconnected (something went wrong, there could be multiple reasons)
    - resuming (resuming and connecting are similar but different)
- last recieved sequence number (dispatch payloads)
    - starts at 0 (haven't recieved a dispatch yet) and increments up with every dispatch (theoretically)
    - determined by DISPATCHES, not the library (don't just increment up)
- pointer to the lws wsi for our current lws connection
- rate limit data
   
# Code Snippets
Sending a HTTP GET request for a bot's user informations:

`
    struct _u_request *req;
    struct _u_response rep;
    ulfius_init_request(req);
    ulfius_init_response(&rep);
    req = ld_http_generate_request_string(sd, LD_HTTP_GET, "/users/@me", NULL);
    ulfius_send_http_request(req, &rep);
    ld_http_print_response(&rep);
`