#include "handle_http.h"

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

#include "format.h"
#include "http/httpGenerator.h"
#include "http/httpParser.h"

#include "io/fileIO.c"

void handleRecv(char *buf, int size, char **msg_ptr, int* msg_size) {
    struct HttpReq req;
	int fsize;

	struct HttpReplyHeader replyHeader;
	replyHeader.version = v1_1;

    if (parseHttpReq(buf, size, &req) == -1) {
		printf("badrequest: %s %d\n", buf, size);
        replyHeader.retCode = CODE_400;
        replyHeader.contType = TEXT_HTML;
    } else {
		if ((fsize = fileExist(req.reqLoc)) == -1) {
			replyHeader.retCode = CODE_404;
			replyHeader.contType = TEXT_HTML;
		} else if (fileReadable(req.reqLoc) == 0) {
			replyHeader.retCode = CODE_403;
			replyHeader.contType = TEXT_HTML;
		} else {
			replyHeader.retCode = CODE_200;
			replyHeader.contType = req.contType;
		}
	}

    char hbuf[1024];
    int head_size = genHeader(&replyHeader, hbuf, sizeof hbuf);
    if (head_size == -1) {
		perror("head_size error\n");
        return;
    }

	int code = replyHeader.retCode;
	if (code < 0 || code > 3)
		return;
    switch (code) {
    case CODE_200:
        *msg_size = head_size + fsize;
        *msg_ptr = (char*) malloc(*msg_size);
        memcpy(*msg_ptr, hbuf, head_size);
        if (loadFile(*msg_ptr+head_size, req.reqLoc, fsize) == -1) {
			perror("load file error\n");
        }
        break;
    default:
        *msg_size = head_size + sizeof(httpFile[code]);
        *msg_ptr = (char*) malloc(*msg_size);
        memcpy(*msg_ptr, hbuf, head_size);
        memcpy(*msg_ptr+head_size, httpFfile[code], sizeof(httpFile[code]));
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

