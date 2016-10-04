#include <string.h>
#include <ctype.h>
#include "check_header.h"

int checkget(char *str){
  
  if(strncmp(str,"GET",3) == 0)
    return 1;
  return 0;
}

int checkword(char *str){
  int i = 0;
  int count = 0;
  char c = '\0';
  while (str[i] != '\r'){
    if(isspace(str[i]) && !isspace(c))
      count++;
    c = str[i];
    i++;
  }
  if (str[i] == '\r' && !isspace(c))
    count++;
  return count;
}
