#include <stdio.h>
#include <sys/stat.h>

char home[] = "/home/zagfox/cse124/web";

void genFileName(char path[], char file[], int file_size) {
    if (strlen(path) == 1 && path[0] == '/') {
        snprintf(file, file_size, "%s%s", home, "/index.html");
    } else {
        snprintf(file, file_size, "%s%s", home, path);
    }
}

/*
 * if file exist, return file size
 * otherwise, return -1
 */
int fileExist(char fname[]) {
    FILE *fp;
	struct stat st;
	if (-1 == stat(fname, &st)) {
		perror("file stat error\n");
		return -1;
	}
	int size = st.st_size;
	return size;
}

int loadFile(void *dst, char fname[], int fsize) {
    FILE *file;
    if (0 != (file = fopen(fname, "r"))) {
        int nread = fread(dst, 1, fsize, file);
        fclose(file);
        return nread;
    }
    return -1;
}
