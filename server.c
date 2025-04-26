#include "server.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

void *client_thread(void *arg);

typedef struct {
    int client_fd;
    int verbose;
} thread_args_t;

int run_server(int port, int verbose, int multi_threaded) {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        error("Error opening socket");
    }
    
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        error("Error setting socket options");
    }
    
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);
    
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        error("Error binding socket");
    }
    
    if (listen(server_fd, 5) < 0) {
        error("Error listening");
    }
    
    printf("Server listening on port %d\n", port);
    
    while (1) {
        client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
        if (client_fd < 0) {
            perror("Error accepting connection");
            continue;
        }
        
        printf("New client connected\n");
        
        if (multi_threaded) {
            pthread_t thread_id;
            thread_args_t *args = malloc(sizeof(thread_args_t));
            if (args == NULL) {
                perror("Failed to allocate memory for thread arguments");
                close(client_fd);
                continue;
            }
            
            args->client_fd = client_fd;
            args->verbose = verbose;
            
            if (pthread_create(&thread_id, NULL, client_thread, args) != 0) {
                perror("Failed to create thread");
                free(args);
                close(client_fd);
                continue;
            }
            
            pthread_detach(thread_id);
        } else {
            handle_client(client_fd, verbose);
        }
    }
    
    close(server_fd);
    return 0;
}

void *client_thread(void *arg) {
    thread_args_t *args = (thread_args_t *)arg;
    int client_fd = args->client_fd;
    int verbose = args->verbose;
    
    free(arg);
    
    handle_client(client_fd, verbose);
    
    return NULL;
}