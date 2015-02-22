#include <stdio.h>
#include <memory.h>
#include <string.h>

#include "format.h"

/* 
 * return 1 if file escape home
 * return 0 if not
 */
int fileEscapeHome(char path[], int size) {
	int i, cnt = 0;
	char *token;
	token = strtok(path, "/");
	while (token != NULL) {
		cnt++;
		if (strcmp(token, "..") == 0) {
			cnt -= 2;
		}
		if (cnt < 0) {
			return 1;
		}
		token = strtok(NULL, "/");
	}
	return 0;
}

void getFileNameType(char path[], int size, struct HttpReq *req) {
	//printf("size %d, path %s\n", size, path);
    if (size == 1 && path[0] == '/') {
        snprintf(req->reqLoc, (sizeof home) + (sizeof index), "%s%s", home, index);
    } else {
        snprintf(req->reqLoc, (sizeof home) + size, "%s%s", home, path);
    }
	req->contType = TEXT_HTML;
	if (size >= 4) {
	  	if (strncmp(".jpg", path+size-4, 4) == 0) 
			req->contType = IMG_JPEG;
	}
}

/*
 * parse succ return 0, otherwise, return -1
 */
int parseHttpReq(char *buf, int size, struct HttpReq *req) {
    //native implementation
    int start = 0, end = 0;
    for (int i = 0; i < size; i++) {
        if (i == 0) {
            if (buf[0] == 'G') {
                req->reqType = GET;
            } else {
                return -1;
            }
        }
        if (buf[i] == ' ') {
            if (start == 0) {
                start = i + 1;
            } else {
                end = i;
                break;
            }
        }    
    }
    if (start == 0 || end == 0) {
        return -1;
    }
    memset(req->reqLoc, 0, sizeof(req->reqLoc));

	getFileNameType(buf+start, end-start, req);

	if (fileEscapeHome(buf+start, end-start)) {
		return -1;
	}

    return 0;
}


