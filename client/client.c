#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#define PORT 8080
#define MAX_STR_SIZE 1024

static void *send_msg(void *arg) {
    int sock_fd = *(int *)arg;

    while(1){
        char *send_msg = (char *)malloc(MAX_STR_SIZE);
        printf("You: ");
        fgets(send_msg, 1024, stdin);
        send(sock_fd, send_msg, strlen(send_msg), 0);
        free(send_msg);
    }

    close(sock_fd);
    return NULL;
}

static void *recv_msg(void *arg) {
    int sock_fd = *(int *)arg;

    while(1) {
        char buffer[1024] = { 0 };
        read(sock_fd, buffer, 1024 - 1);
        printf("client : %s\n", buffer);
    }
    return NULL;
}

int client_init(){

    int status, client_fd;
    struct sockaddr_in serv_addr;
    pthread_t send_msg_thread;
    pthread_t recv_msg_thread;
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary
    // form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if ((status = connect(client_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    if(0 != pthread_create(&send_msg_thread, NULL, send_msg, &client_fd))
        perror("pthread_create for send msg");

    if(0 != pthread_create(&recv_msg_thread, NULL, recv_msg, &client_fd))
        perror("pthread_create for send msg");

    pthread_join(send_msg_thread, NULL);
    pthread_join(recv_msg_thread, NULL);

    close(client_fd);
    return 0;
}

int main(int argc, char const* argv[])
{
    client_init();
    return 0;
}
