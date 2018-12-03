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
int sock;

// when the user presses ctrl+c
void handler(int sig) {
  close(sock);
  exit(0);
}

int main(int argc, char* argv[]) {
  sock = socket(AF_INET, SOCK_STREAM, 0);
  signal(SIGINT, handler);

  // validate that the arguments look like the following
    // Program name
    // server address
    // server port
    // nickname
  if (argc != 4) {
    printf("Please provide 3 arguments for the program!\n");
    printf("\tServer Address\n");
    printf("\tServer Port\n");
    printf("\tNickname to use\n");
    exit(1);
  }

  // use our socket struct
  struct sockaddr_in server;
  server.sin_family = AF_INET;
  // get the server address and port
  // convert the port to an int
  int arg = strtol(argv[2], NULL, 10); 

  printf("CONNECTING TO: %s\n", argv[1]);
  printf("ON PORT: %d\n", arg);
  
  server.sin_port = htons(arg);

  struct hostent * host;
  host = gethostbyname(argv[1]);

  // Try to connect to the server
  if (!host) {
    printf("Unable to connect to host!\n");
    exit(1);
  }
  if (sock == -1) {
    printf("Encountered an error creating a socket!\n");
    exit(1);
  }

  // Copy host information to our address.
  memcpy(&server.sin_addr, host->h_addr, host->h_length);

  // Bind our socket to the address
  if (connect(sock, (struct sockaddr*) &server,
        sizeof(struct sockaddr_in)) == -1) {
    printf("Error connecting the socket.\n");
    close(sock);
    exit(1);
  }

  // accept a connection
  char * input = calloc(100, sizeof(char));
  char data[256] = {};
  size_t readin = 100;

  // First message to the server is the client name.
  write(sock, argv[3], strlen(argv[3]) + 1);
  // Loop until the user presses ctrl-c
  while (1) {
    // read from keyboard send data to the server
    getline(&input, &readin, stdin);
    write(sock, input, strlen(input) + 1);
    read(sock, data, 256);
    printf("CLIENT RECVD: %s\n", data);
  }
  close(sock);
  return 0;
}

