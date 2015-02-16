#ifndef FORMAT_H
#define FORMAT_H

#include <stdio.h>

/* several enum */
typedef enum {GET} httpReqType;
typedef enum {v1_1} httpVersion;
typedef enum {OK, BAD_REQUEST, NOT_FOUND}   httpRetMsg;
typedef enum {TEXT_HTML, IMG_JPEG} httpContType;

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
char str_httpVersion_v1_1[] = "HTTP/1.1";

char str_httpRetMsg_OK[] = "200 OK\r\n";
char str_httpRetMsg_BAD_REQUEST[] = "400 Bad Request\r\n";
char str_httpRetMsg_NOT_FOUND[] = "404 Not Found\r\n";

char str_httpContType_TEXT_HTML[] = "Content-Type: text/html\r\n";
char str_httpContType_IMG_JPEG[] = "Content-Type: img/jpeg\r\n";

/* related to file parser */
const char home[] = "/home/zagfox/cse124/web";
const char index[] = "/index.html";

#endif /*FORMAT_H*/

