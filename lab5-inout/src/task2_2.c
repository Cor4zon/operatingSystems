#include <fcntl.h>

#define FILENAME "alphabet.txt"

int main(){
    char c;
    int fd1 = open(FILENAME, O_RDONLY);
    int fd2 = open(FILENAME, O_RDONLY);
    int flag1, flag2;

    while(flag1 || flag2){
	flag1 = read(fd1, &c, 1);
        if (flag1) write(1, &c, 1);

	flag2 = read(fd2, &c, 1);
	if (flag2) write(1, &c, 1);
    }
    return 0;
}

