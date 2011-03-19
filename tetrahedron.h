#include<QtOpenGL>
#include<QtGui>
#include<QMessageBox>
#include<time.h>
#include<errno.h>

#define MAXGROUPS 200
//aAjuns(), metoda al clasei nava este acum inutila
//poti renunta la nava.groupID?????????
//float cursorx,cursory;
//MESAJUL SETORIGIN PUTE!!

class planeta
{
private:
    float x,y,radius;
    int playerID;
    float population,maxpopulation;
public:
    planeta(float x=0,float y=0, float radius=0.25,float playerID=0,float maxpopulation=10)
    {
this->x=x;
this->y=y;
this->radius=radius;
this->playerID=playerID;
this->maxpopulation=maxpopulation;
this->population=maxpopulation;
    }
    void setX(float x) {this->x=x;}
    void setY(float y) {this->y=y;}
    void setRadius(float r) {radius=r;}
    void setPlayerID(int pid) {playerID=pid;}
    float getX() {return x;}
    float getY() {return y;}
    float getRadius() {return radius;}
    int getPlayerID() {return playerID;}
    bool mouseOver(float cx,float cy)//nu am cum sa iau direct coord cursorului; bine asa mai ales pentru eventuale scimbari
    {
        float dist,i;
        dist=pow(x-cx,2)+pow(y-cy,2);
        dist=sqrt(dist);
        if(dist<=radius)
        {
            glLoadIdentity();
            glTranslatef(x,y,-20);
            glBegin(GL_LINE_STRIP);
            for(i=0;i<=6.3;i+=0.1)
            {
                glColor3f(sin(i),0,cos(i));
                glVertex2f(1.2*radius*sin(i),1.2*radius*cos(i));
            }
            glEnd();
            return true;
        }
        else return false;
        return false;
     }
    void setPlayerID(float playerID) {this->playerID=playerID;}
    void modifyPopulation(float inc) {if(population+inc>=0) population+=inc;}

    int getPopulation() {return population;}
    int getMaxPopulation() {return maxpopulation;}
};


class nava
{
private :
float x,y,angle;
float xd,yd,angled;
int playerID,numberOfPlanets,groupID;
float xignore,yignore,rignore;//coordonatele planetei care este ignorata (planeta destinatie la miscare)
planeta lp[20];//lista cu planete care trebuiesc adaugate
void avoidPlanet(planeta pl)
{
float xnext,ynext,dist,x2,y2;//x2 e inutil, x2 si y2 sunt coordonatele centrului planetei din punctul de vedere al navei
float i;
for(i=0;i<1;i+=0.2)
{
xnext=x+i*cos(angle*3.14/180);
ynext=y+i*sin(angle*3.14/180);

dist=pow(xnext-pl.getX(),2)+pow(ynext-pl.getY(),2);//ERA X-p1,getX() BA BOULE
dist=sqrt(dist)*1.2;

if(dist<pl.getRadius()*1.2)//ca sa nu evite planeta la limita
    {
    x2=pl.getX();
    y2=pl.getY();
    x2=x2-x;
    y2=y2-y;
    y2=y2+dist*sin(angle);
    x2=x2+dist*cos(angle);
    if(y2<0) angle+=10;//initial era 20
    if(y2>0) angle-=10;
    }
}
}

public:

void reset() {numberOfPlanets=0;}
bool aAjuns()//A DEVENIT INUTILA!!!!
{
float dist;
dist=pow(x-xignore,2)+pow(y-yignore,2);
dist=sqrt(dist);
if(dist<=rignore*0.95) return true;
return false;
}

bool impact(planeta *p,float xorigine,float yorigine)
{
 float dist,xp,yp,radius;
xp=(*p).getX();
yp=(*p).getY();
radius=(*p).getRadius();
dist=pow(xp-x,2)+pow(yp-y,2);
dist=sqrt(dist);

if(dist<=radius && xp!=xorigine && yp!=yorigine)//radius*0.8
    {
    if((*p).getPopulation()==0)
    {
     if((*p).getPlayerID()!=playerID)   (*p).setPlayerID(playerID);
     else (*p).modifyPopulation(+1);
    }
    else {
        if((*p).getPlayerID()!=playerID)  (*p).modifyPopulation(-1);
        else (*p).modifyPopulation(+1);
            }
    return true;
    }
return false;
}

nava(float x=-500,float y=-500,float angle=0,int playerID=0,int gid=0)
{
this->x=x;
this->y=y;
this->angle=angle;
this->playerID=playerID;
groupID=gid;
xd=0;yd=0;angled=0;
numberOfPlanets=0;
xignore=-10000;
yignore=-10000;
rignore=0;
}
void addPlanet(planeta p) {lp[numberOfPlanets]=p;numberOfPlanets++;}
void ignorePlanet(float xign,float yign,float rign) {xignore=xign;yignore=yign;rignore=rign;}
void move(float xdest,float ydest)
{
    int i;
angled=atan((ydest-y)/(xdest-x));
angled=angled*180/3.14;//din radiani in grade
if(xdest-x<0) angled+=180;
for(i=0;i<numberOfPlanets;i++)
{
    if(lp[i].getX()!=xignore || lp[i].getY()!=yignore)
     avoidPlanet(lp[i]);

}

while(angled>=360) angled-=360;
while(angled<0) angled+=360;
while(angle>=360) angle=angle-360;
while(angle<0) angle+=360;
if(angle<angled) {if(abs(angle-angled)<180)angle+=5;//poate si astea ar trebui sa depinda de speed
    else angle-=5;}
if(angle>angled) {if(abs(angle-angled)<180) angle-=5;
    else angle+=5;}
while(angle>=360) angle=angle-360;
while(angle<0) angle+=360;
float speed=0.05;//o sa o masuram cand avem un schelet bun in JocPRC.cpp

x+=speed*cos(angle*3.14/180);
y+=speed*sin(angle*3.14/180);
}


float getX() {return x;}
float getY() {return y;}
float getAngle() {return angle;}
int getNumberOfPlanets() {return numberOfPlanets;}

void setXY(float x,float y) {this->x=x;this->y=y;}
void setPlayerID(int ID) {playerID=ID;}
int getPlayerID() {return playerID;}
void setGroupID(int gid) {groupID=gid;}
void setAngle(float a) {angle=a;}
};








