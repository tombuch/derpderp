#include <string.h>
#include <ctype.h>
#include "check_header.h"

int checkget(char *str){
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
