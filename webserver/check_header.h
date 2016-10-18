#ifndef __CHECK_HEADER_H__
#define __CHECK_HEADER_H__

#include "http_request.h"

int	parse_http_request(const char *request_line, http_request *request);
char	*rewrite_target(char *target);

/*int checkget(char *str);
int checkvers(char *str);
char * get_url(char *str);*/
#endif
