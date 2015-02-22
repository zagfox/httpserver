#include "handle_http.h"

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

#include "format.h"
#include "fileIO.c"
#include "httpGenerator.h"
#include "httpParser.h"


void handleRecv(char *buf, int size, char **msg_ptr, int* msg_size) {
    struct HttpReq req;
	int fsize;

	struct HttpReplyHeader replyHeader;
	replyHeader.version = v1_1;

    if (parseHttpReq(buf, size, &req) == -1) {
		printf("badrequest: %s %d\n", buf, size);
        replyHeader.retCode = 400;
        replyHeader.retMsg = BAD_REQUEST;
        replyHeader.contType = TEXT_HTML;
    } else {
		//printf("file path: %s\n", req.reqLoc);
		if ((fsize = fileExist(req.reqLoc)) == -1) {
			replyHeader.retCode = 404;
			replyHeader.retMsg = NOT_FOUND;
			replyHeader.contType = TEXT_HTML;
		} else if (fileReadable(req.reqLoc) == 0) {
			replyHeader.retCode = 403;
			replyHeader.retMsg = FORBIDDEN;
			replyHeader.contType = TEXT_HTML;
		} else {
			replyHeader.retCode = 200;
			replyHeader.retMsg = OK;
			replyHeader.contType = req.contType;
		}
	}

    char hbuf[1024];
    int head_size = genHeader(&replyHeader, hbuf, sizeof hbuf);
    //printf("header is: %d | %shaha\n", head_size, hbuf);
    if (head_size == -1) {
		perror("head_size error\n");
        return;
    }
    const char file_400[] = "<html><body>Bad Request</body></html>";
    const char file_403[] = "<html><body>403 Forbidden</body></html>";
    const char file_404[] = "<html><body>Request File Not Found</body></html>";
    switch (replyHeader.retCode) {
    case 400:
        *msg_size = head_size + sizeof file_400;
        *msg_ptr = (char*) malloc(*msg_size);
        memcpy(*msg_ptr, hbuf, head_size);
        memcpy(*msg_ptr+head_size, file_400, sizeof file_400);
		break;
	case 403:
        *msg_size = head_size + sizeof file_403;
        *msg_ptr = (char*) malloc(*msg_size);
        memcpy(*msg_ptr, hbuf, head_size);
        memcpy(*msg_ptr+head_size, file_403, sizeof file_403);
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
        if (loadFile(*msg_ptr+head_size, req.reqLoc, fsize) == -1) {
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

