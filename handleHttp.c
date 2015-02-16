#include <stdio.h>
#include <memory.h>

#include "format.h"
#include "fileIO.c"


/*
 * parse succ return 0, otherwise, return -1
 */
int parseHttpReq(char *buf, int size, struct HttpReq *req) {
    //native implementation
	//printf("httpReq: %s\n", buf);
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
    memcpy(req->reqLoc, buf + start, end - start);
    return 0;
}

void handleRecv(char *buf, int size, char **msg_ptr, int* msg_size) {
    struct HttpReq req;
	int fsize;
	char file_name[256];

	struct HttpReplyHeader replyHeader;
	replyHeader.version = v1_1;

    if (parseHttpReq(buf, size, &req) == -1) {
		printf("badrequest: %s %d\n", buf, size);
        replyHeader.retCode = 400;
        replyHeader.retMsg = BAD_REQUEST;
        replyHeader.contentType = TEXT_HTML;
    } else {
		genFileName(req.reqLoc, file_name, 256);

		if ((fsize = fileExist(file_name)) == -1) {
			replyHeader.retCode = 404;
			replyHeader.retMsg = NOT_FOUND;
			replyHeader.contentType = TEXT_HTML;
		} else {
			replyHeader.retCode = 200;
			replyHeader.retMsg = OK;
			replyHeader.contentType = TEXT_HTML;
		}
	}

    char hbuf[1024];
    int head_size = genHeader(&replyHeader, hbuf, sizeof hbuf);
    //printf("header is: %d | %shaha\n", head_size, hbuf);
    if (head_size == -1) {
		perror("head_size error\n");
        return;
    }
    const char file_400[] = "<html><body>bad request</body></html>";
    const char file_404[] = "<html><body>request file not found</body></html>";
    switch (replyHeader.retCode) {
    case 400:
        *msg_size = head_size + sizeof file_400;
        *msg_ptr = (char*) malloc(*msg_size);
        memcpy(*msg_ptr, hbuf, head_size);
        memcpy(*msg_ptr+head_size, file_400, sizeof file_400);
		break;
    case 404:
        *msg_size = head_size + sizeof file_404;
        *msg_ptr = (char*) malloc(*msg_size);
        memcpy(*msg_ptr, hbuf, head_size);
        memcpy(*msg_ptr+head_size, file_404, sizeof file_404);
        break;
    case 200:    
        *msg_size = head_size + fsize;
        *msg_ptr = (char*) malloc(*msg_size);
        memcpy(*msg_ptr, hbuf, head_size);

        if (loadFile(*msg_ptr+head_size, file_name, fsize) == -1) {
			perror("load file error\n");
        }
        break;
    default:
        break;
    }
}

void handleSent(char **msg_ptr, int* msg_size) {
    if (*msg_ptr != 0) {
        free(*msg_ptr);
    }
    *msg_ptr = 0;
    *msg_size = 0;
}



