#include<QtOpenGL>
#include<QtGui>
//poti renunta la nava.groupID?????????
class planeta
{
private:
    float x,y,radius;
    int playerID;
public:
    planeta(float x=0,float y=0, float radius=0.25,float playerID=0)
    {
this->x=x;
this->y=y;
this->radius=radius;
this->playerID=playerID;
    }
    void setX(float x) {this->x=x;}
    void setY(float y) {this->y=y;}
    void setRadius(float r) {radius=r;}
    void setPlayerID(int pid) {playerID=pid;}
    float getX() {return x;}
    float getY() {return y;}
    float getRadius() {return radius;}
    int getPlayerID() {return playerID;}
};


class nava
{
private :
float x,y,angle;
float xd,yd,angled;
int playerID,numberOfPlanets,groupID;
planeta lp[20];//lista cu planete care trebuiesc adaugate
void avoidPlanet(planeta pl)
{
float xnext,ynext,dist,x2,y2;//x2 e inutil, x2 si y2 sunt coordonatele centrului planetei din punctul de vedere al navei
float i;
for(i=0;i<2;i+=0.2)
{
xnext=x+i*cos(angle*3.14/180);
ynext=y+i*sin(angle*3.14/180);
/*
glLoadIdentity();
glTranslatef(0,0,-20);
glColor3f(1,1,0);
glBegin(GL_POINTS);
glVertex2f(xnext,ynext);
glEnd();
*/
dist=pow(xnext-pl.getX(),2)+pow(ynext-pl.getY(),2);//ERA X-p1,getX() BA BOULE
dist=sqrt(dist);
//printf("(%f,%f) (%f,%f) r=%f dist=%f\n",x,y,pl.getX(),pl.getY(),pl.getRadius(),dist);
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
 //   printf("(%f,%f) dist:f,angle:%f\n",x2,y2,angle); IMPORTANT!!
    }
}
}

public:
nava(float x=0,float y=0,float angle=0,int playerID=0,int gid=0)
{
this->x=x;
this->y=y;
this->angle=angle;
this->playerID=playerID;
groupID=gid;
xd=0;yd=0;angled=0;
numberOfPlanets=0;

}
void addPlanet(planeta p) {lp[numberOfPlanets]=p;numberOfPlanets++;}
void move(float xdest,float ydest)
{
    int i;
angled=atan((ydest-y)/(xdest-x));
angled=angled*180/3.14;//din radiani in grade
if(xdest-x<0) angled+=180;
for(i=0;i<numberOfPlanets;i++)
avoidPlanet(lp[i]);

while(angled>=360) angled-=360;
while(angled<0) angled+=360;
while(angle>=360) angle=angle-360;
while(angle<0) angle+=360;
if(angle<angled) {if(abs(angle-angled)<180)angle+=2;//poate si astea ar trebui sa depinda de speed
    else angle-=2;}
if(angle>angled) {if(abs(angle-angled)<180) angle-=2;
    else angle+=2;}
while(angle>=360) angle=angle-360;
while(angle<0) angle+=360;
float speed=0.1;//o sa o masuram cand avem un schelet bun in JocPRC.cpp

x+=speed*cos(angle*3.14/180);
y+=speed*sin(angle*3.14/180);



}
float getX() {return x;}
float getY() {return y;}
float getAngle() {return angle;}
void setPlayerID(int ID) {playerID=ID;}
int getPlayerID() {return playerID;}
void setGroupID(int gid) {groupID=gid;}
};








class grupDeNave
{
private:
nava n[10];
int nrNave;
public:
grupDeNave() {nrNave=0;}
void reset() {nrNave=0;}//bullshit doar ca sa rezolv o eroare
void adaugaNava(nava nav)
    {
    if(nrNave<10) {
        n[nrNave]=nav;
        n[nrNave].setGroupID(nrNave);
        nrNave++;
    }
    else printf("eroare,prea multe nave in grup\n");
    }
void miscaGrup(float x,float y)
{
int i;
printf("misc grupul: Nrnave=%d => ",nrNave);
for(i=0;i<nrNave;i++)
    {
    float x2,y2;//auxiliare pt navele care tre sa fie mai "pe margine"
    float d=i;//corectie pt  mai tarziu: d=f(i,R)
    if(i%2==1) d=-d;
    x2=x+d*sin(n[i].getAngle()*3.14/180);
    y2=y+d*cos(n[i].getAngle()*3.14/180);
n[i].move(x2,y2);
printf("%d:(%f,%f);",i,n[i].getX(),n[i].getY());
    }
printf("\n");
}
nava getNava(int index) {return n[index];}
};










class Tetrahedron : public QGLWidget
{
Q_OBJECT
private:
 GLfloat playerColors[5][3];
 GLfloat xx,mousex,mousey,cursorx,cursory;
GLfloat rotationX;
GLfloat rotationY;
GLfloat rotationZ;
GLuint texture;
planeta p,p2;
nava n,n2,n3;
grupDeNave g;

float wwidth,wheight;
    public:
planeta getPlaneta() {return p;}
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
p.setPlayerID(1);
p.setX(-2);
p.setY(-3);
p.setRadius(0.5);
p2.setPlayerID(2);
p2.setX(2);
p2.setY(2);
p2.setRadius(0.75);
}