class grupDeNave
{
private:
nava n[6];
int nrNave;
float xorigine,yorigine;//planeta din care provin
float xignore,yignore,rignore,xdest,ydest,rdest;//sa tina minte x,y destinatie si cand am luat mouse-ul de pe planeta
bool ajuns[6];
//un set de x,y,r este redundant? (sunt identice)
public:
void setOrigin(float xo,float yo)//poti renunta la xorigine si yorigine
{
xorigine=xo;
yorigine=yo;
}
void setFlag(int index,bool value) {ajuns[index]=value;}
void setNumberOfShips(int ns) {nrNave=ns;}
bool getFlag(int index) {return ajuns[index];}
bool grupLiber()
{
    int i;
    bool rez=true;
    for(i=0;i<nrNave;i++)
    {
        printf("%d,",ajuns[i]);
    if(!ajuns[i]) rez=false;
    }
    printf("\n");
    return rez;
}

void setPlayerID(int ID)
{
    int i;
    for(i=0;i<6;i++) n[i].setPlayerID(ID);
}

grupDeNave() {
    int i;
    nrNave=6;
    for(i=0;i<6;i++) ajuns[i]=false;
    nrNave=0;xignore=-10000;yignore=-10000;rignore=0;xdest=-10000;ydest=-10000;
}
void reset() {
    int i;
    for(i=0;i<6;i++) ajuns[i]=false;
    nrNave=0;xignore=-10000;yignore=-10000;rignore=0;xdest=-10000;ydest=-10000;}//aiureala doar ca sa rezolv o eroare
void resetShip(int index,int xnou,int ynou) {n[index].setXY(xnou,ynou);}
float getXOrigine() {return xorigine;}
float getYOrigine() {return yorigine;}
//float getOrigin
void adaugaNava(nava nav)
    {
    if(nrNave<6) {
        n[nrNave]=nav;
        n[nrNave].setGroupID(nrNave);
        nrNave++;
    }
    else printf("eroare,prea multe nave in grup\n");
    }
void setDest(float x,float y,float r) {xdest=x;ydest=y;rdest=r;
    xignore=x;yignore=y;rignore=r;
}
void miscaGrup()
{
int i;
float div[6]={1,2,2,3,3,4},temp;

if(xignore!=-10000&&yignore!=-10000)
for(i=0;i<nrNave;i++)
    {
    float x2,y2;//auxiliare pt navele care tre sa fie mai "pe margine"
                //d reprezinta distanta care ar trebui sa o aiba navele intre ele
    float d=(float)i/div[i];
    d=d*rdest*0.5;
    if(i%2==1) d=-d;
    x2=xdest+d*cos(3.14/2+n[i].getAngle()*3.14/180);
    y2=ydest+d*sin(3.14/2+n[i].getAngle()*3.14/180);

    if(ajuns[i])
    {
    glLoadIdentity();
    glTranslatef(getNava(i).getX(),getNava(i).getY(),-20);
    glScalef(0.5,0.5,0.5);
    glColor3f(0.5,0.5,0.5);
    glBegin(GL_LINE_STRIP);
    glVertex2f(-1,-1);
    glVertex2f(1,-1);
    glVertex2f(1,1);
    glVertex2f(-1,1);
    glVertex2f(-1,-1);
    glEnd();
    }

    glLoadIdentity();
    glTranslatef(0,0,-20);
    glColor3f(1,(float)i/3,0);
    glBegin(GL_LINES);
    glVertex2f(x2-0.25,y2);
    glVertex2f(x2+0.25,y2);
    glVertex2f(x2,y2-0.25);
    glVertex2f(x2,y2+0.25);
    glEnd();

n[i].move(x2,y2);
n[i].ignorePlanet(xignore,yignore,rignore);
if(n[i].aAjuns())//A DEVENIT INUTIL!!!!!!!!!!!!!!!!!
{/*
n[i].setXY(xorigine,yorigine);
if(xorigine>xdest && yorigine>ydest) n[i].setAngle(225);
if(xorigine>xdest && yorigine<ydest) n[i].setAngle(135);
if(xorigine<xdest && yorigine>ydest) n[i].setAngle(315);
if(xorigine<xdest && yorigine<ydest) n[i].setAngle(45);
*/
}
    }
}

void ignorePlanet(float xign,float yign,float rign){xignore=xign;yignore=yign;rignore=rign;}
nava getNava(int index) {return n[index];}
float getXDest() {return xdest;}
float getYDest() {return ydest;}
float getRDest() {return rdest;}

};








