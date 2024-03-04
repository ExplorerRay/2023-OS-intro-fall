#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

void show(int fork_id) {
    printf("Fork %d. I'm the child %d, my parent is %d.\n", fork_id, getpid(), getppid());
}

int main() {
    printf("Main Process ID: %d\n\n", getpid());
    
    pid_t pid;
    pid = fork(); // Fork 1
    if (pid == 0){ //代表Fork接下來要做的
        show(1);
		pid = fork(); //fork 2
        if (pid == 0){
            show(2);
        }
        else{
            wait(NULL);
            pid = fork(); //fork 3
            if (pid == 0){
                show(3);
            }
            else{
                wait(NULL);
            }
            pid = fork(); //fork 4
            if(pid == 0){
                show(4);
            }
            else{
                wait(NULL);
            }
        }
    } 
	else{ //代表自己要做的
        wait(NULL);
		pid = fork(); //fork 2
        if (pid == 0){
            show(2);
        }
        else{
            wait(NULL);
            pid = fork(); //fork 3
            if (pid == 0){
                show(3);
            }
            else{
                wait(NULL);
            }
            pid = fork(); //fork 4
            if(pid == 0){
                show(4);
            }
            else{
                wait(NULL);
            }
        }
	}
	
    return 0;
}
