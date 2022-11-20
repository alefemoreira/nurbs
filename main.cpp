#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <iostream>
using namespace std;

GLfloat ctrlpoints[4][4][3] = {
 {{-1.5, 1.0, -1.5}, {-0.5, 1.0,-1.5 }, {0.5, 1.0, -1.5 }, {1.5, 1.0,-1.5}}, 
 {{-1.5, 1.0, -0.5}, {-0.5, 2.0,-0.5 }, {0.5, 2.0, -0.5 }, {1.5, 1.0,-0.5}}, 
 {{-1.5, 1.0,  0.5}, {-0.5, 2.0, 0.5 }, {0.5, 2.0,  0.5 }, {1.5, 1.0, 0.5}}, 
 {{-1.5, 1.0,  1.5}, {-0.5, 1.0, 1.5 }, {0.5, 1.0,  1.5 }, {1.5, 1.0, 1.5}}
};

float preRotX = 0.0;
float preRotY = 0.0;
float rotX = 0;
float rotY = 0;
int positionA = 0;
int positionB = 0;
bool isFirstDown = true;
bool changingPoint = false;
bool isMiddle = false;
int orgX = 0;
int orgY = 0;
float scale = 1;

void display(void)
{
   int i, j;

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glColor3f(1.0, 1.0, 1.0);
   glPushMatrix();
   glRotatef((preRotX + rotX), 0.0f, 1.0f, 0.0f);
   glRotatef((preRotY + rotY), 1.0f, 0.0f, 0.0f);
   //glRotatef(25.0, 1.0, 1.0, 1.0);
   glScalef(scale, scale, scale);

   glColor3f(1.0f, 0.0f, 0.0f);
   glPointSize(5.0f);
   glBegin(GL_POINTS);
     for(i = 0; i < 4; i++)
       for(j = 0; j < 4; j++)
         glVertex3f(ctrlpoints[i][j][0], ctrlpoints[i][j][1], ctrlpoints[i][j][2]);
   glVertex3f(0.0f,0.0f,0.0f);
   glVertex3f(0.0f,1.0f,0.0f);
   glVertex3f(1.0f,0.0f,0.0f);
   glEnd();

   glColor3f(1.0, 1.0, 1.0);
   for (j = 0; j <= 8; j++) {
      glBegin(GL_LINE_STRIP);
      for (i = 0; i <= 30; i++)
         glEvalCoord2f((GLfloat)i/30.0, (GLfloat)j/8.0);
      glEnd();
      glBegin(GL_LINE_STRIP);
      for (i = 0; i <= 30; i++)
         glEvalCoord2f((GLfloat)j/8.0, (GLfloat)i/30.0);
      glEnd();
   }

   glPopMatrix();
   glFlush();
}

void initMap() {
   glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4,
           0, 1, 12, 4, &ctrlpoints[0][0][0]);
   glEnable(GL_MAP2_VERTEX_3);
   glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);
}

void init(void)
{
   glClearColor(0.0, 0.0, 0.0, 0.0);
   initMap();
   glEnable(GL_DEPTH_TEST);
   glShadeModel(GL_FLAT);
}

