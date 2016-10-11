#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "http_serv.h"
#include "fgets_exit.h"


char	*fgets_or_exit(char *buffer, int size, FILE *stream){

  if (fgets(buffer, size, stream) == NULL)
    exit(EXIT_FAILURE);
  return buffer;
}

void	skip_headers(FILE *client){
  char buffer[BUFF_SIZE];
  
  while (fgets_or_exit(buffer, BUFF_SIZE, client)){ 
    if (strncmp(buffer, "\n", 1) == 0)
      return;
    if (strncmp(buffer, "\r\n", 2) == 0)
      return;
  }
}
