#include <stdio.h>

char home[] = "/Users/fengzhu/coding/cse124/web/cseweb.ucsd.edu/f7zhu";

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
    FILE *file;
    if (0 != (file = fopen(fname, "rb"))) {
        printf("file found\n");
        fseek(file, 0, SEEK_END);
        int size = ftell(file);
        fseek(file, 0, SEEK_SET);
        fclose(file);
        return size;
    }
    printf("file not found\n");
    return -1;
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
