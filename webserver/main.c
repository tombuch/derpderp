#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include "socket.h"
#include "check_header.h"

#define BUFF_SIZE 1024

void	welcome_client(FILE  *socket_client){
  char welcome[BUFF_SIZE];
  int fd;
  FILE *f;
  
  fd = open("ressources/Welcome", O_RDONLY);
  if (fd == -1)
    {
      perror("open Welcome\n");
    return;
    }
  f = fdopen(fd, "r");
  if (f == NULL)
    {
      perror("fopen Welcome\n");
    }
  while (fgets(welcome, BUFF_SIZE, f) != NULL){
    fprintf(socket_client, "%s", welcome);
    }
  fclose(f);
}


void	bad_request(FILE *f){
  fprintf(f, "HTTP/1.1 400 Bad Request\r\n");
  fprintf(f, "Connection: close\r\n");
  fprintf(f, "Content-Length: 17\r\n");
  fprintf(f, "\r\n");
  fprintf(f, "400 Bad request\r\n");
}

void	req_answer(FILE *f){
  fprintf(f, "HTTP/1.1 200 OK\r\n");
  fprintf(f, "Content-Length: 11327\r\n");
  fprintf(f, "\r\n");
  welcome_client(f);
}

void	not_found(FILE *f){
  fprintf(f, "HTTP/1.1 404 Not Found\r\n");
  fprintf(f, "Connection: close\r\n");
  fprintf(f, "Content-Length: 15\r\n");
  fprintf(f, "\r\n");
  fprintf(f, "404 Not Found\r\n");
}


void	answer(FILE  *socket_client){
  char buffer[BUFF_SIZE];
  
  
  if (fgets(buffer, BUFF_SIZE, socket_client) != NULL){
    if (checkget(buffer) == 0){
      bad_request(socket_client);
      return;
    }
  }
  if (strncmp(get_url(buffer), "/", 2) != 0){
    not_found(socket_client);
    return;
  }
  while (fgets(buffer, BUFF_SIZE, socket_client) != NULL){
    if (strncmp(buffer, "\r\n", 2) == 0)
      break;
  }
  req_answer(socket_client);
}

void traitement_signal(int sig)
{
  int status;

  if (sig == SIGCHLD){
    if (waitpid(-1, &status, WNOHANG) == -1){
      perror("waitpid");
    }
  }
}

void initialiser_signaux(void)
{
  struct sigaction sa ;
  sa.sa_handler = traitement_signal;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  if (sigaction(SIGCHLD, &sa, NULL) == -1)
    {
      perror ("sigaction(SIGCHLD)");
    }
  if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
    perror("signal");
}

int	main(){
  int socket_serveur;
  int socket_client;
  pid_t pid;
  FILE *f;
  
  initialiser_signaux();
  socket_serveur = creer_serveur(8080);
  if (socket_serveur == -1)
    return -1;  
  while((socket_client = accept(socket_serveur, NULL,NULL)) > 0){
    if (socket_client == -1){
      perror("socket_client");
      return -1;
    }
    if (( f = fdopen(socket_client, "w+")) == NULL){
      perror("fdopenclient");
      return -1;
    }
    pid = fork();
    if (pid == 0){
      //welcome_client(f);
      answer(f);
      fclose(f);
      exit(EXIT_SUCCESS);
    }
    fclose(f);
  }
  return 0;
}
