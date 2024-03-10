#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
using namespace std;
#ifdef MAC
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

// Surface class
const int SIZE = 60;
class Surface {
public:
   float Px[SIZE][SIZE];
   float Py[SIZE][SIZE];
   float Pz[SIZE][SIZE];
   float Nx[SIZE][SIZE];
   float Ny[SIZE][SIZE];
   float Nz[SIZE][SIZE];
};

struct Point {
    float x, y;
};

vector<Point> points;

// Global variables 
const int N = 1000; // Number of points to capture
const int SPEED = 30; // Speed of UFO (higher is slower)
size_t step = 0;
Point flyPath[N+1];
float currUFOX = 0; float currUFOY = 0;
bool isMousePressed = false;
Surface UFO;

void renderLine() {
    if(points.size() > 0){
        glBegin(GL_LINE_STRIP);
        int sampleScale = points.size()/N;
        if(sampleScale > 0){
            for (size_t i = 0; i < N; i++) {
                Point point = points[sampleScale*i];
                glVertex3f(point.x, point.y, 0);
                flyPath[i] = point;
            }
        }else{
            for (size_t i = 0; i < points.size(); i++) {
                Point point = points[i];
                glVertex3f(point.x, point.y, 0);
                flyPath[i] = point;
            }
        }
        glVertex3f(points.back().x, points.back().y, 0);
        glEnd();
    }
}


void surface_normals(Surface &s)
{
   // Find surface normals
   for (int u = 0; u < SIZE-1; u++)
   for (int v = 0; v < SIZE-1; v++)
   {
      // Get two tangent vectors
      float Ux = s.Px[u+1][v] - s.Px[u][v];
      float Uy = s.Py[u+1][v] - s.Py[u][v];
      float Uz = s.Pz[u+1][v] - s.Pz[u][v];
      float Vx = s.Px[u][v+1] - s.Px[u][v];
      float Vy = s.Py[u][v+1] - s.Py[u][v];
      float Vz = s.Pz[u][v+1] - s.Pz[u][v];
      
      // Do cross product
      s.Nx[u][v] = Uy * Vz - Uz * Vy;
      s.Ny[u][v] = Uz * Vx - Ux * Vz;
      s.Nz[u][v] = Ux * Vy - Uy * Vx;
      float length = sqrt( 
         s.Nx[u][v] * s.Nx[u][v] + 
         s.Ny[u][v] * s.Ny[u][v] + 
         s.Nz[u][v] * s.Nz[u][v]);
      if (length > 0)
      {
         s.Nx[u][v] /= length;
         s.Ny[u][v] /= length;
         s.Nz[u][v] /= length;
      }
   } 

   // Wrap around values for last row and col
   for (int u = 0; u < SIZE; u++)
   {
      s.Nx[u][SIZE-1] = s.Nx[u][0];
      s.Ny[u][SIZE-1] = s.Ny[u][0];
      s.Nz[u][SIZE-1] = s.Nz[u][0];
   }
   for (int v = 0; v < SIZE; v++)
   {
      s.Nx[SIZE-1][v] = s.Nx[0][v];
      s.Ny[SIZE-1][v] = s.Ny[0][v];
      s.Nz[SIZE-1][v] = s.Nz[0][v];
   }
}

void init_UFO(Surface &s)
{
   // Initialize UFO surface and normals
   float radius1 = 0.25;
   float objSize = 0.5;
   for (int u = 0; u < SIZE/2; u++)
   {
      float angle1 = 2 * u * M_PI / (SIZE/2 - 1);
      for (int v = 0; v < SIZE/2; v++)
      {
        float angle2 = 2 * v * M_PI / (SIZE/2 - 1);
        s.Px[u][v] = objSize * (2.0 * v / (SIZE/2 - 1.0) - 1.0);
        s.Py[u][v] = objSize * cos(angle1) * (radius1*sin(0.5*angle2));
        s.Pz[u][v] = objSize * sin(angle1) * (radius1*sin(0.5*angle2));
      }
   }
   for (int u = SIZE/2; u < SIZE; u++)
   {
      float angle1 = 2 * u * M_PI / (SIZE/2 - 1);
      for (int v = SIZE/2; v < SIZE; v++)
      {
         float angle2 = v * M_PI / (SIZE/2 - 1);
         s.Px[u][v] = s.Nx[u][v] = objSize * 0.25 * sin(angle2) * cos(angle1);
         s.Py[u][v] = s.Ny[u][v] = objSize * (0.25 * sin(angle2) * sin(angle1) + 0.15);
         s.Pz[u][v] = s.Nz[u][v] = objSize * 0.25 * cos(angle2);
      }
   }
}

