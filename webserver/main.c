#include <stdio.h>
#include <string.h>

int	main(int argc, char **argv){

  if (argc > 1 && strncmp(argv[1], "-advice", 7) == 0){
    printf("Don't panic!\n");
    return 42;
  }
  printf("Need an advice?\n");
  return 0;
}
