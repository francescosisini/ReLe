#include <stdio.h> /* printf, sprintf */
#include <stdlib.h> /* exit */
#include <unistd.h> /* read, write, close */
#include <string.h> /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h> /* struct hostent, gethostbyname */

#include "libcallidus.h"

#define RESP_SIZE 12096

void error(const char *msg) { perror(msg); exit(0); }

/***: web ***/

void encode_url(char * msg,char *emsg){
  int c;
  char *hex = "0123456789abcdef";
  
  while( c=*(msg++) ){
    if( ('a' <= c && c <= 'z')
	|| ('A' <= c && c <= 'Z')
	|| ('0' <= c && c <= '9') ){
      *(emsg++)=c;
    } else
      {
	*(emsg++)='%';
	*(emsg++)=hex[c >> 4];
	*(emsg++)=hex[c & 15];	
      }
  }
}

char * callidus_GET(int cod_servizio)
{
   
  int portno =        80;
  char *host =        "pumar.local";

   char str[100];
   char *message_fmt;
   message_fmt = malloc(4000*sizeof(char));
   
   
   //strcat(message_fmt,"GET /check/");
   //strcat(message_fmt,str);
   //strcat(message_fmt," HTTP/1.1\r\nHost: www.beembee.it\r\nUser-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1)\r\n\r\n");
   
   message_fmt = "GET /callidus/callidus.php HTTP/1.1\r\nHost: pumar.local\r\nUser-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1)\r\n\r\n";
   
   struct hostent *server;
   struct sockaddr_in serv_addr;
   int sockfd, bytes, sent, received, total;
   char message[4096];
   char * response = (char*)malloc(RESP_SIZE);
      
   sprintf(message,message_fmt);
   //printf("Request:\n%s\n",message);
   
   /* create the socket */
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   if (sockfd < 0) error("ERROR opening socket");

   /* lookup the ip address */
   server = gethostbyname(host);
   if (server == NULL) error("ERROR, no such host");
   
   /* fill in the structure */
   memset(&serv_addr,0,sizeof(serv_addr));
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_port = htons(portno);
   memcpy(&serv_addr.sin_addr.s_addr,server->h_addr,server->h_length);
   
   /* connect the socket */
   if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
     error("ERROR connecting");
   
   /* send the request */
   total = strlen(message);
   sent = 0;
   do {
     bytes = write(sockfd,message+sent,total-sent);
     if (bytes < 0)
       error("ERROR writing message to socket");
     if (bytes == 0)
       break;
     sent+=bytes;
   } while (sent < total);
   
   /* receive the response */
   memset(response,0,RESP_SIZE);
   total = RESP_SIZE-1;
   received = 0;
   do {
     bytes = read(sockfd,response+received,total-received);
     if (bytes < 0)
       {
	 error("ERROR reading response from socket");
	 exit(1);
       }
     if (bytes == 0)
       break;
     received+=bytes;
     
   } while (received < total);
   
   if (received == total)
     printf("ERROR storing complete response from socket...");
   
   /* close the socket */
   close(sockfd);
   
   char *pr = response;
   for(int i=0;i<RESP_SIZE;i++)
     {
       if(*pr == 10 && *(pr+1) == 13)
	 break;
       pr++;
     }
   pr+=3;
   char * mr =malloc(RESP_SIZE-(pr-response));
   memcpy(mr,pr,RESP_SIZE-(pr-response));
   free(response);
   
   return mr;
}

/** Gestione dati*/

int db_inserisci_immobile(Immobile *im)
{
  int chiave = 1;
  int err;
  FILE * f = fopen("Immobile.db","a+b");
  //Mi sposto in fondo al file
  int s;
  s = fseek(f,-sizeof(Immobile),SEEK_END);
  
  printf("###%d\n",s);
  
  fflush(stdout);
  if(s<=-1)
    {
      im->id = chiave;
    }
  else
    {
      Immobile r;
      fread(&r,sizeof(Immobile),1,f);
      chiave = r.id+1;
      im->id = chiave;
    }

  err = fwrite(im,sizeof(Immobile),1,f);
  
  if(err<=0)
    {
      printf("###%d\n",s);
      printf("db: errori inserimento\n");
      exit(1);
    }
  
  fclose(f);
  return chiave;
  
}

int db_aggiorna_immobile(Immobile c)
{
  int s;
  Immobile t;
  FILE * f = fopen("Immobile.db","r+b");
  char trovato = 0;
  printf("Aggiorno per id = %d\n",c.id);
  /* cerca il record */
  do
    {
      s = fread(&t,sizeof(Immobile),1,f);
      if(s<=0)
	{
	  fclose(f);
	  return -1;
	}
      printf("Letto %d\n",t.id);
      if(t.id == c.id) trovato = 1;
      
    }while(!trovato);
  
  /* si posiziona sul record */
  s = fseek(f,-sizeof(Immobile),SEEK_CUR);

  int err = fwrite(&c,sizeof(Immobile),1,f);

  fclose(f);

  if(err<=1) return err;

  return c.id;
}

int  db_seleziona_immobile(Immobile * t, Immobile c, int chiave)
{
  int s;
  FILE * f = fopen("Immobile.db","r+b");
  char trovato = 0;
  
  do
    {
     s = fread(t,sizeof(Immobile),1,f);
     if(s<=0)
       {
	 fclose(f);
	 return -1;
       }
     if(t->id>chiave &&
	(c.area == t->area || c.area == -1) &&
	(c.piani == t->piani || c.piani == -1) &&
	(c.bagni == t->bagni || c.bagni == -1) &&
	(c.giardino == t->giardino || c.giardino == -1) &&
	(c.distanza_centro == t->distanza_centro || c.distanza_centro == -1) &&
	(c.vani == t->vani || c.vani == -1) &&
	(c.tipo_r == t->tipo_r || c.tipo_r == -1) &&
	(c.lati_indipendenti == t->lati_indipendenti || c.lati_indipendenti ==-1)
	) trovato = 1;
     
    }while(!trovato);
  fclose(f);
  return t->id;

}

void db_inizializza_immobile(Immobile *im)
{
  im->area = -1;
  im->piani = -1;
  im->bagni = -1;
  im->giardino = -1;
  im->distanza_centro = -1;
  im->vani = -1;
  im->tipo_r = -1;
  im->lati_indipendenti = -1;
}