void display_surface(Surface &s)
{
   for (int u = 0; u < SIZE-1; u++)
      for (int v = 0; v < SIZE-1; v++)
      {
         glBegin(GL_LINE_LOOP);
         glNormal3f(s.Nx[u][v], s.Ny[u][v], s.Nz[u][v]);
         glVertex3f(s.Px[u][v], s.Py[u][v], s.Pz[u][v]);
         glNormal3f(s.Nx[u + 1][v], s.Ny[u + 1][v], s.Nz[u + 1][v]);
         glVertex3f(s.Px[u + 1][v], s.Py[u + 1][v], s.Pz[u + 1][v]);
         glNormal3f(s.Nx[u + 1][v + 1], s.Ny[u + 1][v + 1], s.Nz[u + 1][v + 1]);
         glVertex3f(s.Px[u + 1][v + 1], s.Py[u + 1][v + 1], s.Pz[u + 1][v + 1]);
         glNormal3f(s.Nx[u][v + 1], s.Ny[u][v + 1], s.Nz[u][v + 1]);
         glVertex3f(s.Px[u][v + 1], s.Py[u][v + 1], s.Pz[u][v + 1]);
         glEnd();
      }
}

void display_surface(Surface &s, float scale, float dx, float dy, float dz)
{
   glPushMatrix();
   glTranslatef(dx, dy, dz);
   glScalef(scale, scale, scale);
   display_surface(s);
   glPopMatrix();
}

void init()
{
   // Initialize OpenGL
   glClearColor(0.0, 0.0, 0.0, 1.0);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-2.0, 2.0, -2.0, 2.0, -2.0, 2.0);
   glEnable(GL_DEPTH_TEST);
}

void keyboard(unsigned char key, int x, int y)
{
   if (key == 'q')
      exit(0);
}

void move(Point start, Point end, float t)
{
    float dx = end.x - start.x;
    float dy = end.y - start.y;
    currUFOX = start.x + t * dx;
    currUFOY = start.y + t * dy;
    glutPostRedisplay();
}

void animateTimer(int value)
{
    if (step < (points.size() > N ? N : points.size() - 1)) {
        move(points[step], points[step + 1], 1.0f);
        step++;
        glutTimerFunc(16, animateTimer, 0);
    }
}

void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON)
    {
        if (state == GLUT_DOWN)
        {
            isMousePressed = true;
            points.clear();
            // Normalizes the points by considering their scale compared to window dimensions and then repositioning to ortho
            float normalizedX = 4.0f*(x / static_cast<float>(glutGet(GLUT_WINDOW_WIDTH))) - 2.0f;
            float normalizedY = 2.0f - 4.0f*(y / static_cast<float>(glutGet(GLUT_WINDOW_HEIGHT)));
            Point firstPoint = {normalizedX, normalizedY};
            currUFOX = normalizedX; currUFOY = normalizedY;
            points.push_back(firstPoint);
        }
        else if (state == GLUT_UP)
        {
            isMousePressed = false;
            // Add the last point and render the line
            float normalizedX = 4.0f*(x / static_cast<float>(glutGet(GLUT_WINDOW_WIDTH))) - 2.0f;
            float normalizedY = 2.0f - 4.0f*(y / static_cast<float>(glutGet(GLUT_WINDOW_HEIGHT)));
            Point lastPoint = {normalizedX, normalizedY};
            points.push_back(lastPoint);
            init_UFO(UFO);
            glutPostRedisplay();
            step = 0;
            glutTimerFunc(16, animateTimer, 0);
        }
    }
}

void mouseMotion(int x, int y)
{
    if (isMousePressed)
    {
        // Store points during mouse motion
        float normalizedX = 4.0f*(x / static_cast<float>(glutGet(GLUT_WINDOW_WIDTH))) - 2.0f;
        float normalizedY = 2.0f - 4.0f*(y / static_cast<float>(glutGet(GLUT_WINDOW_HEIGHT)));
        Point newPoint = {normalizedX, normalizedY};
        points.push_back(newPoint);
        glutPostRedisplay();
    }
}

void display()
{
   // Incrementally rotate objects
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   glColor3f(1.0, 1.0, 1.0);
   renderLine();
   display_surface(UFO, 0.5, currUFOX, currUFOY, -1);

   glFlush();
}

int main(int argc, char *argv[])
{
   glutInit(&argc, argv);
   glutInitWindowSize(1000, 1000);
   glutInitWindowPosition(0, 0);
   glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH);
   glutCreateWindow("Object");
   glutDisplayFunc(display);
   glutKeyboardFunc(keyboard);
   glutMouseFunc(mouse);
   glutMotionFunc(mouseMotion);
   init();
   glutMainLoop();
   return 0;
}