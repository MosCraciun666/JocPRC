#include <QApplication>
#include <iostream>
#include <QMessageBox>
#include "tetrahedron.h"

#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/stat.h>
#include<netinet/in.h>
#include<fcntl.h>
#include<signal.h>
#include<pthread.h>
#include "netio.c"

#define PORT_SERVER 7777
#define PORTSC 8888
#define ADRESA_SERVER "127.0.0.1"
//era 10.11.23.34
using namespace std;

int OK;
int START_VALID;
bool done=false;
//done = inutil deocamdata

void trateaza_alarma(int sig){
  START_VALID = 1;
  cout<<"A trecut timpul acordat!";
}

int main(int argc, char *argv[])
{
QApplication app(argc, argv);
if (!QGLFormat::hasOpenGL()) 
	{
	cerr << "This system has no OpenGL support" << endl;
	return 1;
	}

Tetrahedron tetrahedron;
/*QMessageBox msgBox;
msgBox.setText("Incepe jocul");
msgBox.exec();*/


QMessageBox tmpBox;
tmpBox.setText("MessageBox temporar pentru teste");
tmpBox.exec();
/**************************CONECTAREA LA SERVER*******************************************/

int buf;
struct sockaddr_in local, rmt;
int sockfd;
int soc;
int id, cul;
int * tabela_clienti = NULL;
signal(SIGALRM,trateaza_alarma);
alarm(5);
sockfd = socket(PF_INET,SOCK_STREAM,0);
set_addr(&local, NULL, INADDR_ANY, 0);
bind(sockfd, (struct sockaddr *)&local, sizeof(local));
set_addr(&rmt,ADRESA_SERVER, 0, PORT_SERVER);
connect(sockfd,(struct sockaddr *)&rmt, sizeof(rmt));
read(sockfd, &buf, sizeof(int));
id = buf;
cul = buf;
cout<<"Id-ul primit este:"<<id<<" iar culoare primita este: "<<cul<<endl;
tetrahedron.setHostPlayerID(id);
int lung;
read(sockfd,&lung,sizeof(int));
tabela_clienti = (int *)realloc(tabela_clienti,lung*sizeof(int));
read(sockfd,tabela_clienti, lung * sizeof(int));
for(int i = 0; i < lung; i ++)
  cout<<tabela_clienti[i]<<" ";
cout<<endl;
OK = 1;
//scrie OK la server . ... adik conexiunea avut succes - e suficient sa scrie ceva ca serverul sa inteleaga ca a avut succes
write(sockfd, &OK, sizeof(int));
while(1){
  int k;
  //acum fiecare este pregatit sa preia id-urile noilor participanti
  read(sockfd,&k,sizeof(int));
  cout<<"Avem un nou coechipier cu id: "<< k <<endl;
  //scriu toti clientii valoarea start , care este 0 initial , prin comanda unui buton aceasta va fi facuta 1
  //procesul vostru va scrie spre server valoarea noua a variabilei de start
  write(sockfd, &START_VALID, sizeof(int));
  //dupa care toti asteapta ca serverul sa le dea GO - INCEPE JOCUL sicronizat printro valoare 1
  int start_sau_nu;
  //daca variabila transmisa catre server de clientul unu era 1 acesta scrie inapoi 1 daca nu scrie ianpoi 0
  //daca clientul citeste 1 din pipe inseamna ca poate da drumu la joc
  cout<<"Cred ca aici e problema!";
  read(sockfd, &start_sau_nu, sizeof(int));
  if(start_sau_nu == 1)
    break;
}
struct sockaddr_in remote, loc;
socklen_t remlen= sizeof(remote);
cout<<"Acum incep sa joc!";
soc = socket(PF_INET,SOCK_STREAM,0);
set_addr(&loc,NULL,INADDR_ANY,0);
cout<<"Inainte de bind!";
bind(soc,(struct sockaddr *)&loc, remlen);
set_addr(&remote,ADRESA_SERVER,0,PORTSC);
//citeste o confirmare
int ma_pot_conecta;
read(sockfd,&ma_pot_conecta,sizeof(int));
  connect(soc,(struct sockaddr *)&remote,remlen);
  int k;
  cout<<"Inainte de read";
  read(soc,&k,sizeof(int));
  cout<<"Dupa read";
  cout<<"Valoare: "<<k;
cout<<"Am intrat in joc !";
close(sockfd);
/**************************REDAREA SCENEI GL*******************************************/
QMessageBox msgBox;
msgBox.setText("Incepe jocul");
msgBox.exec();

tetrahedron.setWindowTitle(QObject::tr("Joc PRC"));
tetrahedron.initializeGL();
tetrahedron.resize(1000, 500);
tetrahedron.paintGL();
tetrahedron.show();
tetrahedron.draw();
/*while(!tetrahedron.isDone())
{tetrahedron.updateGL();tetrahedron.draw();}
exit(0);*/
return app.exec();
}

