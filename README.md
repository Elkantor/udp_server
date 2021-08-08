# udp server
> Simple server in C using UDP socket (currently only for linux)

## Installing / Getting started

1. [Linux (gcc or clang)](#on-linux)
2. [Windows (cl, gcc or clang) *soon*](#on-windows)

### On Linux
Replace **cc** by **gcc** or **clang**:

```bash
git clone https://github.com/Elkantor/udp_server.git
cd udp_server
```

or with clib:
```bash
clib install Elkantor/udp_server
#optionally cd deps/udp_server
```

### To test the simple example:
(wich listen on port 4950, and print the message sent from a client, and its address)
```bash
cc server.c -o server
./server
```