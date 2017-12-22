/**
  Nathan West
  11/19/17
  CMSC 257 - assignment4
  Description: The purpose of this class is to create a shell
  which will receive and execute a command given by the user
*/

#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>

#define CMD_DELIMS " \t\n"  // command delimiter tool

pid_t pid;  // child process
char* args[51];  // arguments
int n = 0;  // number of calls to 'command'

// function to handle signals
void signal_handler(int num)
{
	fprintf(stdout, "\nSignal was caught\n");
	return;
}

// function to execute the command
int execute()
{
	pid = fork();  // child process
	if (pid < 0) {
		fprintf(stdout, "Fork error\n");
		exit(1);
	} else if (pid > 0) {
		fprintf(stdout, "Parent - New child pid: %d\n", pid);
	} else {
		if (execvp( args[0], args) == -1)
			_exit(EXIT_FAILURE);  // if child fails
	}
	return 0;
}

// function to parse the input (cmd)
void parse_command(char* cmd)
{
	int i = 0;
	char* token = strtok( cmd, CMD_DELIMS );  // splits up cmd using defined delimiter

	while( token != NULL ) {
		args[i++] = token;  // setting values of global args
		token = strtok( NULL, CMD_DELIMS );  // refresh token
  }
	args[i] = NULL;
}

// function to run the command
int run(char* cmd)
{
	int success = 0;
	parse_command( cmd );  // parse the command

	if ( args[0] != NULL ) {
		if ( strcmp(args[0], "exit") == 0 )	{  // exit
			raise(SIGKILL);  // raise the SIGKILL
		}
		if ( strcmp(args[0], "cd") == 0 ) {  // change directory
			if ( args[1] == NULL ) {
				char *buf;
				buf = (char *) malloc( 100*sizeof(char) );
				getcwd( buf, 100 );
				printf("%s \n", buf);
				return 0;
			}
			success = chdir( args[1] );  // if success == 0, cd was successful
			if ( success != 0 ) {
				fprintf(stdout, "error: unable to change directory\n");
				return 1;
			}
			return success;
		}
		if ( strcmp(args[0], "pid") == 0 ) {  // returns pid
			fprintf(stdout, "Present pid: (%d)\n", getpid());
			return 0;
		}
		if ( strcmp(args[0], "ppid") == 0 ) {  // returns ppid
			fprintf(stdout, "Parent pid: (%d)\n", getppid());
			return 0;
		}
		n += 1;
		return execute();
	}
	return 0;
}

// function to check exit status of child processes
void check_status(int n)
{
	int status, i;
	for (i = 0; i < n; i++) {
		wait( &status );
		printf("Child exit status : %d\n", WEXITSTATUS(status));
	}
}

// main function running infinite loop
int main(int argc, char **argv)
{
	printf("……………………………………………………………………………………………\n");
	printf("Entering shell... Type exit to exit\n");

	char* prompt = "my257sh";  // default prompt
	char line[1024];
	signal(SIGINT, signal_handler);  // to catch any signals

	if ( argc >= 2 && (strcmp(argv[1], "-p") == 0) )
		prompt = argv[2];  // setting customized prompt
	fflush(NULL);  // clears the output buffer

	while (1) {
    printf( "%s$ ", prompt );  // prints command line
		if ( !fgets(line, 1024, stdin) )  // reads the command line
			return 0;

		int input = 0, i;
		char* cmd = line;
		input = run( cmd );  // run() returns int

		if ( input != 0 )  // if input == 0, command executed sucessfully
			printf("%s: could not execute command\n", prompt);

		check_status( n );
		n = 0;
	}
	return 0;
}
