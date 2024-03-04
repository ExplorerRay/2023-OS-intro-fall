#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>

#define MAXCOM 100 // for max letters of command
#define MAX_ARG_CNT 10

void exec_cmd(char* cmd) {
	char* args[MAX_ARG_CNT];
	int cnt = 0;

	char* token = strtok(cmd, " ");
	while(token != NULL && cnt < MAX_ARG_CNT){
		args[cnt++] = token;
		token = strtok(NULL, " ");
	}
	
	args[cnt] = NULL;

	if(strcmp(args[0], "exit") == 0){
		printf("Process end\n");
		exit(0);
	}
	
	pid_t pid = fork();
	if(pid == 0){ // child
		execvp(args[0], args);	
	}
	else{
		wait(NULL);
	}
}


int main() {
	int running = 1;
	int cnt = 0;
	
	char* args;
	char inpCMD[MAXCOM];
	while (running) {
		printf("osh> ");
		fflush(stdout);
		
		fgets(inpCMD, sizeof(inpCMD), stdin);
		// convert \n to \0
		inpCMD[strcspn(inpCMD, "\n")] = '\0';
		//read(STDIN_FILENO, inpCMD, MAXCOM);
		exec_cmd(inpCMD);
	}


    return 0;
}
