#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef MAC
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <time.h>
#include <iostream>
using namespace std;

const int MAXFIREWORKLINES = 200;
const int MAXFIREWORKS = 10; //DO NOT CHANGE
long unsigned int currNumFireworks = 0;


struct fireworkObj{
   float r, g, b, x, y, z;
};

struct fireworkLineObj{
   fireworkObj parent;
   float endx, endy, endz;
};

fireworkLineObj fireworkArr[MAXFIREWORKS*MAXFIREWORKLINES];

void init(){
   glClearColor(0.0, 0.0, 0.0, 1.0);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-10.0, 10.0, -10.0, 10.0, -10.0, 10.0);
}

void fireworkLine(fireworkObj& parent, float endx, float endy, float endz){
   //generates new instance of fireworkLineObj from parent
   fireworkLineObj newFireworkLine = {parent, endx, endy, endz};
   if(currNumFireworks <= MAXFIREWORKS*MAXFIREWORKLINES){
      fireworkArr[currNumFireworks] = newFireworkLine;
      currNumFireworks++;
      glBegin(GL_LINES);
      glColor3f(parent.r, parent.g, parent.b);
      glVertex3f(parent.x, parent.y, parent.z);
      glVertex3f(parent.x + endx, parent.y + endy, parent.z + endz);
      glEnd();
   }
}

void fireworkLine(fireworkLineObj& instance){
   //generates preallocated instance of fireworkLineObj
   fireworkObj parent = instance.parent;
   glBegin(GL_LINES);
   glColor3f(parent.r, parent.g, parent.b);
   glVertex3f(parent.x, parent.y, parent.z);
   glVertex3f(parent.x + instance.endx, parent.y + instance.endy, parent.z + instance.endz);
   glEnd();
}

void firework(float r, float g, float b, float x, float y, float z){
   //creates firework parent from which all children inherit color and starting point from
   fireworkObj newFirework = {r, g, b, x, y, z};
   long unsigned int numOfLines = random()%6 + MAXFIREWORKLINES - 5;
   for(size_t i = 0; i < numOfLines; i++){
      fireworkLine(newFirework, (random()%2000 - 1000)/200.0f, (random()%2000 - 1000)/200.0f, (random()%2000 - 1000)/200.0f);
   }
}

void display(){
   //determine if fireworks have already been drawn; if so, reinstantiate upon rerender, otherwise, generate random new instances
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   if(currNumFireworks < 1){
      long unsigned int numOfFireworks = random()%6 + MAXFIREWORKS - 5;
      for(size_t i = 0; i < numOfFireworks; i++){
         firework((random()%100)/100.0f, (random()%100)/100.0f, (random()%100)/100.0f, (random()%2000 - 1000)/200.0f, (random()%2000 - 1000)/200.0f, (random()%2000 - 1000)/200.0f);
      }
   }else{
      for(size_t i = 0; i < currNumFireworks; i++){
         fireworkLine(fireworkArr[i]);
      }
   }
   glFlush();
}

int main(int argc, char *argv[])
{
   //randomize results and set up window
   srand(time(NULL));
   glutInit(&argc, argv);
   glutInitWindowSize(800, 800);
   glutInitWindowPosition(0, 0);
   glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
   glutCreateWindow("Fireworks");
   glutDisplayFunc(display);
   init();
   glutMainLoop();
   return 0;
}