void initializeGL()
{

    n.addPlanet(p);
    n.addPlanet(p2);
    n2.addPlanet(p);
    n2.addPlanet(p2);
    n3.addPlanet(p);
    n3.addPlanet(p2);
    g.reset();
    g.adaugaNava(n);
    g.adaugaNava(n2);
    g.adaugaNava(n3);
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
    playerColors[3][1]=0;
    playerColors[3][2]=0.51;

    playerColors[4][0]=0.31;
    playerColors[4][1]=0.21;
    playerColors[4][2]=0;

qglClearColor(Qt::black);
glShadeModel(GL_SMOOTH);
glEnable(GL_DEPTH_TEST);
glDisable(GL_CULL_FACE);

GLfloat mat_specular[] = { 0.25,0.25, 0.25, 1.0 };
GLfloat mat_shininess[] = { 2.5 };
GLfloat light_position[] = { 100.0, 100.0, 50.0, 0.0 };
glClearColor (0.0, 0.0, 0.0, 0.0);
glShadeModel (GL_SMOOTH);
glColorMaterial ( GL_FRONT_AND_BACK, GL_EMISSION ) ;
glEnable ( GL_COLOR_MATERIAL ) ;

glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
glLightfv(GL_LIGHT0, GL_POSITION, light_position);

glEnable(GL_LIGHTING);
glEnable(GL_LIGHT0);
glEnable(GL_DEPTH_TEST);
/*
glEnable(GL_TEXTURE_2D);
glGenTextures(1, &texture);
glBindTexture(GL_TEXTURE_2D, texture);
glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, pixmap_height, pixmap_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (void*)(&(xim->data[0])));
*/

//texture=LoadTexture("planet_texture/bmp");
/*int j=0;
texture=bindTexture(QPixmap("side1.png"), GL_TEXTURE_2D);*/
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

void drawPlanet(GLfloat x,GLfloat y,float radius,int playerID)
{
    glLoadIdentity();
    glTranslatef(x,y,-20);

    GLint slices, stacks;
    GLUquadricObj *quadObj;

    quadObj = gluNewQuadric();
    slices = 40;//nivelul de detaliu
    stacks = 40;


    glColor3f(playerColors[playerID][0],playerColors[playerID][1],playerColors[playerID][2]);
    gluSphere(quadObj, radius, slices, stacks);
}

void drawShip(GLfloat x,GLfloat y,GLfloat angle,int playerID)
{
glLoadIdentity();
glTranslatef(x,y,-20);

glScalef(0.25,0.25,0.25);
glDisable(GL_LIGHTING);
glRotatef(angle-90,0,0,1);

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

glMatrixMode(GL_MODELVIEW);
glLoadIdentity();
glTranslatef(0.0, 0.0, -20.0);
glRotatef(rotationX, 1.0, 0.0, 0.0);
glRotatef(rotationY, 0.0, 1.0, 0.0);
glRotatef(rotationZ, 0.0, 0.0, 1.0);


drawPlanet(p.getX(),p.getY(),p.getRadius(),p.getPlayerID());
drawPlanet(p2.getX(),p2.getY(),p2.getRadius(),p2.getPlayerID());

glLoadIdentity();
glTranslatef(cursorx,cursory,-20);
glBegin(GL_LINES);
glVertex2f(0,-0.1);
glVertex2f(0,0.1);
glVertex2f(-0.1,0);
glVertex2f(0.1,0);
glEnd();
/*
n.move(cursorx,cursory);
n2.move(cursorx+1,cursory+1);*/
g.miscaGrup(cursorx,cursory);
n2.setPlayerID(1);
n3.setPlayerID(2);
int i;
for(i=0;i<3;i++)
drawShip(g.getNava(i).getX(),g.getNava(i).getY(),g.getNava(i).getAngle(),g.getNava(i).getPlayerID());

}

void paintGL()
{
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
glClearColor(0,0,0,1);
draw();
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
cursorx=mousex/100-wwidth/150;
cursory=-mousey/100+wheight/150;
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


void mouseDoubleClickEvent(QMouseEvent *event)
{
int face = faceAtPosition(event->pos());
if (face != -1) {
QColor color = QColorDialog::getColor(faceColors[face], this);
if (color.isValid()) {
faceColors[face] = color;
updateGL();
}
}
}

int faceAtPosition(const QPoint &pos)
{
const int MaxSize = 512;
GLuint buffer[MaxSize];
GLint viewport[4];
glGetIntegerv(GL_VIEWPORT, viewport);
glSelectBuffer(MaxSize, buffer);
glRenderMode(GL_SELECT);
glInitNames();
glPushName(0);
glMatrixMode(GL_PROJECTION);
glPushMatrix();
glLoadIdentity();
gluPickMatrix(GLdouble(pos.x()), GLdouble(viewport[3] - pos.y()),
5.0, 5.0, viewport);
GLfloat x = GLfloat(width()) / height();
glFrustum(-x, x, -1.0, 1.0, 4.0, 15.0);
draw();
glMatrixMode(GL_PROJECTION);
glPopMatrix();
if (!glRenderMode(GL_RENDER))
return -1;
return buffer[3];
}

};



