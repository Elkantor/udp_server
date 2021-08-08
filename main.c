/*
** listener.c -- a datagram sockets "server" demo
*/

#include "dependencies.h"

const unsigned int  config_buffer_length_max   = 64000;
const char*         config_port                = "4950";

int main(void)
{
    const int socket_file_descriptor = udp_server_ipv6_socket_create(config_port);
    printf("listener: waiting to recvfrom...\n");

    while(1)
    {
        struct sockaddr_storage their_addr;
        socklen_t addr_len = sizeof(their_addr);
        char buffer[config_buffer_length_max];

        // receive data from socket
        const int numbytes = recvfrom(socket_file_descriptor, buffer, config_buffer_length_max -1 , 0, (struct sockaddr*)&their_addr, &addr_len);
        if (numbytes  == -1)
        {
            perror("recvfrom");
            exit(1);
        }

        char s[INET6_ADDRSTRLEN];
        void* client_ip = udp_server_sockaddr_get((struct sockaddr *)&their_addr);
        const char* client_addr = inet_ntop(their_addr.ss_family, client_ip, s, sizeof(s));
        
        printf("listener: got packet from %s\n", client_addr);
        printf("listener: packet is %d bytes long\n", numbytes);
        buffer[numbytes] = '\0';
        printf("listener: packet contains \"%s\"\n", buffer);
    }

    close(socket_file_descriptor);

    return 0;
}