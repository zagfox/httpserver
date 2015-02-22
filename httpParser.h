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

int getFileName(char path[], int size, struct HttpReq *req) {
	//printf("size %d, path %s\n", size, path);
	if (size < 1 || path[0] != '/') {
		return -1;
	}

	snprintf(req->reqLoc, (sizeof home) + size, "%s%s", home, path);
	int pos = (sizeof home) + size;
    if (path[size-1] == '/') {
        snprintf(req->reqLoc+pos-1, (sizeof index), "%s", index);
    } 


	return 0;
}

int getFileType(struct HttpReq *req) {

	req->contType = TEXT_HTML;
	int size = strlen(req->reqLoc);
	if (size >= 4) {
	  	if (strncmp(".jpg", req->reqLoc+size-4, 4) == 0) 
			req->contType = IMG_JPEG;
		else if (strncmp(".png", req->reqLoc+size-4, 4) == 0)	
			req->contType = IMG_PNG;
		else 
			return -1;	
	}

	return 0;


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

	if (getFileName(buf+start, end-start, req) == -1) {
		return -1;
	}

	if (fileEscapeHome(buf+start, end-start)) {
		return -1;
	}

    return 0;
}


