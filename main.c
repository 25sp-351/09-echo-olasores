#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define DEFAULT_PORT 2345

void print_usage(const char *program_name) {
    printf("Usage: %s [-p port] [-v] [-m]\n", program_name);
    printf("  -p port    Set the server port (default: %d)\n", DEFAULT_PORT);
    printf("  -v         Enable verbose mode (print received messages)\n");
    printf("  -m         Enable multi-threaded mode (handle each connection in a separate thread)\n");
}

int main(int argc, char *argv[]) {
    int port = DEFAULT_PORT;
    int verbose = 0;
    int multi_threaded = 0;
    int opt;
    
    while ((opt = getopt(argc, argv, "p:vm")) != -1) {
        switch (opt) {
            case 'p':
                port = atoi(optarg);
                if (port <= 0 || port > 65535) {
                    fprintf(stderr, "Invalid port number: %s\n", optarg);
                    print_usage(argv[0]);
                    return 1;
                }
                break;
            case 'v':
                verbose = 1;
                break;
            case 'm':
                multi_threaded = 1;
                break;
            default:
                print_usage(argv[0]);
                return 1;
        }
    }
    
    printf("Starting echo server with configuration:\n");
    printf("  Port: %d\n", port);
    printf("  Verbose mode: %s\n", verbose ? "enabled" : "disabled");
    printf("  Multi-threaded mode: %s\n", multi_threaded ? "enabled" : "disabled");
    
    return run_server(port, verbose, multi_threaded);
}