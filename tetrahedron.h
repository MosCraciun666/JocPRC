#include<QtOpenGL>
#include<QtGui>



class Tetrahedron : public QGLWidget
{
Q_OBJECT
public:
 GLfloat playerColors[5][3];
 GLfloat xx;
GLfloat rotationX;
GLfloat rotationY;
GLfloat rotationZ;
GLuint texture;
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
}
/*
GLuint LoadTexture(char *filename)
{
        GLuint texID;

        AUX_RGBImageRec *image = auxDIBImageLoad(filename);

        glGenTextures(1, &texID);
        glBindTexture(GL_TEXTURE_2D, texID);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        gluBuild2DMipmaps(GL_TEXTURE_2D, 3, image->sizeX, image->sizeY, GL_RGB, GL_UNSIGNED_BYTE, image->data);

        return texID;
}*/


void initializeGL()
{
    xx=-3;
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
glShadeModel(GL_FLAT);
glEnable(GL_DEPTH_TEST);
glDisable(GL_CULL_FACE);

GLfloat mat_specular[] = { 0.25,0.25, 0.25, 1.0 };
GLfloat mat_shininess[] = { 25.0 };
GLfloat light_position[] = { 100.0, 100.0, 1.0, 0.0 };
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

glDisable(GL_TEXTURE_2D);

//texture=LoadTexture("planet_texture/bmp");
//int j=0;
//texture=bindTexture(QPixmap(QString("planet_texture.bmp").arg(j + 1)), GL_TEXTURE_2D);
}

void resizeGL(int width, int height)
{
glViewport(0, 0, width, height);
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
GLfloat x = GLfloat(width) / height;
glFrustum(-x, x, -1.0, 1.0, 4.0, 15.0);
glMatrixMode(GL_MODELVIEW);
}

void drawPlanet(GLfloat x,GLfloat y,float radius,int playerID)
{
    glLoadIdentity();
    glTranslatef(x+xx,y,-10);

    GLint slices, stacks;
    GLUquadricObj *quadObj;

    quadObj = gluNewQuadric();
    slices = 40;//nivelul de detaliu
    stacks = 40;


    glColor3f(playerColors[playerID][0],playerColors[playerID][1],playerColors[playerID][2]);
    gluSphere(quadObj, radius, slices, stacks);
}

void draw()
{
    xx+=0.01;
glMatrixMode(GL_MODELVIEW);
glLoadIdentity();
glTranslatef(0.0, 0.0, -10.0);
glRotatef(rotationX, 1.0, 0.0, 0.0);
glRotatef(rotationY, 0.0, 1.0, 0.0);
glRotatef(rotationZ, 0.0, 0.0, 1.0);

drawPlanet(1,2,0.21,0);
drawPlanet(-1,-2,0.25,1);
drawPlanet(-1,2,0.21,2);
drawPlanet(1,-2,0.25,3);
drawPlanet(0,0,0.25,4);

}

void paintGL()
{
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
