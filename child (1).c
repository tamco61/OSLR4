#include <stdio.h>
#include <fcntl.h>             
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
 #include <errno.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <unistd.h>



int main() {
	int x = 0, sum = 0, i = 2, res;
	char c;
	
	int file_messages;
	if ((file_messages = open("messages.bin", O_RDWR)) == -1) {
		perror("open file messages error");
		return 128128;
	}
	
	int *msg_ptr = mmap(NULL, 1024*sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED, file_messages, 0);
	
	msg_ptr[0] = 3;
	
	while (1) {
    		res = read(STDIN_FILENO, &c, sizeof(char));
		if (c == ' ' || c == '\n' || c == '\t') {
			sum += x;
			x = 0;
		} else {
			x = x*10 + c - '0';
		}	

		if (res == 0 || c == '\n') {
			if (res == 0) {
				msg_ptr[0] = 2;
				break;
			}
			msg_ptr[i] = sum;
			msg_ptr[1] = i;
			msg_ptr[0] = 1;
			msync(msg_ptr, 1024*sizeof(int), MS_SYNC | MS_INVALIDATE);
			while (msg_ptr[0] == 1) {
				continue;
			} 
			i++;
			x = 0;
			sum = 0;
		}
	}
	return 0;
}