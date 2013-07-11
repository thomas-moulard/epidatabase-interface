/*
** libepidb.c for libepidb in /
**
** Made by Thomas Moulard
** Login   moular_t
**
** Started on  26/06/2005 moular_t
** Last update 26/06/2005 moular_t
**
*/
#include "libepidb.h"


epidb * epidb_connect(const char *host, const char *user, const char *password)
{
      WSADATA wsaData;
      struct sockaddr_in hostaddr;
      char sockbuf[255];
      
      epidb *sock;
      sock=malloc(1*sizeof(epidb));
      
      WSAStartup(MAKEWORD(1,1), &wsaData);
      *sock = socket(PF_INET,SOCK_STREAM,0);
      if(*sock<0)
         return NULL;
         
      hostaddr.sin_family = AF_INET;
      hostaddr.sin_addr.s_addr = inet_addr(host);
      hostaddr.sin_port = htons(DEFAULT_PORT);
      
      recv(*sock, sockbuf, sizeof(sockbuf), 0);
      PRINT_MESSAGE();
      
      strcpy(sockbuf, user);
      send(*sock,  sockbuf, sizeof(sockbuf), 0);
      
    
    /* RECV - +OK USER */
    recv(*sock, sockbuf, sizeof(sockbuf), 0);
    PRINT_MESSAGE();
    
    if(! RECV_IS_OK )
    {
         printf("Error: authentification failure (code 1).\n");
         epidb_close(sock);
         return NULL;
    }
    
    /* SEND - <user> */
    strcpy(sockbuf, user);
    send(*sock,  sockbuf, sizeof(sockbuf), 0);

    /* RECV - +OK PASSWORD */
    recv(*sock, sockbuf, sizeof(sockbuf), 0);
    PRINT_MESSAGE();
    
    if(! RECV_IS_OK )
    {
         printf("Error: authentification failure (code 2).\n");
         epidb_close(sock);
         return NULL;
    }
    
    /* SEND - <password> */
    strcpy(sockbuf, password);
    send(*sock,  sockbuf, sizeof(sockbuf), 0);
    
    /* RECV - +OK Authentification has successed. Wait for query. */
    recv(*sock, sockbuf, sizeof(sockbuf), 0);
    PRINT_MESSAGE();
    
    if(! RECV_IS_OK )
    {
         printf("Error: authentification failure (code 3).\n");
         epidb_close(sock);
         return NULL;
    }
    
    return sock;
}

void epidb_close(epidb *sock)
{
     if(sock>0)
        closesocket(*sock);
     WSACleanup();
}

void epidb_unbuffered_query(char *host, char *username, char *password, epidb *sock)
{
     char buffer[255], sockbuf[65535];
    
    /* SEND - <query> */
    send(*sock, buffer, sizeof(buffer), 0);
    
    /* RECV - +RES <error_code> */
    recv(*sock, sockbuf, sizeof(sockbuf), 0);
    PRINT_MESSAGE();
    if(sockbuf[5]=='0')
       printf("The query has been processed with success by epiDatabase.\n");
    else
       printf("EpiDatabase has returned an error: %s\n",&sockbuf[5]);
}

epidbres * epidb_query(char *host, char *username, char *password, epidb *sock)
{
    char buffer[255], sockbuf[65535];
    epidbres *res = NULL;
     
    /* SEND - <query> */
    send(*sock, buffer, sizeof(buffer), 0);
    
    /* RECV - +RES <error_code> */
    recv(*sock, sockbuf, sizeof(sockbuf), 0);
    PRINT_MESSAGE();
    if(sockbuf[5]=='0')
       printf("The query has been processed with success by epiDatabase.\n");
    else
       printf("EpiDatabase has returned an error: %s\n",&sockbuf[5]);
    
    /* RECV - +RES <DATA> */
    while(strncmp(sockbuf, END_MESSAGE, 8)!=0)
    {
       recv(*sock, sockbuf, sizeof(sockbuf), 0);
       PRINT_MESSAGE();
    }
    
    return res;
}
