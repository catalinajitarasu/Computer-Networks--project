#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <mysql/mysql.h>

/* portul folosit */
#define PORT 2043


/* codul de eroare returnat de anumite apeluri */
extern int errno;

//pentru data de baze

MYSQL *conn;
MYSQL_RES *res;
MYSQL_ROW row;

char *serverr="localhost";
char *user="root";
char *password="kali";
char *database="MagazinOnline";


int main ()
{
    struct sockaddr_in server;	// structura folosita de server
    struct sockaddr_in from;
    int sd;			//descriptorul de socket
    int status=0;

    /* crearea unui socket */
    if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
    	perror ("[server]Eroare la socket().\n");
    	return errno;
    }
    
    //conectam la baza de date
    
    conn=mysql_init(NULL);
    if(!mysql_real_connect(conn,serverr,user,password,database,0,NULL,0)){
      fprintf(stderr,"%s\n", mysql_error(conn));
      
      }

    /* pregatirea structurilor de date */
    bzero (&server, sizeof (server));
    bzero (&from, sizeof (from));

    /* umplem structura folosita de server */
    /* stabilirea familiei de socket-uri */
    server.sin_family = AF_INET;
    /* acceptam orice adresa */
    server.sin_addr.s_addr = htonl (INADDR_ANY);
    /* utilizam un port utilizator */
    server.sin_port = htons (PORT);

    /* atasam socketul */
    if (bind (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
    {
    	perror ("[server]Eroare la bind().\n");
    	return errno;
    }

    /* punem serverul sa asculte daca vin clienti sa se conecteze */
    if (listen (sd, 1) == -1)
    {
    	perror ("[server]Eroare la listen().\n");
    	return errno;
    }

    /* servim in mod concurent clientii... */
    while (1)
    {
    	int client;
    	int length = sizeof (from);

    	printf ("[server]Asteptam la portul %d...\n",PORT);
    	fflush (stdout);

    	/* acceptam un client (stare blocanta pana la realizarea conexiunii) */
    	client = accept (sd, (struct sockaddr *) &from, &length);

    	/* eroare la acceptarea conexiunii de la un client */
    	if (client < 0)
    	{
    		perror ("[server]Eroare la accept().\n");
    		continue;
    	}

    	int pid;
    	if ((pid = fork()) == -1) {
    		close(client);
    		continue;
    	} else if (pid > 0) {
    		// parinte
    		close(client);
    		// while(waitpid(-1,NULL,WNOHANG));
    		continue;
    	} else if (pid == 0) {
    		// copil
    		close(sd);
    		
    		char msgClient[500]={0};
    		char rspClient[500]={0};
    		char nume_utilizator[100]={0};
    		int Clientconectat=1;
    		
    		//s-a realizat conexiunea iar acum se serveste un client cat timp acesta este conectat
    		while(1 && Clientconectat==1)
    		{
    		  bzero(rspClient,500);
    		  read(client,msgClient,500);
    		  printf("[server] Mesajul primit de la client: %s ", msgClient);
    		  
    		  if(strstr(msgClient,"logare")!=NULL && status==0)
    		  {
    		   
    		   bzero(nume_utilizator,100);
    		   char *token;
    		   const char sep[2]=" ";
    		   token=strtok(msgClient,sep);
    		   while(token!=NULL){
    		      if(strstr(token,"logare")==NULL) 
    		            strcpy(nume_utilizator,token);
    		      token=strtok(NULL,sep);
    		   }
    		   
    		   //in nume_utilizator avem numele de utilizator introdus de client
    		   //verificam daca numele de utilizator se regaseste in baza de date
    		   
    		   
    		   if(mysql_query(conn, "select UserNume from User")){
                   fprintf(stderr, "%s\n", mysql_error(conn));
                   }         

                   res=mysql_use_result(conn);

                   //raspunsul intors de query
                   //verificam daca nume de utilizator introdus este in baza de date
                   //daca este in baza de date atunci status=1
                   //daca nu, status=0

                   while((row=mysql_fetch_row(res))!=NULL)
                    {
                    if(strstr(nume_utilizator, row[0])) 
                       {status=1; break;}
                    else
                       status=0;
                    }
                    
                    
    		   if(status==1){
    		     strcpy(rspClient,"\n\nBine ati venit pe platforma magazinului nostru online! \n Aveti la dispozitie urmatoarele comezi: \n1.Vizualizare categorii \n2.Vizualizare produse din 'categoria' \n3.Adaugare in cos 'nume_produs' \n 5.Stergere din cos 'nume_produs'\n5.Vizualizare cos \n6.Plasare comanda \n7.exit \n 8.quit \n");
    		     //strcpy(rspClient, nume_utilizator);
    		     
    		  }
    		  else {
    		  strcpy(rspClient, "Ne pare rau! Nu sunteti utilizator al magazinului. Introduceti alt nume de utilizator\n");}
    		  
    		  } //logare
    		  
    		  //userul este conectat la magazin, deci acum urmeaza comenzile pentru care status=1; !!!in nume_utilizator avem utilizatorul 
    		  else if(strstr(msgClient,"Vizualizare categorii")!=NULL && status==1){
    		  mysql_free_result(res);
                   if(mysql_query(conn, "select CategorieNume from Categorie")){
                   fprintf(stderr, "%s\n", mysql_error(conn));
                   }   
                   res=mysql_use_result(conn);
                   while((row=mysql_fetch_row(res))!=NULL)
                  { strcat(rspClient, row[0]);
                   strcat(rspClient, "\n");}
    		  } //vizualizare categorii
    		  
                  //continuam cu userul conectat la magazin
    		  else if(strstr(msgClient, "Vizualizare produse din") != NULL && status==1){
    		  //din mesajul primit de la client pun in variabila categorie care este numele categorii introduse de client
    		   char categorie[100]={0};
    		   bzero(categorie,100);
    		   char *token;
    		   const char sep[2]=" ";
    		   int nr=0;
                   int nrProd=0;
    		   token=strtok(msgClient,sep);
    		   while(token!=NULL){
    		      nr=nr+1;
    		   
    		      if(nr==4) 
    		            strcat(categorie,token);
    		      if(nr>4){
    		      strcat(categorie," ");
    		      strcat(categorie,token);
    		      }
    		      token=strtok(NULL,sep);
    		    }
    		   
    		   //interogarea sql
    		   char interogare[300]={0};
    		   bzero(interogare,300);
    		   strcat(interogare,"select ProdusNume, ProdusPret from Produs where CategoriNume=");
          	   strcat(interogare,categorie);
          	   
          	   
    		   mysql_free_result(res);
    		   if(mysql_query(conn, interogare)){
                   fprintf(stderr, "%s\n", mysql_error(conn));
                   }         

                   res=mysql_use_result(conn);
                   while((row=mysql_fetch_row(res))!=NULL)
                   {
                   strcat(rspClient, row[0]);
                   strcat(rspClient,", pret: ");
                   strcat(rspClient, row[1]);
                   strcat(rspClient, "\n");
                    nrProd=nrProd+1;}
    		    if((row=mysql_fetch_row(res))==NULL && nrProd==0){
                   strcat(rspClient,"Ati introdus o categorie care nu exista. Va rugam reincercati!");
                  }

    		  }//produse din categorie
    		  
    		  else if(strstr(msgClient,"Adaugare in cos")!=NULL && status==1){
    		  //numele de utilizator il avem in nume_utilizator
    		  
    		  //in variabila produs avem numele produsul pe care utilizatorul doreste sa il adauge in cos
    		   char produs[100]={0};
    		   bzero(produs,100);
    		   char *token;
    		   const char sep[2]=" ";
    		   int nr=0;
    		   token=strtok(msgClient,sep);
    		   while(token!=NULL){
    		      nr=nr+1;
    		   
    		      if(nr==4) 
    		            strcat(produs,token);
    		      if(nr>4){
    		      strcat(produs," ");
    		      strcat(produs,token);
    		      }
    		      token=strtok(NULL,sep);
    		    }
    		    
    		    
    		  
    		 //verificam mai intai cate produse sunt in stoc;
    		 //daca produsul este in stoc atunci il adaugam in cos, altfel afisam un mesaj
    		   char interogare[300]={0};
    		   bzero(interogare,300);
    		   strcat(interogare,"select ProdusCantitate from Produs where ProdusNume=");
          	   strcat(interogare,produs);
          	   
          	   char cantitate_char[5]={0};
          	   int cantitate;
    		 
    		 
    		   mysql_free_result(res);
    		   if(mysql_query(conn, interogare)){
                   fprintf(stderr, "%s\n", mysql_error(conn));
                   }         

                   res=mysql_use_result(conn);
                   while((row=mysql_fetch_row(res))!=NULL)
                      strcat(cantitate_char, row[0]);
    		    printf("%s\n",cantitate_char);
    		    
    		   sscanf(cantitate_char, "%d", &cantitate); //cast din char* in int
    		 
    		  
    		   if(cantitate == 0){
    		   strcat(rspClient, "Din pacate produsul dorit nu este in stoc! Ne pare rau! Introduceti o alta comanda");
    		   }
    		   else {
    		   //adaugarea produsului in cos
    		   strcpy(interogare, "INSERT INTO CosCump (ProdusNume, UserNume) Values (");
    		   strcat(interogare, produs);
    		   strcat(interogare, ",");
    		   strcat(interogare, nume_utilizator);
    		   strcat(interogare, ")");
    		   
    		   //mysql_free_result(res);
    		   if(mysql_query(conn, interogare)){
                   fprintf(stderr, "%s\n", mysql_error(conn));
                   strcpy(rspClient, "Ati introdus un produs care nu exista, va rugam reincercati!");
                   }         
                   else strcpy(rspClient, "Produsul a fost adaugat cu succes!");
                   
                   //scaderea cantitatii produsului respectiv cu -1
                   cantitate=cantitate-1;
                   sprintf(cantitate_char,"%d",cantitate); //cast din int in char*
                   strcpy(interogare, "UPDATE Produs set ProdusCantitate=");
                   strcat(interogare,cantitate_char);
                   strcat(interogare," where ProdusNume=");
                   strcat(interogare,produs);
                   if(mysql_query(conn, interogare)){
                   fprintf(stderr, "%s\n", mysql_error(conn));
                   }         
                   
    		   } //cand produsul este in stoc 
    		  } //adaugare produs in cos
    		  
    		  else if(status==1 && strstr(msgClient, "Stergere din cos")!=NULL){
    		  
    		  //in variabila produs avem numele produsul pe care utilizatorul doreste sa il stearga din cos
    		   char produs[100]={0};
    		   bzero(produs,100);
    		   char *token;
    		   const char sep[2]=" ";
    		   int nr=0;
    		   int existaProdus=0;
    		   token=strtok(msgClient,sep);
    		   while(token!=NULL){
    		      nr=nr+1;
    		   
    		      if(nr==4) 
    		            strcat(produs,token);
    		      if(nr>4){
    		      strcat(produs," ");
    		      strcat(produs,token);
    		      }
    		      token=strtok(NULL,sep);
    		    }
    		  
    		  //verificam mai intai daca produsul se regaseste in cosul utilizatorului
    		  char interogare[300]={0};
    		   bzero(interogare,300);
    		   strcat(interogare,"select ProdusNume from CosCump where UserNume=");
          	   strcat(interogare,nume_utilizator);
          	   mysql_free_result(res);
    		   if(mysql_query(conn, interogare)){
                   fprintf(stderr, "%s\n", mysql_error(conn));
                   }         

                   res=mysql_use_result(conn);
                   
                   while((row=mysql_fetch_row(res))!=NULL)
                    {
                    if(strstr(produs, row[0])) 
                       existaProdus=1; 
                    }
                    //daca produsul se regaseste in cos atunci il sterg, daca nu, afisam un mesaj
                    
                    if(existaProdus==0){strcpy(rspClient,"Produsul pe care doriti sa il stergeti nu se afla in cosul dumneavoastra. Introduceti alta comanda! "); }
                    else {
                    //stergem produsul din cosul utilizatorului
                    //delete from CosCump where ProdusNume='nume_produs' and UserNume='nume_utilizator' limit 1;
                    strcpy(interogare, "delete from CosCump where ProdusNume=");
                    strcat(interogare, produs);
                    strcat(interogare, " and UserNume=");
                    strcat(interogare, nume_utilizator);
                    strcat(interogare, " limit 1");
                    mysql_free_result(res);
    		   if(mysql_query(conn, interogare)){
                   fprintf(stderr, "%s\n", mysql_error(conn));
                   }         

                   res=mysql_use_result(conn);
                    strcpy(rspClient,"Produsul a fost sters din cos cu succes!");
                    
                    }
    		  
    		  
    		  } //stergerea unui produs din cos
    		  
    		  else if(status==1 && strstr(msgClient, "Vizualizare cos")!=NULL){
    		   char interogare[300]={0};
    		   bzero(interogare,300);
    		   
    		   //mai intai afisam ce produse avem in cos
    		   
    		   strcat(interogare,"select distinct ProdusNume from CosCump where UserNume=");
          	   strcat(interogare,nume_utilizator);
    		  
    		   mysql_free_result(res);
                   if(mysql_query(conn, interogare)){
                   fprintf(stderr, "%s\n", mysql_error(conn));
                   }  
                   
                   strcpy(rspClient, "Aveti urmatoarele produse in cos: \n");

                   res=mysql_use_result(conn);
                   while((row=mysql_fetch_row(res))!=NULL)
                  { strcat(rspClient, row[0]);
                   strcat(rspClient, "\n");}
                   
                   //apoi afisam cate produse sunt in cos
                   strcpy(interogare,"select count(*) from CosCump where UserNume=");
          	   strcat(interogare,nume_utilizator);
                   mysql_free_result(res);
                   if(mysql_query(conn, interogare)){
                   fprintf(stderr, "%s\n", mysql_error(conn));
                   }  
                   
                   strcat(rspClient, "Cosul dumneavoastra contine: ");      

                   res=mysql_use_result(conn);
                   
                   while((row=mysql_fetch_row(res))!=NULL)
                       strcat(rspClient, row[0]);
                        
                   strcat(rspClient, " produse.\n");
    		  
    		  //apoi afisam cat costa tot cosul
    		  int suma=0;
    		  char pret_char[10]={0};
    		  char suma_char[10]={0};
    		  int pret;
    		  strcpy(interogare,"select s.ProdusPret from Produs s JOIN CosCump c on s.ProdusNume=c.ProdusNume where UserNume=");
    		  strcat(interogare, nume_utilizator);
    		   mysql_free_result(res);
                  if(mysql_query(conn, interogare)){
                  fprintf(stderr, "%s\n", mysql_error(conn));
                  }
                  
                 strcat(rspClient, "Totalul cosului dumneavoastra este: ");
                    res=mysql_use_result(conn);
                 while((row=mysql_fetch_row(res))!=NULL)
                       {strcpy(pret_char,row[0]);
                        sscanf(pret_char, "%d", &pret); //cast din char* in int
                        printf("%d",pret);
                        suma=suma+pret; //adaugam la suma
                        //strcat(rspClient,row[0]);
                        //strcat(rspClient," ");
                       }
                 sprintf(suma_char,"%d",suma); //cast din int in char*
                 strcat(rspClient, suma_char);
                 strcat(rspClient, " lei.");
                   
    		  }//vizualizare cos de cumparaturi
    		  
    		  
    		  //salvarea cosului de cumparaturi cand dam comanda "exit"
    		  
    		  else if(strstr(msgClient,"exit")!=NULL && status==1)
    		    {
    		      strcpy(rspClient, "Inainte de a parasi platforma noastra, salvati cosul de cumparaturi ?");
    		      write(client, rspClient, 500); 
    		      
    		      read(client,msgClient,500);
    		      if(strstr(msgClient,"Da")!=NULL){
    		        strcpy(rspClient, " Cosul dumneavoastra a fost salvat.\nLa revedere! Va mai asteptam pe platforma magazinului nostru! ");
    		        
    		      } //Da
    		      else if(strstr(msgClient,"Nu")!=NULL){
    		      //inseamna ca trb sa stergem din cosul de cumparaturi tot
    		      char interogare[300]={0};
    		      bzero(interogare,300);
    		      strcpy(interogare,"delete from CosCump where UserNume=");
          	      strcat(interogare,nume_utilizator);
          	   
    		      mysql_free_result(res);
    		      
    		     if(mysql_query(conn, interogare)){
                     fprintf(stderr, "%s\n", mysql_error(conn));
                     }    
                     
                     strcpy(rspClient, "Cosul dumneavoastra este gol. \nLa revedere! Va mai asteptam pe platforma magazinului nostru! ");     
    		             
    		      } //Nu
    		     Clientconectat=0;
    		    } //salvarea cosului cu exit
    		    
    		   //plasarea comenzii
    		   
    		   else if(strstr(msgClient,"Plasare comanda") !=NULL && status==1){
    		   
    		      strcpy(rspClient, "Va rugam introduceti datele de livrare. ");
    		      
    		      write(client, rspClient, 500); 
    		      
    		      read(client,msgClient,500);
    		      
    		      if(strstr(msgClient,"Strada")!=NULL){
    		      //inseamna ca trb sa stergem din cosul de cumparaturi tot
    		      char interogare[300]={0};
    		      bzero(interogare,300);
    		      strcpy(interogare,"delete from CosCump where UserNume=");
          	      strcat(interogare,nume_utilizator);
          	   
    		      mysql_free_result(res);
    		      
    		     if(mysql_query(conn, interogare)){
                     fprintf(stderr, "%s\n", mysql_error(conn));
                     }    
                     
                     strcpy(rspClient, "Comanda dumneavoastra a fost plasata. Va multumim!");
                          
    		             
    		      } 
    		      Clientconectat=0;
    		}
    		   
    		 //pentru cazurile in care comanda nu exista
    		  else if(status==1 && strstr(msgClient,"logare")!=NULL)
    		    strcpy(rspClient, "Sunteti deja logat la aplicatia noastra. Introduceti o alta comanda!");
    		  else if(strstr(msgClient,"quit")!=NULL)
    		   {strcpy(rspClient, "La revedere! Va mai asteptam pe platforma magazinului nostru! ");
    		     Clientconectat=0;
    		    }
    		  else if(status==1) 
    		    strcpy(rspClient, "Ati introdus o comanda gresita! Reincercati! ");
    		  else if(status==0)
    		    strcpy(rspClient, "Trebuie sa va autentificati. Va rugam introduceti nume de utilizator folosind comanda logare nume_utilizator ");
    		  
    		  write(client, rspClient, 500);
    		} //while(Clientconectat)
    		close(client);
    		exit(0);
                mysql_close(conn);
    	} //copilul

    } //while-ul mare
} //int main
