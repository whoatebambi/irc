# IRC Setup Guide

## Running the IRC Server

In the current terminal, compile and start the server:
```
make
./ircserv
```

## Connecting with irssi

In another terminal, open **irssi** and connect to the server:
```
irssi -c 127.0.0.1 -p 6667 -n testuser -w 1234
```

## Disconnecting

To disconnect from the server, use:
```
/quit
```