class Tetrahedron : public QGLWidget
{
Q_OBJECT
private:
 GLfloat playerColors[5][3];
GLfloat xx,mousex,mousey,cursorx,cursory;
GLfloat nextOriginX,nextOriginY;//originea urmatorului grup
GLfloat rotationX;
GLfloat rotationY;
GLfloat rotationZ;
GLuint textureID;
time_t t1,t2;//pentru rezolvarea unei erori (influenteaza sensibilitatea mouse-ului)
time_t t_crestere1,t_crestere2;
int hostPlayerID;
int numberOfPlanets,numberOfShips,numberOfClicks,numberOfGroups;
int socket;
bool joaca[4];//cine joaca; joaca[0]=false=>1 nu joaca;joaca[1]=true=>2joaca

planeta p[20];
nava n[200];//poate 1200!!!
grupDeNave g[MAXGROUPS];

float wwidth,wheight;
    public:
void sendMessage(int sockfd,char *msg,int size)
{

    char tmp[512];
    int trimis;

    sprintf(tmp,"%d %s",hostPlayerID,msg);//atasam id-ul in fata mesajului,ca sa stim de la cine vine
    trimis=write(sockfd,tmp,512);
    usleep(25);//pentru ca se pot trimite mesaje mai repede decat se pot procesa
                //iar 25 us sunt nesemnificative

    printf("Scena trimite mesajul \":%s\" pe socket %d, marime: %d\n",tmp,sockfd,trimis);
   if(trimis==-1 && errno==EBADF) printf("Socketul e inchis!:%d\n",errno);
}

void processMessage(char *message)
{

char type[50],aux[50];
int iarg[5];
float farg[5];
int ID;
sscanf(message,"%d%s",&ID,type);
if(ID!=hostPlayerID)//daca eu l-am trimis nu vreau sa execut aceeasi instructiune de 2x
{
if(strcmp(type,"MOD_POP")==0)
    {
    sscanf(message,"%d%s%d%d",&ID,aux,&iarg[0],&iarg[1]);//nu stiu dinainte daca sunt float sau int si nu stiu nici cate sunt
    p[iarg[0]].modifyPopulation(iarg[1]);
    }
if(strcmp(type,"SET_DEST")==0)
    {
    sscanf(message,"%d%s%d%f%f%f",&ID,aux,&iarg[0],&farg[0],&farg[1],&farg[2]);
    printf("Am luat: %d %s %d %f %f %f",ID,aux,iarg[0],farg[0],farg[1],farg[2]);
    g[iarg[0]].setDest(farg[0],farg[1],farg[2]);
    }
if(strcmp(type,"RESET_SHIP")==0)
    {
    sscanf(message,"%d%s%d%d%f%f",&ID,aux,&iarg[0],&iarg[1],&farg[0],&farg[1]);
    g[iarg[0]].resetShip(iarg[1],farg[0],farg[1]);
    }
if(strcmp(type,"SET_FLAG_FALSE")==0)//NETESTAT
    {
    sscanf(message,"%d%s%d%d",&ID,aux,&iarg[0],&iarg[1]);
    g[iarg[0]].setFlag(iarg[1],false);
    }
if(strcmp(type,"SET_NUMBER_OF_SHIPS")==0)//NETESTAT
    {
    sscanf(message,"%d%s%d%d",&ID,aux,&iarg[0],&iarg[1]);
    g[iarg[0]].setNumberOfShips(iarg[1]);
    }
if(strcmp(type,"SET_GROUP_PLAYERID")==0)//ok
    {
    sscanf(message,"%d%s%d%d",&ID,aux,&iarg[0],&iarg[1]);
    g[iarg[0]].setPlayerID(iarg[1]);
    }
if(strcmp(type,"SET_ORIGIN")==0)//NETESTAT
    {
    sscanf(message,"%d%s%d%f%f",&ID,aux,&iarg[0],&farg[0],&farg[1]);
    g[iarg[0]].setOrigin(farg[0],farg[1]);
    }
if(strcmp(type,"SET_ANGLE")==0)//FARA EFECT!
    {
    sscanf(message,"%d%s%d%d%f",&ID,aux,&iarg[0],&iarg[1],&farg[0]);
    g[iarg[0]].getNava(iarg[1]).setAngle(farg[0]);
    }
if(strcmp(type,"NEW_GROUP")==0) numberOfGroups++;
}
else printf("E mesajul meu\n");
}
void addPlayer(int i) {joaca[i-1]=true;}
void setSocket(int sockfd) {socket=sockfd;}
void setHostPlayerID(int ID) {if(ID>0&&ID<5) hostPlayerID=ID; else printf("Valoare invalida pentru hostPlayerID\n");}
int getHostPlayerID() {return hostPlayerID;}
bool isDone() {if (xx>6) return true; return false;}
QColor faceColors[4];
QPoint lastPos;

Tetrahedron(QWidget *parent = 0)
    : QGLWidget(parent)
{
setFormat(QGLFormat(QGL::DoubleBuffer | QGL::DepthBuffer));

rotationX = -21.0;
rotationY = -57.0;
rotationZ = 0.0;
faceColors[0] = Qt::red;
faceColors[1] = Qt::green;
faceColors[2] = Qt::blue;
faceColors[3] = Qt::yellow;
numberOfClicks=0;

int i;
for(i=0;i<4;i++)
    joaca[i]=false;
}
void initializeGL()
{
    int i,j;
    t1=clock();
    t_crestere1=clock();
    numberOfPlanets=10;//max. 17 planete; de ce?
    numberOfShips=6;
    numberOfGroups=200;
if(joaca[0])   p[0].setPlayerID(1);//altfel ramane implicit 0 (daca nu joaca rosu, planeta ramane neutra)
    p[0].setX(-2);
    p[0].setY(-3);
    p[0].setRadius(0.25);
 if(joaca[1])   p[1].setPlayerID(2);
    p[1].setX(-3);
    p[1].setY(2);
    p[1].setRadius(1.75);
  if(joaca[2])  p[2].setPlayerID(3);
    p[2].setX(2);
    p[2].setY(2);
    p[2].setRadius(0.6);
  if(joaca[3])  p[3].setPlayerID(4);
    p[3].setX(2);
    p[3].setY(-4);
    p[3].setRadius(0.7);

    for(i=4;i<numberOfPlanets;i++)
    {
       /* p[i].setPlayerID(i%4+1);
        p[i].setRadius(i*0.05);
        p[i].setX(i-11);
        p[i].setY(0);*///max 17 planete asa

        printf("y");
        p[i].setRadius((float)i*0.075);
        printf("z");
        p[i].setX((float)i*2.2-14+sin(i));
        printf("k");
        p[i].setY((float)i-8+cos(i));//max 10 planete asa
        printf("i=%d\n",i);
    }

    for(i=0;i<200;i++)
    {
        n[i].setPlayerID(hostPlayerID);
        for(j=0;j<numberOfPlanets;j++)
        {
    n[i].addPlanet(p[j]);
        }
    }
    for(i=0;i<numberOfGroups;i++)
    g[i].reset();

    for(j=0;j<numberOfGroups;j++)
    for(i=0;i<numberOfShips;i++)
    g[j].adaugaNava(n[i+j*numberOfShips]);
numberOfGroups=0;//1

    for(i=0;i<numberOfPlanets;i++)
    {
    if(p[i].getPlayerID()==hostPlayerID)
    {
nextOriginX=p[i].getX();
nextOriginY=p[i].getY();
break;
    }
    }

    playerColors[0][0]=0.15;
    playerColors[0][1]=0.15;
    playerColors[0][2]=0.15;

    playerColors[1][0]=0.51;
    playerColors[1][1]=0;
    playerColors[1][2]=0;

    playerColors[2][0]=0;
    playerColors[2][1]=0.21;
    playerColors[2][2]=0;

    playerColors[3][0]=0;
    playerColors[3][1]=0.12;
    playerColors[3][2]=0.41;

    playerColors[4][0]=0.31;
    playerColors[4][1]=0.21;
    playerColors[4][2]=0;

glShadeModel(GL_SMOOTH);
glEnable(GL_DEPTH_TEST);
glDisable(GL_CULL_FACE);

GLfloat mat_specular[] = { 0.25,0.25,0.25, 1.0 };//0.25 peste tot,ultima 1
GLfloat mat_shininess[] = { 2.5 };
GLfloat light_position[] = { 100.0, 100.0, 50.0, 0.0 };

glShadeModel (GL_SMOOTH);
glColorMaterial ( GL_FRONT_AND_BACK, GL_EMISSION ) ;
glEnable ( GL_COLOR_MATERIAL ) ;

glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
glLightfv(GL_LIGHT0, GL_POSITION, light_position);

glEnable(GL_LIGHTING);
glEnable(GL_LIGHT0);
glEnable(GL_DEPTH_TEST);

}



void resizeGL(int width, int height)
{
    wwidth=width;
    wheight=height;
glViewport(0, 0, width, height);
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
GLfloat x = GLfloat(width) / height;
glFrustum(-x, x, -1.0, 1.0, 4.0, 25.0);
glMatrixMode(GL_MODELVIEW);
}

void drawPlanet(GLfloat x,GLfloat y,float radius,int playerID,int population)
{

    glLoadIdentity();
    GLdouble modelMatrix[16];
    glGetDoublev(GL_MODELVIEW_MATRIX,modelMatrix);
    GLdouble projMatrix[16];
    glGetDoublev(GL_PROJECTION_MATRIX,projMatrix);
    GLdouble position[3];
    int viewport[4];
          glGetIntegerv(GL_VIEWPORT,viewport);
    gluProject(x,-y-radius,-20,modelMatrix,projMatrix,viewport,&position[0],&position[1],&position[2]);
    glColor3f(playerColors[playerID][0],playerColors[playerID][1],playerColors[playerID][2]);
    char msj[10]="";
    sprintf(msj,"%d",population);
    renderText(position[0],position[1], QString(msj) );

    glLoadIdentity();
    glTranslatef(x,y,-20);


    GLint slices, stacks;
    GLUquadricObj *quadObj;

    quadObj = gluNewQuadric();
    slices = 40;//nivelul de detaliu
    stacks = 40;

    glEnable(GL_CULL_FACE);

    gluSphere(quadObj, radius, slices, stacks);
    glDisable(GL_CULL_FACE);
}

void drawShip(GLfloat x,GLfloat y,GLfloat angle,int playerID)
{
glLoadIdentity();
glTranslatef(x,y,-20);

glScalef(0.25,0.25,0.25);
glDisable(GL_LIGHTING);
glRotatef(angle-90,0,0,1);

glColor3f(playerColors[playerID][0],playerColors[playerID][1],playerColors[playerID][2]);
glBegin(GL_POLYGON);
glVertex2f(-0.25,0);
glVertex2f(+0,0.1);
glVertex2f(+0.25,0);
glColor3f(0.25,0.25,0.25);//0.5 peste tot
glVertex2f(0,1);
glColor3f(playerColors[playerID][0],playerColors[playerID][1],playerColors[playerID][2]);
glVertex2f(-0.25,0);
glEnd();

glScalef(1.6,1.6,1.6);
glColor3f(playerColors[playerID][0],playerColors[playerID][1],playerColors[playerID][2]);
glBegin(GL_LINE_STRIP);
glVertex2f(-0.25,0);
glVertex2f(+0,0.1);
glVertex2f(+0.25,0);
glColor3f(0.25,0.25,0.25);//0.5 peste tot
glVertex2f(0,1);
glColor3f(playerColors[playerID][0],playerColors[playerID][1],playerColors[playerID][2]);
glVertex2f(-0.25,0);
glEnd();
glEnable(GL_LIGHTING);

}


void draw()
{

int i,j,k;
char msg[512];//nu pot *msg,ca crashuie la sprintf

glMatrixMode(GL_MODELVIEW);

t_crestere2=clock();
if(t_crestere2-t_crestere1>800000)//NU
{
    t_crestere1=t_crestere2;
    for(i=0;i<numberOfPlanets;i++)
        if(p[i].getPlayerID()!=0 && p[i].getPopulation()<p[i].getMaxPopulation())
        p[i].modifyPopulation(+1);
    }
for(i=0;i<numberOfPlanets;i++)//NU
drawPlanet(p[i].getX(),p[i].getY(),p[i].getRadius(),p[i].getPlayerID(),p[i].getPopulation());


for(k=0;k<numberOfGroups;k++)//NU CRED (SPECULEAZA!)
for(i=0;i<numberOfPlanets;i++)
{
    for(j=0;j<numberOfShips;j++)//daca s-a busit de o planeta, diferita de cea din care pornesti, resetam pozitia si actualizam populatia
        if(g[k].getNava(j).impact(&p[i],g[k].getXOrigine(),g[k].getYOrigine()))// && p[i].getX()!=g[k].getXOrigine() && p[i].getY()!=g[k].getYOrigine())
        {

        g[k].setFlag(j,true);//SPECULEAZA!!!
        g[k].resetShip(j,-10000,-10000);//SPECULEAZA!!!
        bool rez=true;
        int jj;
        for(jj=0;jj<numberOfShips;jj++)
            if(g[k].getFlag(jj)==false) rez=false;
        if(rez) g[k].setDest(-1000,-1000,1);

        }


}
int firstPlayerID=p[0].getPlayerID(),castigator=p[0].getPlayerID();//NU
for(i=1;i<numberOfPlanets;i++)
    if(p[i].getPlayerID()!=firstPlayerID && p[i].getPlayerID()!=0) castigator=0;
if(castigator!=0)
{
    char mesaj[50]="";
    sprintf(mesaj,"A castigat jucatorul %d",castigator);
    QMessageBox msgBox;
    msgBox.setText(mesaj);
    msgBox.exec();
    exit(0);
}

glLoadIdentity();
glColor3f(1,1,1);
renderText(10,30,QString("Poti trimite grupuri de maxim 6 nave"));

glLoadIdentity();
glTranslatef(cursorx,cursory,-20);
glBegin(GL_LINES);
glVertex2f(0,-0.1);
glVertex2f(0,0.1);
glVertex2f(-0.1,0);
glVertex2f(0.1,0);
glEnd();


//ERORI BA BOULE
bool temp=false;
int auxNumberOfGroups=numberOfGroups;//altfel,dat fiind faptul ca in aceste cicluri repetitive,cadrul este "inghetat",mouseOver va returna tot timpul true, iar numberOfGroups va creste pana cand da segmentation fault
int newGroup=numberOfGroups;//asta este indexul grupului liber
bool creezGrupNou=true;
int originIndex=0,numberOfSentShips=6;//numarul de nave care va fi trimis


if(!numberOfGroups) auxNumberOfGroups=1;
for(k=0;k<auxNumberOfGroups;k++)
for(i=0;i<numberOfPlanets;i++)
if(p[i].mouseOver(cursorx,cursory)) {


    t2=clock();
    if(t2-t1>175000)//era 175000
    {



        int jj;
        t1=t2;
        for(jj=0;jj<numberOfPlanets;jj++)
            if(p[jj].getX()==nextOriginX && p[jj].getY()==nextOriginY)
                originIndex=jj;//indexul planetei origine
        if(p[originIndex].getPopulation()<numberOfSentShips) numberOfSentShips=p[originIndex].getPopulation();
    if(p[i].getPlayerID()!=hostPlayerID)  //nu este planeta mea=>o atac
    {


         int ii;
        temp=true;
        numberOfClicks=0;
        p[originIndex].modifyPopulation(-numberOfSentShips);//DA: originindex, -numberOfSentShips

        sprintf(msg,"MOD_POP %d %d",originIndex,-numberOfSentShips);
        sendMessage(socket,msg,512);//mesajul + NULL//nu cred ca sizeof


        for(ii=0;ii<numberOfGroups;ii++)
        {
            if(g[ii].grupLiber())
            {
            newGroup=ii;
            creezGrupNou=false;
            break;
            }
        }//daca este un loc liber in vectorul de grupuri,acesta este locul unde "cream" noul grup
        g[newGroup].setDest(p[i].getX(),p[i].getY(),p[i].getRadius());//DA: newGroup,x,y,radius
        sprintf(msg,"SET_DEST %d %f %f %f",newGroup,p[i].getX(),p[i].getY(),p[i].getRadius());
        sendMessage(socket,msg,512);//mesajul + NULL//nu cred ca sizeof

                for(ii=0;ii<numberOfShips;ii++)
                {//pun noul grup in planeta origine
                    g[newGroup].resetShip(ii,nextOriginX,nextOriginY);//DA: newGroup,ii,nextOX,nextOY
                    sprintf(msg,"RESET_SHIP %d %d %f %f",newGroup,ii,nextOriginX,nextOriginY);
                    sendMessage(socket,msg,512);//mesajul + NULL//nu cred ca sizeof

                    g[newGroup].setFlag(ii,false);//DA:newGroup,ii [,false]
                    sprintf(msg,"SET_FLAG_FALSE %d %d",newGroup,ii);
                    sendMessage(socket,msg,512);//mesajul + NULL//nu cred ca sizeof

                }
        g[newGroup].setNumberOfShips(numberOfSentShips);//DA,DAR SPECULEAZA!: newGroup,numberOfSentShips
        sprintf(msg,"SET_NUMBER_OF_SHIPS %d %d",newGroup,numberOfSentShips);
        sendMessage(socket,msg,512);//mesajul + NULL//nu cred ca sizeof

        g[newGroup].setPlayerID(Tetrahedron::getHostPlayerID());//DA:newGroup,hostPlayerID
        sprintf(msg,"SET_GROUP_PLAYERID %d %d",newGroup,Tetrahedron::getHostPlayerID());
        sendMessage(socket,msg,512);//mesajul + NULL//nu cred ca sizeof

        g[newGroup].setOrigin(nextOriginX,nextOriginY);//inutil?//DA: newGroup,nextOX,nextOY
        sprintf(msg,"SET_ORIGIN %d %f %f",newGroup,nextOriginX,nextOriginY);
        sendMessage(socket,msg,512);//mesajul + NULL//nu cred ca sizeof

        int unghi;
        for(ii=0;ii<6;ii++)//DA: newGroup,ii,constanta
        {
        if(nextOriginX>p[i].getX() && nextOriginY>p[i].getY()) unghi=225;//ca sa aiba un unghi de start acceptabil
        if(nextOriginX>p[i].getX() && nextOriginY<p[i].getY()) unghi=135;
        if(nextOriginX<p[i].getX() && nextOriginY>p[i].getY()) unghi=315;
        if(nextOriginX<p[i].getX() && nextOriginY<p[i].getY()) unghi=45;

        g[newGroup].getNava(ii).setAngle(unghi);
        sprintf(msg,"SET_ANGLE %d %d %d",newGroup,ii,unghi);
        sendMessage(socket,msg,512);//mesajul + NULL//nu cred ca sizeof

        }
        if(newGroup<MAXGROUPS && creezGrupNou==true)
        {
            numberOfGroups++;//DA, fa si incrementNumberOfGroups
            sprintf(msg,"NEW_GROUP");
            sendMessage(socket,msg,512);//mesajul + NULL//nu cred ca sizeof
        }
        cursorx=-100;//pentru a nu se interpreta un singur click ca fiind mai multe
        cursory=-100;
    }
    else {//este a mea
       if(numberOfClicks==0 && g[k].getXDest()!=p[i].getX() && g[k].getYDest()!=n[i].getY())  //primul click=> setez planeta asta ca origine pentru grupul care urmeaza sa fie creat
                {//NU
                   nextOriginX=p[i].getX();
                   nextOriginY=p[i].getY();
                   numberOfClicks=1;
                   cursorx=-100;
                   cursory=-100;
               }
       else if(numberOfClicks==1){
                     int ii;
                   temp=true;
                   numberOfClicks=0;
                    p[originIndex].modifyPopulation(-numberOfSentShips);//DA: originIndex,-numberOfSentShips
                    sprintf(msg,"MOD_POP %d %d",originIndex,-numberOfSentShips);
                    sendMessage(socket,msg,512);//mesajul + NULL//nu cred ca sizeof
                   for(ii=0;ii<numberOfGroups;ii++)
                   {
                       if(g[ii].grupLiber())
                       {
                       newGroup=ii;
                       creezGrupNou=false;
                       break;
                       }
                   }//daca este un loc liber in vectorul de grupuri,acesta este locul unde "cream" noul grup
if(p[i].getX()!=nextOriginX && p[i].getY()!=nextOriginY)
                   {
                   g[newGroup].setDest(p[i].getX(),p[i].getY(),p[i].getRadius());//DA:newGroup,x,y,R
                   sprintf(msg,"SET_DEST %d %f %f %f",newGroup,p[i].getX(),p[i].getY(),p[i].getRadius());
                   sendMessage(socket,msg,512);//mesajul + NULL//nu cred ca sizeof
                   }
                           for(ii=0;ii<numberOfShips;ii++)
                           {
                               g[newGroup].resetShip(ii,nextOriginX,nextOriginY);//DA:newGroup,ii,nextOriginX,nextOY
                               sprintf(msg,"RESET_SHIP %d %d %f %f",newGroup,ii,nextOriginX,nextOriginY);
                               sendMessage(socket,msg,512);//mesajul + NULL//nu cred ca sizeof

                               g[newGroup].setFlag(ii,false);//DA:newGroup,ii,[false]
                               sprintf(msg,"SET_FLAG_FALSE %d %d",newGroup,ii);
                               sendMessage(socket,msg,512);//mesajul + NULL//nu cred ca sizeof
                           }
                   g[newGroup].setNumberOfShips(numberOfSentShips);//DA:newGroup,numberOfShips
                   sprintf(msg,"SET_NUMBER_OF_SHIPS %d %d",newGroup,numberOfSentShips);
                   sendMessage(socket,msg,512);//mesajul + NULL//nu cred ca sizeof

                   g[newGroup].setPlayerID(Tetrahedron::getHostPlayerID());//DA:newGroup,ID
                   sprintf(msg,"SET_GROUP_PLAYERID %d %d",newGroup,Tetrahedron::getHostPlayerID());
                   sendMessage(socket,msg,512);//mesajul + NULL//nu cred ca sizeof

                   g[newGroup].setOrigin(nextOriginX,nextOriginY);//inutil?//DA:newGroup,nOx,nOy
                   sprintf(msg,"SET_ORIGIN %d %f %f",newGroup,nextOriginX,nextOriginY);
                   sendMessage(socket,msg,512);//mesajul + NULL//nu cred ca sizeof

                   int unghi;
                   for(ii=0;ii<6;ii++)
                   {//DA:newGroup,ii,constanta
                   if(nextOriginX>p[i].getX() && nextOriginY>p[i].getY()) unghi=225;//ca sa aiba un unghi de start acceptabil
                   if(nextOriginX>p[i].getX() && nextOriginY<p[i].getY()) unghi=135;
                   if(nextOriginX<p[i].getX() && nextOriginY>p[i].getY()) unghi=315;
                   if(nextOriginX<p[i].getX() && nextOriginY<p[i].getY()) unghi=45;
                   g[newGroup].getNava(ii).setAngle(unghi);
                   sprintf(msg,"SET_ANGLE %d %d %d",newGroup,ii,unghi);
                   sendMessage(socket,msg,512);//mesajul + NULL//nu cred ca sizeof
                   }
                   if(newGroup<MAXGROUPS && creezGrupNou==true)
                   {
                       numberOfGroups++;//DA,fa un IncrementNumberOFGRoups()
                       sprintf(msg,"NEW_GROUP");
                       sendMessage(socket,msg,512);//mesajul + NULL//nu cred ca sizeof
                   }
                   cursorx=-100;//pentru a nu se interpreta un singur click ca fiind mai multe//a devenit inutil
                   cursory=-100;

               }//al doilea click consecutiv pe o planeta de a mea; vreau sa imi transfer fortele de pe o planeta pe alta
    }
}
}

for(i=0;i<numberOfGroups;i++)
g[i].miscaGrup();


glColor3f(playerColors[hostPlayerID][0],playerColors[hostPlayerID][1],playerColors[hostPlayerID][2]);
renderText(10,50,QString("Aceasta este culoarea ta"));


for(j=0;j<numberOfGroups;j++)
for(i=0;i<numberOfShips;i++)
drawShip(g[j].getNava(i).getX(),g[j].getNava(i).getY(),g[j].getNava(i).getAngle(),g[j].getNava(i).getPlayerID());

}

void paintGL()
{
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
glClearColor(0,0,0.0,1);
draw();
update();
}

void mousePressEvent(QMouseEvent *event)
{
lastPos = event->pos();

}
void mouseMoveEvent(QMouseEvent *event)
{
GLfloat dx = GLfloat(event->x() - lastPos.x()) / width();
GLfloat dy = GLfloat(event->y() - lastPos.y()) / height();
mousex=event->x();
mousey=event->y();
cursorx=mousex/75-wwidth/150;
cursory=-mousey/75+wheight/150;
if (event->buttons() & Qt::LeftButton) {
rotationX += 180 * dy;
rotationY += 180 * dx;
updateGL();
} else if (event->buttons() & Qt::RightButton) {
rotationX += 180 * dy;
rotationZ += 180 * dx;
updateGL();
}
lastPos = event->pos();
}

};
