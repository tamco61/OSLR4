#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include "d_string.h"
#include <signal.h>

int *msg_ptr;

int main() {
	char* filename = NULL;
	int filename_len = 0;
	int file_input, file_messages;
	if (scan_string(&filename, &filename_len)) {
		return -1;
	}
	
	if ((file_input = open(filename, O_RDONLY)) == -1) {
		perror("open file answers error");
		return 9812;
	}
	if ((file_messages = open("messages.bin", O_RDWR|O_TRUNC|O_CREAT, S_IRWXU)) == -1) {
		perror("open file messages error");
		return 128128;
	}
	
	if (ftruncate(file_messages, 1024*sizeof(int)) == -1) {
		perror("change length message file error");
		return 12823;
	} 
	
	pid_t pid = fork();
	if (pid < 0) {
		perror("fork error");
		return 1899;
	} else if (pid == 0) {
		if (dup2(file_input, STDIN_FILENO) == -1) {
			perror("dup2 stdin error");
			return 191929;
		}
		close(file_messages);
		execl("./child", "./child", (char *) NULL);
		perror("execl error");
		return 102910;
	}
	
	msg_ptr = mmap(NULL, 1024*sizeof(int), PROT_WRITE | PROT_READ, MAP_SHARED, file_messages, 0);
	msg_ptr[0] = 0;
	int i = 0;
	while (1) {
		if (msg_ptr[0] == 1) {
			printf("%d ", msg_ptr[msg_ptr[1]]);
			msg_ptr[0] = 0;
			i++;
		} 
		if (msg_ptr[0] == 2) {
			break;
		}
	}
}