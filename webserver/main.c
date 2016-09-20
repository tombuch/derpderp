#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include "socket.h"

void	welcome_client(int socket_client){
  char welcome[1024];
  int fd;
  int n = 0;
  
  fd = open("ressources/Welcome", O_RDONLY);
  if (fd == -1)
    {
      perror("open Welcome\n");
    return;
    }
  while ((n = read(fd, welcome, 1024)) > 0){
      write(socket_client, &welcome, n);
    }
}

int	main(){
  int socket_serveur;
  int socket_client;
  
  socket_serveur = creer_serveur(8080);
  if (socket_serveur == -1)
    return -1;
  socket_client = accept(socket_serveur, NULL,NULL);
  if (socket_client == -1){
    perror("socket_client");
    return -1;
  }
  welcome_client(socket_client);
  return 0;
}
