#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include "check_header.h"


int	parse_http_request(const char *request_line, http_request *request){

  if (strncmp(request_line, "GET", 3) == 0){
    request->method = HTTP_GET;
  }
  else
    request->method = HTTP_UNSUPPORTED;
  int i = 0;
  while (!isspace(request_line[i]) && request_line[i])
    i++;
  if (!isspace(request_line[i]))
    return 0;
  i++;
  while (!isspace(request_line[i]) && request_line[i])
    i++;
  if (!isspace(request_line[i]))
    return 0;
  i++;
  if ((strncmp(&request_line[i], "HTTP/1.0", 8) != 0) && (strncmp(&request_line[i], "HTTP/1.1", 8) != 0))
    return 0;
  i += 8;
  if (request_line[i] != '\n' && (request_line[i] != '\r' && request_line[i+1] != '\n'))
    return 0;
  char *str;
  char *save = strdup(request_line);
  str = strtok(save, " ");
  str = strtok(NULL, " ");
  request->target = rewrite_target(str);
  fprintf(stderr, "%s\n", request->target);
  str = strtok(NULL, " ");
  request->major_version = atoi(&str[5]);
  request->minor_version = atoi(&str[7]);
  return 1;
}

char	*rewrite_target(char *target){

  int i = 0;  
  while (target[i] != '\0'){
    if (target[i] == '?')
      target[i] = '\0';
    i++;
  }
  return target;
}

/*int checkget(char *str){
  int i = 0;
  
  if(strncmp(str,"GET",3) == 0)
    i = 1;
  if((checkvers(str) == 3) && i==1)
    return 1;
  return 0;
}

int checkvers(char *str){
  int i = 0;
  int count = 0;
  char c = '\0';
  int stock = 0;
  while (str[i] != '\r' && str[i] != '\n' && str[i] != '\0'){
    if(isspace(str[i]) && !isspace(c))
      count++;
    c = str[i];
    if (count == 2 && !isspace(str[i]) && stock == 0){
      stock = i;
    }
    i++;
  }
  if ((str[i] == '\r' || str[i] == '\n' || str[i] == '\0') && !isspace(c) )
    count++;
  if(strncmp(&str[stock] , "HTTP/1.1", 8) != 0 && strncmp(&str[stock], "HTTP/1.0",8) != 0)
    return -1;
  return count;
}

char  *get_url(char *str){
  char *res;
  int i = 0;
  int y = 0;
  while (str[i] != '/'){
    i++;
    y = i;
  }
  while (!isspace(str[i])){
      i++;
    }
  if ((res = malloc(sizeof(char) *(i-y+1))) == NULL){
    perror("malloc");
  }
  int n = 0;
  while (y < i){
    res[n] = str[y];
    y++;
    n++;
  }
  res[n] = '\0';
  return res;
}
*/
