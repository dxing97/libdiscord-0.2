# Bot 2
This bot will connect to the Discord gateway, remain connected, and have it's current game set to the name of this library. (libdiscord v0.1.0)

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
    
## HTTP endpoints
- /channels/{channel.id}
    - /messages
        - /{message.id}
            - /reactions
                - /{emoji}
                    - /@me
                    - /user.id
        - /bulk-delete
    - /permissions/{overwrite.id}
    - /invites
    - /typing
    - /pins
        - /{message.id}
    - /recipients/{user.id}

# rate limiting
### HTTP rate limits
- Exceeding the HTTP rate limit will result in a HTTP 429 response code with a retry-after header.
- There is a global rate limit which is invisible to the client until the client recieves
 a 429 response with the global header set to true, at which point you've already hit the global rate limit.
    - When that happens, the client must stop sending all HTTP requests 
    until the amount of time in retry-after has passed.
- a "route" is the HTTP verb and the HTTP path used when sending a HTTP request to the API
- Each individual HTTP route has its own rate limit, the parameters of which will be passed as the following
HTTP headers:
    - X-RateLimit-Global
        - sent with a HTTP 429 message
    - X-RateLimit-Limit
        - number of requests that you can make
    - X-RateLimit-Remaining
        - number of remaining requests that you can make
    - X-RateLimit-Reset
        - time at which the number of remaining requests resets back to the number of requests
- If two HTTP routes are "related" then they'll share the same rate limit
    - i.e. rate limit headers for one route can be used to determine whether or not a request on a "related" 
    route exceeds the rate limit
- Two routes are "related" if they:
    - are the exact same route
    - the difference between paths is a non-major parameter
        - i.e. `GET /channels/1234/messages/12345`, `GET /channels/1234/messages/67890` and 
        `GET /channels/1234/messages` are related routes
        - the only major parameters are currently channel.id and guild.id
    - have different HTTP verbs and the same path
- Two routes are not "related" if:
    - they have different routes
    - they differ by a major parameter
        i.e. `GET /channels/1234/messages/` and `GET /channels/5678/messages` are unrelated routes

- Exceptions:
    - the DELETE /channel/{channel.id}/message/{message.id} route is not related to any other routes, 
    including those that share the same path but have different verbs
    - All reaction endpoints for the same channel ID are related, regardless of HTTP verb and path
    



### Websocket rate limits
Exceeding the WSS rate limit leads to a payload with opcode 9 (INVALID_SESSION), closure of the websocket,
 and close code 4008 (rate limited)
- last connection attempt
    - the client can only attempt to connect to the gateway every 5 sec
- last 120 send event time
    - the client can attempt to send up to 120 payloads in one minute 
    (on average one every 500 ms)
- last 5 game status event times
    - the client can attempt to send up to 5 game status updates in one minute 
    (on average one every 12 sec)