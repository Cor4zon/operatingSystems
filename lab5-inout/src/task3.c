#include <stdio.h>

#define FILENAME "newFile.txt"

int main(){
    FILE* fd1 = fopen(FILENAME, "w");
    FILE* fd2 = fopen(FILENAME, "w");

    // check is file open?
    
    for (char c = 'a'; c <= 'z'; c++){
        if (c % 2){
	    fprintf(fd1, "%c", c);
	}
	else{
	    fprintf(fd2, "%c", c);
	}
    }

    fclose(fd1);
    fclose(fd2);

    return 0;
}
