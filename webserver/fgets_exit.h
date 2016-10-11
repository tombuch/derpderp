#ifndef	__FGETS_EXIT__
#define __FGETS_EXIT__

char	*fgets_or_exit(char *buffer, int size, FILE *stream);
void	skip_headers(FILE *client);

#endif
