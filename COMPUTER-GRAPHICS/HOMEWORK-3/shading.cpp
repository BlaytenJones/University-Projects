#ifdef MAC
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

//---------------------------------------
// Program: shading.cpp
// Purpose: Functions for Phong shading
// Author:  John Gauch
// Date:    March 2013
//---------------------------------------

// Material properties
float Ka = 0.2764;
float Kd = 0.3618;
float Ks = 0.3618;
float Kp = 0.09;

//---------------------------------------
// Initialize material properties
//---------------------------------------
void init_material(float Ka, float Kd, float Ks, float Kp,
                   float Mr, float Mg, float Mb)
{
   // Material variables
   float ambient[] = { Ka * Mr, Ka * Mg, Ka * Mb, 1.0 };
   float diffuse[] = { Kd * Mr, Kd * Mg, Kd * Mb, 1.0 };
   float specular[] = { Ks * Mr, Ks * Mg, Ks * Mb, 1.0 };

   // Initialize material
   glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
   glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
   glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, Kp);
}

//---------------------------------------
// Initialize light source
//---------------------------------------
void init_light(int light_source, float Lx, float Ly, float Lz,
                float Lr, float Lg, float Lb)
{
   // Light variables
   float light_position[] = { Lx, Ly, Lz, 0.0 };
   float light_color[] = { Lr, Lg, Lb, 1.0 };

   // Initialize light source
   glEnable(GL_LIGHTING);
   glEnable(light_source);
   glLightfv(light_source, GL_POSITION, light_position);
   glLightfv(light_source, GL_AMBIENT, light_color);
   glLightfv(light_source, GL_DIFFUSE, light_color);
   glLightfv(light_source, GL_SPECULAR, light_color);
   glLightf(light_source, GL_CONSTANT_ATTENUATION, 1.0);
   glLightf(light_source, GL_LINEAR_ATTENUATION, 0.0);
   glLightf(light_source, GL_QUADRATIC_ATTENUATION, 0.0);
   glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);
   glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
}