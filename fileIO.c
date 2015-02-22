#ifndef FILEIO_H
#define FILEIO_H

#include <stdio.h>
#include <sys/stat.h>


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

/* if a file is readable
 * should ensure the file exist
 * return 1, readable
 * return 0, not readable
 */
int fileReadable(char fname[]) {
    if (NULL == fopen(fname, "r")) {
		return 0;
	}
	return 1;
}

int loadFile(void *dst, char fname[], int fsize) {
    FILE *file;
    if (NULL != (file = fopen(fname, "r"))) {
        int nread = fread(dst, 1, fsize, file);
        fclose(file);
        return nread;
    }
    return -1;
}

#endif /*FILEIO_H*/
