#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
}

int socket_init() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    pthread_t send_msg_thread;
    pthread_t recv_msg_thread;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    
    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    printf("Server socket created: %d\n", server_fd);
    
    // Set socket options
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    
    // Define the address and port for the server
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    // Bind the socket to the address and port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    
    // Start listening for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    
    printf("Chat server started on port %d\n", PORT);
    
    // Accept a new connection
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    if(0 != pthread_create(&send_msg_thread, NULL, send_msg, &new_socket))
        perror("pthread_create for send msg");

    if(0 != pthread_create(&recv_msg_thread, NULL, recv_msg, &new_socket))
        perror("pthread_create for send msg");

    pthread_join(send_msg_thread, NULL);
    pthread_join(recv_msg_thread, NULL);

    close(server_fd);
    close(new_socket);
    return 0;
}

int main() {

    socket_init();
    return 0;
}