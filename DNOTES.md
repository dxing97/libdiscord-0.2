 
## HTTP endpoints
- /gateway
    - /bot
- /oauth2*
    - /applications*
        - /@me
- /channels*
    - /{channel.id}
        - /messages
            - /{message.id}
                - /reactions$
                    - /{emoji}
                        - /@me
                        - /user.id
            - /bulk-delete
        - /permissions*
            - /{overwrite.id}
        - /invites
        - /typing
        - /pins
            - /{message.id}
        - /recipients*
            - /{user.id}
        - /webhooks
- /guilds
    - /{guild.id}
        - /prune
        - /regions
        - /invites
        - /embed
        - /integrations
            - /{integration.id}
                - /sync
        - /roles
            - /{role.id}
        - /audit-log
        - /channels
        - /bans
            - /{user.id}
        - /members
            - /@me*
                - /nick
            - /{user.id}
                - /roles*
                    - /{role.id}
        - /webhooks
- /invites*
    - /{invite.code}
- /users*
    - /@me
        - /connections
        - /channels
        - /guilds
            - /{guild.id}
    - /{user.id}
- /voice*
    - /regions
- /webhook*
    - /{webhook.id}
        - /{webhook.token}
            - /slack
            - /github
            
\* not an API endpoint in the docs (should return 404)

$ rate limit exception (see rate limit section)
    

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