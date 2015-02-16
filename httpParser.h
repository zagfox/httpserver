#include <stdio.h>
#include <memory.h>
#include "format.h"

void getFileNameType(char path[], int size, struct HttpReq *req) {
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
	//printf("s %d, e %d\n", start, end);
	//printf("file name:%s\n", req->reqLoc);

    return 0;
}


