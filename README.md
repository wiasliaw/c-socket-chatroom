# c-socket-chatroom

## How to run

### Compile

```bash
$ make
```

### server

* argument
    * IP
    * Port

```bash
$ ./bin/server <IP> <Port>

# e.g.
$ ./bin/server 127.0.0.1 8888
```

### client

* argument
    * IP
    * Port

```bash
$ ./bin/client <IP> <Port> <Name> <room ID>

# e.g.
$ ./bin/client 127.0.0.1 8888 will 2
```

## How to use

1. run `bin/server`
2. run `bin/client`
3. type in terminal to send and receive message from other people in the room
