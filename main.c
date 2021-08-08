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
        udp_server_struct_data data = udp_server_ipv6_data_receive(socket_file_descriptor, config_buffer_length_max);
        
        {
            char s[INET6_ADDRSTRLEN];
            void* client_ip                                         = udp_server_sockaddr_get(&data.client_socket_address);
            const struct sockaddr_storage* client_address_storage   = (struct sockaddr_storage*)client_ip;
            const char* client_address_text                         = inet_ntop(client_address_storage->ss_family, client_ip, s, sizeof(s));
            printf("listener: got packet from %s\n", client_address_text);
            printf("listener: packet is %d bytes long\n", data.bytes);
            printf("listener: packet contains \"%s\"\n", data.buffer);
        }

    }

    close(socket_file_descriptor);

    return 0;
}