#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

struct UdpHead
{
    short src_port;
    short dest_port;
    short len;
    short checksum;
};

int main()
{
    char buf[] = "HI!\n";
    char buf2[128];
    char message[128];
    struct sockaddr_in addr, server_addr;
    int sock, bytes_read, cycle = 1, size = sizeof(server_addr);
    struct UdpHead header;
    short port;
    
    sock = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if(sock < 0)
    {
        perror("sock");
        exit(-1);
    }
    
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    
    header.src_port = htons(12346);
    header.dest_port = htons(3333);
    header.len = htons(sizeof(header) + sizeof(buf));
    header.checksum = 0;
    
    memcpy(message, (char *)&header, sizeof(header));
    memcpy(message + sizeof(header), buf, sizeof(buf));
    sendto(sock, message, sizeof(header) + sizeof(buf), 0, (struct sockaddr *)&addr, sizeof(addr));
    while(cycle)
    {
        if(bytes_read = recv(sock, buf2, sizeof(buf2), 0) < 0)
        {
            perror("recv");
            exit(-1);
        }
        port = ntohs(*((int *)(buf2 + 22)));
        if(port == 12346)
        {
            cycle = 0;
            printf("CLIENT: form %d , message: %s\n", port, buf2 + 28);
        }
    }
    close(sock);
    exit(0);
}
