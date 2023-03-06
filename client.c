#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>

/* codul de eroare returnat de anumite apeluri */
extern int errno;

/* portul de conectare la server*/
int port;

int main (int argc, char *argv[])
{
  int sd;			// descriptorul de socket
  struct sockaddr_in server;	// structura folosita pentru conectare 
  char buf[500]={0};
  char rspServer[500]={0};
  int plasat=0;

  /* exista toate argumentele in linia de comanda? */
  if (argc != 3)
    {
      printf ("Sintaxa: %s <adresa_server> <port>\n", argv[0]);
      return -1;
    }

  /* stabilim portul */
  port = atoi (argv[2]);

  /* cream socketul */
  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("Eroare la socket().\n");
      return errno;
    }

  /* umplem structura folosita pentru realizarea conexiunii cu serverul */
  /* familia socket-ului */
  server.sin_family = AF_INET;
  /* adresa IP a serverului */
  server.sin_addr.s_addr =inet_addr( argv[1]);
  /* portul de conectare */
  server.sin_port = htons (port);
  
  /* ne conectam la server */
  if (connect (sd, (struct sockaddr *) &server,sizeof (struct sockaddr)) == -1)
    {
      perror ("[client]Eroare la connect().\n");
      return errno;
    }
    
  //afisarea meniului principal
  
  printf("\n\nBine ati venit pe platforma magazinului nostru!\n Daca doriti sa va conectati introduceti comanda \n1.logare 'nume_utilizator ' \n 2.quit \n ");

  
while(1 && strstr(buf,"quit")==NULL){
    printf("\n[client]:");
    fgets(buf,500,stdin);
    
    //trimitem mesajul la server
    if(write(sd,buf,500)<=0){
    perror("Eroare la write() spre server.\n");
    return errno;
    }
    
    if(read(sd,rspServer,500)<0){
    perror("[client]Eroare la read() de la server.\n");
    return errno;
    }
    //afisam mesajul primit
    printf("\n%s",rspServer);
    
    if(strstr(rspServer,"salvati cosul de cumparaturi")!=NULL){
      printf("\n[client]:");
      fgets(buf, 500, stdin);
      
      //trimitem mesajul la server
      if(write(sd,buf,500)<=0){
      perror("Eroare la write() spre server.\n");
      return errno;
      }
    
      if(read(sd,rspServer,500)<0){
      perror("[client]Eroare la read() de la server.\n");
      return errno;
      }
      printf("\n%s",rspServer); 
      strcpy(buf,"quit");
    }
    
    if(strstr(rspServer,"Va rugam introduceti datele de livrare.")!=NULL && plasat==0){
      printf("\n[Adresa]:");
      fgets(buf, 500, stdin);
      
      //trimitem mesajul la server
      if(write(sd,buf,500)<=0){
      perror("Eroare la write() spre server.\n");
      return errno;
      }
    
      if(read(sd,rspServer,500)<0){
      perror("[client]Eroare la read() de la server.\n");
      return errno;
      }
      printf("\n%s",rspServer); 
      strcpy(buf,"quit");
    }
    
  }//cat timp clientul este inca conectat
  
  
 
  //inchidem conexiunea
  close(sd);

}
