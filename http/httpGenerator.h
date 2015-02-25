#include "format.h"

int genHeader(struct HttpReplyHeader *hdr, char *hbuf, int hbuf_size) {
    int code, type;
	int pos = 0;

	//http version
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

	//ret code, msg
	code = hdr->retCode;
	snprintf(hbuf+pos, hbuf_size-pos, "%s", httpRetMsg[code]);
	pos += strlen(httpRetMsg[code]);

	//content type
	type = hdr->contType;
	snprintf(hbuf+pos, hbuf_size-pos, "%s", str_httpContType[type]);
	pos += strlen(str_httpContType[type]);
    
    snprintf(hbuf+pos, hbuf_size-pos, "\r\n");
    pos += 2;

    return pos;
}
