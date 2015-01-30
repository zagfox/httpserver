#include <stdio.h>

typedef enum {GET} httpReqType;
struct HttpReq {
    httpReqType reqType;
    char        reqLoc[1024];
};

typedef enum {v1_1} httpVersion;
typedef enum {OK, NOT_FOUND}   httpRetMsg;
typedef enum {TEXT_HTML} httpContentType;
struct HttpReplyHeader {
    httpVersion version;
    int retCode;
    httpRetMsg retMsg;
    httpContentType contentType;
};

char str_httpVersion_v1_1[] = "HTTP/1.1";

char str_httpRetMsg_OK[] = "200 OK\r\n";
char str_httpRetMsg_NOT_FOUND[] = "404 Not Found\r\n";

char str_httpContentType_TEXT_HTML[] = "Content-Type: text/html\r\n";

int genHeader(struct HttpReplyHeader *hdr, char *hbuf, int hbuf_size) {
    int pos = 0;
    switch (hdr->version) {
    case v1_1:
        snprintf(hbuf+pos, hbuf_size-pos, "%s", str_httpVersion_v1_1);
        pos += strlen(str_httpVersion_v1_1);
        break;
    default:    
        return -1;
    }
    snprintf(hbuf+pos, hbuf_size-pos, " ");
    pos += 1;

    switch (hdr->retMsg) {
    case OK:
        snprintf(hbuf+pos, hbuf_size-pos, "%s", str_httpRetMsg_OK);
        pos += strlen(str_httpRetMsg_OK);
        break;
    case NOT_FOUND:
        snprintf(hbuf+pos, hbuf_size-pos, "%s", str_httpRetMsg_NOT_FOUND);
        pos += strlen(str_httpRetMsg_NOT_FOUND);
        break;
    default:    
        return -1;
    }

    switch (hdr->contentType) {
    case TEXT_HTML:
        snprintf(hbuf+pos, hbuf_size-pos, "%s", str_httpContentType_TEXT_HTML);
        pos += strlen(str_httpContentType_TEXT_HTML);
        break;
    default:    
        return -1;
    }
    snprintf(hbuf+pos, hbuf_size-pos, "\r\n");
    pos += 2;

    return pos;
}
