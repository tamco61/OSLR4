#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

int main() {
	int x = 0, sum = 0, res;
	char c;
	
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
				if ((write(STDOUT_FILENO, "C", sizeof(char)) == -1))
					perror("write error");
				break;
			}
			if ((write(STDOUT_FILENO, &sum, sizeof(int)) == -1))
				perror("write error");
			x = 0;
			sum = 0;
		}
	}
	
	return EOF;
}