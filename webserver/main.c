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
#include "fgets_exit.h"
#include "socket.h"
#include "check_header.h"
#include "http_request.h"
#include "http_serv.h"


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
  while (fgets_or_exit(welcome, BUFF_SIZE, f) != NULL){
    fprintf(socket_client, "%s", welcome);
    }
  fclose(f);
}

void	send_status(FILE *client, int code, const char *reason_phrase){
  fprintf(client, "HTTP/1.1 %d %s\r\n", code, reason_phrase);  
}

void	send_reponse(FILE *client, int code, const char *reason_phrase, const char *message_body){
  send_status(client, code, reason_phrase);
  fprintf(client, "Content-Length: %d\r\n", (int)strlen(message_body));
  fprintf(client, "\r\n");
  fprintf(client, "%s", message_body);
}

void	answer(FILE  *socket_client){
  char buffer[BUFF_SIZE];
  http_request request;
  
  fgets_or_exit(buffer, BUFF_SIZE, socket_client);
  if (parse_http_request(buffer, &request) == 0){
    send_reponse(socket_client, 400, "Bad Request", "Bad Request\r\n");
    return;
  }
  if (request.method == HTTP_UNSUPPORTED){
    send_reponse(socket_client, 405, "Method Not Allowed", "Method Not Allowed\r\n");
    return;
  }
  if (strncmp(request.target, "/", 2) != 0){
    send_reponse(socket_client, 404, "Not Found", "Not Found\r\n");
    return;
  }
  skip_headers(socket_client);
  send_reponse(socket_client, 200, "OK", "Bonjour\r\n");
  //  req_answer(socket_client);
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
