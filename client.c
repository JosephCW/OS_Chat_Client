// Add all our headers that will be used.
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
// So we can show disconnect
#include <signal.h>

// Socket for our server/client connection
int socket;

// when the user presses ctrl+c
void handler(int sig) {
  close(socket);
  exit(0);
}

void main(int argc, char* argv[]) {
  socket = socket(AF_INET, SOCK_STREAM, 0);
  signal(SIGINT, handler);
}

