/*
 * Nathan West
 * 12/05/17
 * CMSC 257 – Assignment 5
 * client.c -- a stream socket client
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define MAXDATASIZE 50 // max number of bytes we can get at once

// get sockaddr, IPv4 or IPv6:
int *get_in_addr(struct sockaddr * sa)
{
  if (sa->sa_family == AF_INET) {
    return &(((struct sockaddr_in*) sa)->sin_addr);
  }
  return &(((struct sockaddr_in6*) sa)->sin6_addr);
}

// function to receive the file from the server
void get_file(int sock, char *file_name)
{
  int file;
	char buffer[MAXDATASIZE];
	ssize_t sent_bytes, received_bytes;

 	if((sent_bytes = send(sock, file_name, MAXDATASIZE - 1, 0)) < 0) {
 		perror("send error");
 		exit(-1);
 	}

 	if ((file = open(file_name, O_WRONLY|O_CREAT, 0644)) < 0) {
 		perror("error creating file");
 		exit(-1);
 	}

  while ((received_bytes = recv(sock, buffer, MAXDATASIZE, 0)) > 0) {
    if (write(file, buffer, received_bytes) < 0 ) {
      perror("error writing to file");
      exit(-1);
    }
  }

  printf("Received file : %s\n", file_name);
  close(file);
}

int main(int argc, char *argv[])
{
  int i, num_files = atoi(argv[3]);    // number of files client requests

  /*  ---- CODE EXPLANATION ----
  This for loop controls the whole program. I could not figure out how to
  use pthreads to make the client multi-threaded, so I used a for loop to
  loop through the num_files passed by the user and create a new connection
  each time.
  */

  for (i = 0; i <= num_files; i++) {
    int sock_fd, rv;
    struct addrinfo hints, *servinfo, *p;
    char s[INET6_ADDRSTRLEN], buffer[MAXDATASIZE];
    char file_name[MAXDATASIZE];

    if (argc > 5) {
      fprintf(stderr, "client: incorrect number of arguments\n");
      exit(1);
    }

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(argv[1], argv[2], & hints, & servinfo)) != 0) {
      fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
      return 1;
    }

    // loop through all the results and connect to the fst we can
    for (p = servinfo; p != NULL; p = p->ai_next) {
      if ((sock_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
        perror("client: socket");
        continue;
      }

      // connect to server
      if (connect(sock_fd, p->ai_addr, p->ai_addrlen) == -1) {
        perror("client: connect");
        close(sock_fd);
        continue;
      }
      break;
    }

    if (p == NULL) {
      fprintf(stderr, "client: failed to connect\n");
      return 2;
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr*) p->ai_addr), s, sizeof s);
    printf("client: connecting to %s\n", s);

    freeaddrinfo(servinfo); // all done with this structure
    int sleep_time = atoi(argv[4]);   // sleep time for downloading files
    int rand_num = (rand() % 25);  // pick random file_name from array
    snprintf(file_name, MAXDATASIZE - 1, "file%03d.txt", rand_num);
    get_file(sock_fd, file_name);
    sleep(sleep_time);
    close(sock_fd);
  }
  return 0;
}
