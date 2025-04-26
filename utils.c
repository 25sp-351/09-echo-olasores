#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 1024

void error(const char *msg) {
    perror(msg);
    exit(1);
}

void handle_client(int client_fd, int verbose) {
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    
    while ((bytes_read = read(client_fd, buffer, BUFFER_SIZE - 1)) > 0) {
        buffer[bytes_read] = '\0';  
        
        if (verbose) {
            printf("Received: %s", buffer);
        }
        
        write(client_fd, buffer, bytes_read);
    }
    
    if (bytes_read < 0) {
        perror("Error reading from socket");
    }
    
    close(client_fd);
    printf("Client disconnected\n");
}