# Library notes

## The Send Queue
There are two send queues: the websocket queue and the HTTP queue. 

Other than the fact that they're both queues, they 
are unrelated and shouldn't directly affect each other on the client side.

### The HTTP Send Queue
The HTTP send queue (HSQ) is a *BSD TAILQ (linked list with a tail) of ulfius request structs.

New requests to be sent out are placed in the queue and sent when the HSQ is serviced by a library function. 
This is to allow for rate limits to be implemented easier. 

Each HSQ request has to contain the HTTP API endpoint to request (like GET /gateway/bot, etc), which is broken up into 
two parts:
- HTTP verb (GET, POST, DELETE, etc)
- path (/gateway, /guilds/{channel.id}/messages, etc.)

The function placing a request into the HSQ has to figure out what the verb and path are. 
 
Each HSQ request has to have a body, which must be in JSON format, except for GET requests, 
 which don't need a body, and for the purposes
of this library trying to put a body into a GET request will return an error when inserting it into the queue and
sending the request (check to see how ulfius handles this).

All functions that interface with the HSQ must be able to create request bodies out of JSON types from jansson or plain 
strings.

The request maker function is responsible for attaching the two headers required for all requests: 
the Authorization header 
containing the bot token and the User-Agent header, which specifies the client library and version number. 