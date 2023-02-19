#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <poll.h>
#include "d_string.h"


int main() {
	int fd1[2];
	char* filename = NULL;
	int filename_len = 0;
	
	if (scan_string(&filename, &filename_len, STDIN_FILENO)) {
		return -1;
	}
	
	int file_d = open(filename, O_RDONLY);
	if (file_d == -1) {
		perror("open error");
		return 100;
	} else if (pipe(fd1)) {
		perror("pipe1 error");
		return 101;
	}
	
	pid_t pid = fork();
	if (pid < 0) {
		perror("fork error");
		return -10;
	}	
	else if (pid == 0) {
		if (dup2(file_d, STDIN_FILENO) == -1) {
			perror("dup2 error");
			return 102;
		} else if (dup2(fd1[1], STDOUT_FILENO) == -1) {
			perror("dup2 error");
			return 103;
		} else if (close(fd1[0])) {
			perror("close pipe error");
			return 104;
		} else {
			execl("./child", "./child", (char *) NULL);
		}
	} else {
		if (close(file_d)) {
			perror("close file error");
			return 105;
		}	
		int x;
		while (read(fd1[0], &x, sizeof(int)) != sizeof(char)) {
			printf("%d\n", x);
		}
	}
	
	if (close(fd1[0])) {
        	perror("close pipe error");
        	return 106;
        } else if (close(fd1[1])) {
        	perror("close pipe error");
        	return 107;
        }
	
	return 0;
}