#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <iostream>

int main(){
    int server_sockfd, client_sockfd;
    socklen_t server_len, client_len;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;

    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_address.sin_port = htons(9123);
    server_len = sizeof(server_address);

    int reuse = 1;
    int ret = setsockopt(server_sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int));
    if (ret == -1) {
        std::cerr << strerror(errno) << std::endl;
        abort();
    }

    bind(server_sockfd, (struct sockaddr *)&server_address, server_len);

    listen(server_sockfd, 5);
    while(1){
        char ch[512];
        client_len = sizeof(client_address);
        client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, &client_len);
<<<<<<< HEAD
        int n = read(client_sockfd, &ch, sizeof(512));
=======
        printf("accept client\n");
        int n = read(client_sockfd, &ch, 512);
>>>>>>> ec5df2b52f46149d6b889fa0affca17407bc525d
        write(client_sockfd, &ch, n);
        close(client_sockfd);
    }
}
