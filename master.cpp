#include <iostream>
#include <fcntl.h>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>

using namespace std;


float *msg_ptr;


int main()
{
	// file etc
	string filename;
	cin >> filename;

	int file_d = open(filename.c_str(), O_RDONLY);
	if (file_d == -1) return -11;

	int file_msg_d = open("messages.bin", O_RDWR|O_TRUNC|O_CREAT, S_IRWXU);
	if (file_msg_d == -1) return -12;

	//file map
	if (ftruncate(file_msg_d, 1024*sizeof(float)) == -1) return -2;
	
	struct stat statbuf;
	fstat(file_msg_d, &statbuf);

	// fork etc
	pid_t id = fork();
	if (id == -1) return -3;

	// main algo
	if (id == 0)
	{
	
		if (dup2(file_d, STDIN_FILENO) == -1) return -4;
		close(file_msg_d);
		
		execl("./slave","./slave", (char *) NULL);

	}
	else {
		msg_ptr = (float *)mmap(NULL, statbuf.st_size, PROT_WRITE | PROT_READ, MAP_SHARED, file_msg_d, 0);
		msg_ptr[0] = 0;
		int i = 0;
		while (1) {
			if (msg_ptr[0] == 1) {
				cout << msg_ptr[(int)msg_ptr[1]] << endl;
				msg_ptr[0] = 0;
				i++;
			}	 
			if (msg_ptr[0] == 2) {
				break;
			}
		}
	}

	return 0;
}
