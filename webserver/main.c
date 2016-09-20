#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include "socket.h"


#define BUFF_SIZE 1024


void	welcome_client(int socket_client){
  char welcome[BUFF_SIZE];
  int fd;
  int n = 0;
  
  fd = open("ressources/Welcome", O_RDONLY);
  if (fd == -1)
    {
      perror("open Welcome\n");
    return;
    }
  while ((n = read(fd, welcome, BUFF_SIZE)) > 0){
    write(socket_client, &welcome, n);
  }
  close(fd);
}

void	answer(int socket_client){
  int n = 0;
  char buffer[BUFF_SIZE];
  
  while ((n = read(socket_client, buffer, BUFF_SIZE)) > 0){
      write(socket_client, &buffer, n);
    }
}

void initialiser_signaux(void){
  if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
    perror("signal");
}


int	main(){
  int socket_serveur;
  int socket_client;

  initialiser_signaux();
  socket_serveur = creer_serveur(8080);
  if (socket_serveur == -1)
    return -1;  
  while((socket_client = accept(socket_serveur, NULL,NULL)) > 0){
    if (socket_client == -1){
      perror("socket_client");
      return -1;
    }
    welcome_client(socket_client);
    answer(socket_client);
    close(socket_client);
  }
  return 0;
}
