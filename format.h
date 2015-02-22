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
typedef enum {OK, BAD_REQUEST, NOT_FOUND, FORBIDDEN}   httpRetMsg;
typedef enum {TEXT_HTML, IMG_JPEG, IMG_PNG} httpContType;

/* struct */
struct HttpReq {
    httpReqType  reqType;
	httpContType contType;
    char         reqLoc[1024];
};

struct HttpReplyHeader {
    httpVersion version;
    int retCode;
    httpRetMsg retMsg;
    httpContType contType;
};

/* string */
const static char str_httpVersion_v1_1[] = "HTTP/1.1";

const static char str_httpRetMsg_OK[] = "200 OK\r\n";
const static char str_httpRetMsg_BAD_REQUEST[] = "400 Bad Request\r\n";
const static char str_httpRetMsg_NOT_FOUND[] = "404 Not Found\r\n";
const static char str_httpRetMsg_FORBIDDEN[] = "403 Forbidden\r\n";

const static char str_httpContType_TEXT_HTML[] = "Content-Type: text/html\r\n";
const static char str_httpContType_IMG_JPEG[] = "Content-Type: img/jpeg\r\n";
const static char str_httpContType_IMG_PNG[] = "Content-Type: img/png\r\n";

/* related to file parser */
const static char home[] = "/home/zagfox/cse124/web";
const static char index_html[] = "index.html";

#endif /*FORMAT_H*/

