#include <fcntl.h>             
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>


using namespace std;

int main()
{

	int x = 0, res, n = 1, i = 2;
	float sum = 0;
	char c;
	int check = 0;
	float k;
	
	int file_messages;
	if ((file_messages = open("messages.bin", O_RDWR)) == -1) return -2;
	
	float *msg_ptr = (float *)mmap(NULL, 1024*sizeof(float), PROT_READ|PROT_WRITE, MAP_SHARED, file_messages, 0);
	
	msg_ptr[0] = 3;
	
	while (1) {
    		res = read(STDIN_FILENO, &c, sizeof(char));
		if (c == ' ' || c == '\n' || c == '\t') {
			k = x;
			k /= n;
			sum += k;
			x = 0;
			n = 1;
			check = 0;
		} 
		else if (c == '.')
		{
			check = 1;	
		} 
		else 
		{
			x = x*10 + c - '0';
			if (check)
			{
				n *= 10;
			}
		}	

		if (res == 0 || c == '\n') {
			if (res == 0) {
				msg_ptr[0] = 2;
				break;
			}
			msg_ptr[i] = sum;
			msg_ptr[1] = i;
			msg_ptr[0] = 1;
			msync(msg_ptr, 1024*sizeof(float), MS_SYNC | MS_INVALIDATE);
			while (msg_ptr[0] == 1) {
				continue;
			} 
			i++;
			x = 0;
			sum = 0;
			n = 1;
			check = 0;
		}
	}

	return EOF;
}
