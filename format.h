#ifndef FORMAT_H
#define FORMAT_H

#include <stdio.h>

struct config {
	int mode;
	int port;
};

/* several enum */
typedef enum {GET} httpReqType;
typedef enum {v1_1} httpVersion;

// http ret code
typedef enum {
	CODE_200 = 0,
	CODE_400 = 1,
	CODE_403 = 2,
	CODE_404 = 3
} httpRetCode;

const static char* httpRetMsg[4] = {
	"200 OK\r\n",
	"400 Bad Request\r\n",
	"403 Forbidden\r\n",
	"404 Not Found\r\n"
};

const static char* httpFile[4] = {
	"!!!should not be used!!!",
	"<html><body>Bad Request</body></html>",
	"<html><body>403 Forbidden</body></html>",
	"<html><body>Request File Not Found</body></html>"
};

typedef enum {
	TEXT_HTML = 0, 
	IMG_JPEG  = 1,
	IMG_PNG   = 2
} httpContType;

const static char* str_httpContType[3] = {
	"Content-Type: text/html\r\n",
	"Content-Type: img/jpeg\r\n",
	"Content-Type: img/png\r\n"
};

/* struct */
struct HttpReq {
    httpReqType  reqType;
	httpContType contType;
    char         reqLoc[1024];
};

struct HttpReplyHeader {
    httpVersion version;
	httpRetCode retCode;
    httpContType contType;
};

/* string */
const static char str_httpVersion_v1_1[] = "HTTP/1.1";

/* related to file parser */
const static char home[] = "/home/zagfox/cse124/web";
const static char index_html[] = "index.html";

#endif /*FORMAT_H*/
