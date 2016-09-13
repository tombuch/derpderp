#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fnctl.h>


void	bienvenue(int socket_client){
  char welcome[1024];
  int fd;
  int n = 0;
  
  fd = open("ressourse/Welcome", O_RONLY);
  if (fd == -1)
    return;
  while ((n = read(fd, welcome, 1024) > 0){
      
    }
}

int	main(int argc, char **argv){

  /*  if (argc > 1 && strncmp(argv[1], "-advice", 7) == 0){
    printf("Don't panic!\n");
    return 42;
  }
  printf("Need an advice?\n");*/

  int socket_serveur;
  int socket_client;
  
  socket_serveur = creer_serveur(8080);
  if (socket_serveur == -1)
    return -1;
  socket_client = accept(socket_serveur, null,null);
  if (socket_client == -1){
    perror("socket_client");
    return -1;
  }
  
  
  return 0;
}
