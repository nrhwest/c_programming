/*
 * Nathan West
 * 12/05/17
 * CMSC 257 - Assignment 5
 * server.c -- a stream socket server
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>

#define BACKLOG 10	 // how many pending connections queue will hold
#define MAXDATASIZE 50

pid_t pid;
int sockfd, new_fd, yes = 1;  // listen on sock_fd, new connection on new_fd

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET)
		return &(((struct sockaddr_in*)sa)->sin_addr);
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

// signal handler to catch sig int
void signal_handler(int num)
{
	printf("\nSignal caught, exiting...\n");
  close(sockfd);
  close(new_fd);
	exit(-1);
}

// signal handler to catch child process
void sig_child(int chld)
{
	while (1) {
    pid = waitpid(-1, NULL, WNOHANG);
    if (pid == 0)
      return;
  }
}

// sends file back to client
void transfer_file(int sock, char *file_name)
{
  int file;
  ssize_t read_bytes, sent_bytes;
  char buffer[MAXDATASIZE];
  char *error_message = "File not found";

  if ((file = open(file_name, O_RDONLY)) == 0) {  // if file can't be opened
    perror(file_name);
    if ((sent_bytes = send(sock, error_message, strlen(error_message), 0)) < 0) {
      perror("send error");
      exit(-1);
    }
  } else {  // else file was successfully opened
    printf("Sending file : %s\n", file_name);
    while ((read_bytes = read(file, buffer, MAXDATASIZE)) > 0) {
      if ((sent_bytes = send(sock, buffer, read_bytes, 0)) < read_bytes) {
        perror("send error");
        exit(-1);
      }
    }
    close(file);
  }
}

int main(int argc, char *argv[])
{
  int rv;
	struct addrinfo hints, *servinfo, *p;
	struct sockaddr_storage their_addr; // clients address information
	socklen_t sin_size;
	char file_name[MAXDATASIZE], buffer[MAXDATASIZE], s[INET6_ADDRSTRLEN];
  ssize_t received_bytes;

	// signal handling
	signal(SIGINT, signal_handler);
	signal(SIGCHLD, sig_child);

	if (argc > 2)
		fprintf(stderr,"server: incorrect number of arguments\n");

	memset(&hints, 0, sizeof(hints));
  memset(&their_addr, 0, sizeof(their_addr));

	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // use my IP

	if ((rv = getaddrinfo(NULL, argv[1], &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and bind to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			perror("server: socket");
			continue;
		}

		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
			perror("setsockopt");
			exit(1);
		}

		// makes the connection from client to server
		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("server: bind");
			continue;
		}
		break;
	}

	freeaddrinfo(servinfo); // all done with this structure

	if (p == NULL)  {
		fprintf(stdout, "server: failed to bind\n");
		exit(1);
	}

	// waits for client to connect
	if (listen(sockfd, BACKLOG) == -1) {
		perror("error: client can't connect");
		exit(1);
	}

	printf("server: waiting for connections...\n");

	while(1) {  // main accept() loop
		sin_size = sizeof(their_addr);
		new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
		if (new_fd == -1) {
			perror("accept");
			continue;
		}

		pid = fork();  // creating a child process
    if (pid == 0) {
			inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr),
				s, sizeof s);
			printf("server: got connection from %s\n", s);
      if ((received_bytes = recv(new_fd, buffer, MAXDATASIZE, 0)) < 0) {
        perror("recv error");
        return -1;
      }
      sscanf(buffer, "%s\n", file_name);
      transfer_file(new_fd, file_name);
		}
    close(new_fd);
	}
  close(sockfd);
	return 0;
}
