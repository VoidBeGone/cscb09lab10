#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
	int pipe1[2]; //foo to bar FOO WRITE TO THIS
	int pipe2[2]; // bar to foo BAR WRITE TO THIS foo 
	char input[20];
	char output[20];
	char buffer[20];
	enum {R,W};
	if ((pipe(pipe1) == -1) || (pipe(pipe2) == -1)){ return -1;}
	pid_t pid1, pid2;

	pid1 = fork();
	if (pid1 == -1){
		perror("fork bad");
		return 1;
	}
	if (pid1 ==0 ){
		close(pipe1[R]);
        	close(pipe2[W]);
		snprintf(input, 20, "%d", pipe2[R]);
        	snprintf(output, 20, "%d", pipe1[W]);
        	if (execlp("./foo", "foo", input, output, NULL) == -1){
                	perror("foo side failed");
        	        return -1;
	        }
	}

	pid2 = fork();
	if (pid2 == -1){
		perror("fork bad");
		return -1;
	}
	if (pid2 == 0 ){
		close(pipe1[W]);
        	close(pipe2[R]);
        	snprintf(input, 20, "%d", pipe1[R]);
        	snprintf(output, 20, "%d", pipe2[W]);
        	if (execlp("./bar","bar", input, output, NULL) == -1){
                	perror("bar side failed");
                	return -1;
        	}
	}
	close(pipe1[R]);
	close(pipe2[W]);
	close(pipe1[W]);
        close(pipe2[R]);
	wait(NULL);
	wait(NULL);
	return 0;
}
