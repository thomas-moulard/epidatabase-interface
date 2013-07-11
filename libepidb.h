/*
** libepidb.h for libepidb in /
**
** Made by Thomas Moulard
** Login   moular_t
**
** Started on  26/06/2005 moular_t
** Last update 26/06/2005 moular_t
**
** #define DEBUG for debug mode
*/
#ifndef LIBEPIDB_H_
# define LIBEPIDB_H_

# include <stdio.h>
# include <stdlib.h>
# include <winsock.h>
# include <signal.h>

# include <sys/types.h>
# include <sys/stat.h>

# define BUFFER_LEN   65535
# define DEFAULT_PORT 2278

# define RECV_IS_OK ((sockbuf[0]=='+') && (sockbuf[1]=='O') && (sockbuf[2]=='K'))

# ifdef DEBUG
#  define PRINT_MESSAGE() printf("%s\n",&sockbuf[3]);
# else
#  define PRINT_MESSAGE()
# endif

# define END_MESSAGE "+OK Bye!"

typedef SOCKET epidb;
typedef char** epidbres;

epidb * epidb_connect(const char *host, const char *user, const char *password);
void epidb_close(epidb *sock);
void epidb_unbuffered_query(char *host, char *username, char *password, epidb *sock);
epidbres * epidb_query(char *host, char *username, char *password, epidb *sock);

#endif /* !LIBEPIDB_H_ */