void reshape(int w, int h)
{
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   if (w <= h)
      glOrtho(-5.0, 5.0, -5.0*(GLfloat)h/(GLfloat)w, 
               5.0*(GLfloat)h/(GLfloat)w, -5.0, 5.0);
   else
      glOrtho(-5.0*(GLfloat)w/(GLfloat)h, 
               5.0*(GLfloat)w/(GLfloat)h, -5.0, 5.0, -5.0, 5.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

void mouse(int button, int state, int x, int y) {
  if (button == 4) {
    if (changingPoint) {
      ctrlpoints[positionA][positionB][2] *= 0.9; 
    } else {
      scale *= 0.9;
    }
    glutPostRedisplay();
  }
  if (button == 3) {
    if (changingPoint) {
      ctrlpoints[positionA][positionB][2] *= 1.1; 
    } else {
      scale *= 1.1;
    }
    glutPostRedisplay();
  }
  if (button == 1  && state == 0) {
    isMiddle = true;

    if (isFirstDown) {
      isFirstDown = false;
      orgX = x;
      orgY = y;
    }
  } 
  if (button == 1 && state == 1) {
    isFirstDown = true;
    isMiddle = false;
    preRotY += rotY;
    preRotX += rotX;
    rotX = 0;
    rotY = 0;
  }
}

//void handlerFuncName(wxMouseEvent& event) {
  //cout << event << '\n';
//}
void mouse_motion(int x, int y) {
  if (isMiddle && !isFirstDown) {
      rotX = x - orgX;
      rotY = y - orgY;
    }
  glutPostRedisplay();
}

void keyboard(GLubyte key, GLint x, GLint y) {
  cout << "key: " << (GLint)key << '\n';
  if ((GLint) key == 48) {
  cout << "y: " << y << '\n';
  cout << "x: " << x << '\n';
  cout << "new x: " << ((GLfloat) x - 250.5f) / 50.0f << '\n';
    ctrlpoints[0][0][0] = ((GLfloat) x - 250.5f) / 50.0f;
    ctrlpoints[0][0][1] = ((GLfloat) y - 250.5f) / (-50.0f);
    changingPoint = true;
    positionA = 0;
    positionB = 0;
    initMap();
    glutPostRedisplay();
  }
  if ((GLint) key == 49) {
  cout << "y: " << y << '\n';
  cout << "x: " << x << '\n';
  cout << "new x: " << ((GLfloat) x - 250.5f) / 50.0f << '\n';
    ctrlpoints[0][1][0] = ((GLfloat) x - 250.5f) / 50.0f;
    ctrlpoints[0][1][1] = ((GLfloat) y - 250.5f) / (-50.0f);
    changingPoint = true;
    positionA = 0;
    positionB = 1;
    initMap();
    glutPostRedisplay();
  }
  if ((GLint) key == 50) {
  cout << "y: " << y << '\n';
  cout << "x: " << x << '\n';
  cout << "new x: " << ((GLfloat) x - 250.5f) / 50.0f << '\n';
    ctrlpoints[0][2][0] = ((GLfloat) x - 250.5f) / 50.0f;
    ctrlpoints[0][2][1] = ((GLfloat) y - 250.5f) / (-50.0f);
    changingPoint = true;
    positionA = 0;
    positionB = 2;
    initMap();
    glutPostRedisplay();
  }
  if ((GLint) key == 51) {
  cout << "y: " << y << '\n';
  cout << "x: " << x << '\n';
  cout << "new x: " << ((GLfloat) x - 250.5f) / 50.0f << '\n';
    ctrlpoints[0][3][0] = ((GLfloat) x - 250.5f) / 50.0f;
    ctrlpoints[0][3][1] = ((GLfloat) y - 250.5f) / (-50.0f);
    changingPoint = true;
    positionA = 0;
    positionB = 3;
    initMap();
    glutPostRedisplay();
  }
  if ((GLint) key == 52) {
  cout << "y: " << y << '\n';
  cout << "x: " << x << '\n';
  cout << "new x: " << ((GLfloat) x - 250.5f) / 50.0f << '\n';
    ctrlpoints[1][0][0] = ((GLfloat) x - 250.5f) / 50.0f;
    ctrlpoints[1][0][1] = ((GLfloat) y - 250.5f) / (-50.0f);
    changingPoint = true;
    positionA = 1;
    positionB = 0;
    initMap();
    glutPostRedisplay();
  }
  if ((GLint) key == 53) {
  cout << "y: " << y << '\n';
  cout << "x: " << x << '\n';
  cout << "new x: " << ((GLfloat) x - 250.5f) / 50.0f << '\n';
    ctrlpoints[1][1][0] = ((GLfloat) x - 250.5f) / 50.0f;
    ctrlpoints[1][1][1] = ((GLfloat) y - 250.5f) / (-50.0f);
    changingPoint = true;
    positionA = 1;
    positionB = 1;
    initMap();
    glutPostRedisplay();
  }
  if ((GLint) key == 54) {
  cout << "y: " << y << '\n';
  cout << "x: " << x << '\n';
  cout << "new x: " << ((GLfloat) x - 250.5f) / 50.0f << '\n';
    ctrlpoints[1][2][0] = ((GLfloat) x - 250.5f) / 50.0f;
    ctrlpoints[1][2][1] = ((GLfloat) y - 250.5f) / (-50.0f);
    changingPoint = true;
    positionA = 1;
    positionB = 2;
    initMap();
    glutPostRedisplay();
  }
  if ((GLint) key == 55) {
  cout << "y: " << y << '\n';
  cout << "x: " << x << '\n';
  cout << "new x: " << ((GLfloat) x - 250.5f) / 50.0f << '\n';
    ctrlpoints[1][3][0] = ((GLfloat) x - 250.5f) / 50.0f;
    ctrlpoints[1][3][1] = ((GLfloat) y - 250.5f) / (-50.0f);
    changingPoint = true;
    positionA = 1;
    positionB = 3;
    initMap();
    glutPostRedisplay();
  }
  if ((GLint) key == 56) {
  cout << "y: " << y << '\n';
  cout << "x: " << x << '\n';
  cout << "new x: " << ((GLfloat) x - 250.5f) / 50.0f << '\n';
    ctrlpoints[2][0][0] = ((GLfloat) x - 250.5f) / 50.0f;
    ctrlpoints[2][0][1] = ((GLfloat) y - 250.5f) / (-50.0f);
    changingPoint = true;
    positionA = 2;
    positionB = 0;
    initMap();
    glutPostRedisplay();
  }
  if ((GLint) key == 57) {
  cout << "y: " << y << '\n';
  cout << "x: " << x << '\n';
  cout << "new x: " << ((GLfloat) x - 250.5f) / 50.0f << '\n';
    ctrlpoints[2][1][0] = ((GLfloat) x - 250.5f) / 50.0f;
    ctrlpoints[2][1][1] = ((GLfloat) y - 250.5f) / (-50.0f);
    changingPoint = true;
    positionA = 2;
    positionB = 1;
    initMap();
    glutPostRedisplay();
  }
  if ((GLint) key == 97) {
  cout << "y: " << y << '\n';
  cout << "x: " << x << '\n';
  cout << "new x: " << ((GLfloat) x - 250.5f) / 50.0f << '\n';
    ctrlpoints[2][2][0] = ((GLfloat) x - 250.5f) / 50.0f;
    ctrlpoints[2][2][1] = ((GLfloat) y - 250.5f) / (-50.0f);
    changingPoint = true;
    positionA = 2;
    positionB = 2;
    initMap();
    glutPostRedisplay();
  }
  if ((GLint) key == 98) {
  cout << "y: " << y << '\n';
  cout << "x: " << x << '\n';
  cout << "new x: " << ((GLfloat) x - 250.5f) / 50.0f << '\n';
    ctrlpoints[2][3][0] = ((GLfloat) x - 250.5f) / 50.0f;
    ctrlpoints[2][3][1] = ((GLfloat) y - 250.5f) / (-50.0f);
    changingPoint = true;
    positionA = 2;
    positionB = 3;
    initMap();
    glutPostRedisplay();
  }
  if ((GLint) key == 99) {
    ctrlpoints[3][0][0] = ((GLfloat) x - 250.5f) / 50.0f;
    ctrlpoints[3][0][1] = ((GLfloat) y - 250.5f) / (-50.0f);
    changingPoint = true;
    positionA = 3;
    positionB = 0;
    initMap();
    glutPostRedisplay();
  }
  if ((GLint) key == 100) {
    ctrlpoints[3][1][0] = ((GLfloat) x - 250.5f) / 50.0f;
    ctrlpoints[3][1][1] = ((GLfloat) y - 250.5f) / (-50.0f);
    changingPoint = true;
    positionA = 3;
    positionB = 1;
    initMap();
    glutPostRedisplay();
  }
  if ((GLint) key == 101) {
    ctrlpoints[3][2][0] = ((GLfloat) x - 250.5f) / 50.0f;
    ctrlpoints[3][2][1] = ((GLfloat) y - 250.5f) / (-50.0f);
    changingPoint = true;
    positionA = 3;
    positionB = 2;
    initMap();
    glutPostRedisplay();
  }
  if ((GLint) key == 102) {
    ctrlpoints[3][3][0] = ((GLfloat) x - 250.5f) / 50.0f;
    ctrlpoints[3][3][1] = ((GLfloat) y - 250.5f) / (-50.0f);
    changingPoint = true;
    positionA = 3;
    positionB = 3;
    initMap();
    glutPostRedisplay();
  }
}

void keyboardUp(GLubyte key, GLint x, GLint y) {
  changingPoint = false;
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100);
   glutCreateWindow(argv[0]);
   glutMouseFunc(mouse);
   glutKeyboardFunc(keyboard);
   glutMotionFunc(mouse_motion);
   glutKeyboardUpFunc(keyboardUp);
   init();
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutMainLoop();
   return 0;
}
