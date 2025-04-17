#ifndef UTIL_H
#define UTIL_H

void error(const char *msg);

/**
 * Handle a client connection
 * @param client_fd The client socket file descriptor
 * @param verbose Whether to print received messages to stdout
 */
void handle_client(int client_fd, int verbose);

#endif /* UTIL_H */