/*
** udp_server -- contains helper methods to do network
*/

#if defined(udp_server_SELF_CONTAINED)
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <netdb.h>
    #include <stddef.h>
    #include <stdio.h>
    #include <unistd.h>
    #include "udp_server_structs"
#endif

/********** Structures **********/
typedef struct udp_server_struct_data
{
    int bytes;
    char buffer[64000];
    struct sockaddr client_socket_address;
} udp_server_struct_data;


/********** Procedures **********/

// get sockaddr IPv4 or IPv6 address:
void* udp_server_sockaddr_get(const struct sockaddr* sa)
{
    // ipv4 address
    if (sa->sa_family == AF_INET) 
    {
        struct sockaddr_in* socket_address_ipv4 = (struct sockaddr_in*)sa;
        struct in_addr*     socker_address_ip   = &(socket_address_ipv4->sin_addr);
        return socker_address_ip;
    }

    // ipv6 address
    struct sockaddr_in6*    socket_address_ipv6 = (struct sockaddr_in6*)sa;
    struct in6_addr*        socket_address_ip   = &(socket_address_ipv6->sin6_addr);
    return socket_address_ip;
}

// create a udp socket with ipv6 on port passed in parameter, return the socket file descriptor
int udp_server_ipv6_socket_create(const char* port)
{
    struct addrinfo hints   = { 0 };
    hints.ai_family         = AF_INET6;     // set to AF_INET to use IPv4
    hints.ai_socktype       = SOCK_DGRAM;   // UDP datagrams
    hints.ai_flags          = AI_PASSIVE;   // use my IP

    struct addrinfo* servinfo;
    {
        const int rv = getaddrinfo(NULL, port, &hints, &servinfo);
        if (rv != 0)
        {
            fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
            return 1;
        }
    }

    // loop through all the results and bind to the first we can
    int sockfd = 0;
    struct addrinfo* p;
    for (p = servinfo; p != NULL; p = p->ai_next)
    {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
        {
            perror("listener: socket");
            continue;
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1)
        {
            close(sockfd);
            perror("listener: bind");
            continue;
        }

        break;
    }

    if (p == NULL)
    {
        fprintf(stderr, "listener: failed to bind socket\n");
        return 2;
    }

    freeaddrinfo(servinfo);

    return sockfd;
}

// receive data to a socket_file_descriptor
udp_server_struct_data udp_server_ipv6_data_receive(const int socket_file_descriptor, const unsigned int buffer_length_max)
{
    struct sockaddr_storage client_address_storage;
    socklen_t               client_address_lenght   = sizeof(client_address_storage);
    struct sockaddr*        client_address          = (struct sockaddr *)&client_address_storage;

    udp_server_struct_data data;

    // receive data from socket
    data.bytes = recvfrom(socket_file_descriptor, data.buffer, buffer_length_max -1 , 0, client_address, &client_address_lenght);
    if (data.bytes  == -1)
    {
        perror("recvfrom");
        exit(1);
    }

    //char s[INET6_ADDRSTRLEN];
    //void* client_ip = udp_server_sockaddr_get((struct sockaddr *)&their_addr);
    //const char* client_addr = inet_ntop(their_addr.ss_family, client_ip, s, sizeof(s));
    data.buffer[data.bytes] = '\0';

    return data;
}

// send data (message) to a socket_file_descriptor
void udp_server_ipv6_data_send(int socket_file_descriptor, const size_t message_length, const char message[message_length], const struct sockaddr client_address)
{

    ssize_t result = sendto(socket_file_descriptor, message, message_length, MSG_CONFIRM, &client_address, sizeof(client_address));
    if (result == -1)
    {
        perror("server response");
        exit(1);
    }
}