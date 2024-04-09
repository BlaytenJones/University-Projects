#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include "shading.cpp"
using namespace std;

vector<float> depthVals = {};
struct color{unsigned int R, G, B;};
vector<color> colors = {};
color avgColor = {};
unsigned char displayMode = '1';
int x_angle = 25, y_angle = 25, z_angle = -90;
unsigned char angleInc = 5;
float modelXSize = 500, modelYSize = 500;
const float worldSize = 2.0;
const float offset = -worldSize/4.0;

struct norm{float nx, ny, nz = ((1.0/modelXSize)/modelYSize);}; //note that the nz is constant for all vectors since the x and y coordinates are evenly distributed as "one unit"
vector<norm> normalVals = {};

void wireframe_display(){
    unsigned char wireInc = 5; //How many points should we skip on the wireframe?
    cout << "\nWireframe Display Active\n";
    glColor3ub(avgColor.R, avgColor.G, avgColor.B);
    for(size_t i = 0; i < modelXSize; i += wireInc){
        glBegin(GL_LINE_LOOP);
        for(size_t j = 0; j < modelYSize; j += wireInc){
            glVertex3f(i/modelXSize + offset, j/modelYSize + offset, depthVals[i*modelXSize + j]);
        }
        glEnd();
    }
}

void color_display(){
    cout << "\nColor Display Active\n";
    for(size_t i = 0; i < modelXSize - 1; i++){
        for(size_t j = 0; j < modelYSize - 1; j++){
            glBegin(GL_TRIANGLES);
            //We change the color at each vertex to create an average color effect on each polygon
            color currColor = colors[i*modelXSize + j];
            glColor3ub(currColor.R, currColor.G, currColor.B);
            glVertex3f(i/modelXSize + offset, j/modelYSize + offset, depthVals[i*modelXSize + j]); //P0
            //save these two points for the second triangle! Saves time
            float tmpX1 = (i+1)/modelXSize + offset, tmpY1 = j/modelYSize + offset, tmpZ1 = depthVals[(i+1)*modelXSize + j]; //P1
            float tmpX2 = i/modelXSize + offset, tmpY2 = (j+1)/modelYSize + offset, tmpZ2 = depthVals[i*modelXSize + j + 1]; //P2
            currColor = colors[(i+1)*modelXSize + j];
            glColor3ub(currColor.R, currColor.G, currColor.B);
            glVertex3f(tmpX1, tmpY1, tmpZ1);
            currColor = colors[i*modelXSize + j + 1];
            glColor3ub(currColor.R, currColor.G, currColor.B);
            glVertex3f(tmpX2, tmpY2, tmpZ2); //triangle P0, P1, P2
            //repeat for the other triangle
            currColor = colors[(i+1)*modelXSize + j];
            glColor3ub(currColor.R, currColor.G, currColor.B);
            glVertex3f(tmpX1, tmpY1, tmpZ1);
            currColor = colors[(i+1)*modelXSize + j + 1];
            glColor3ub(currColor.R, currColor.G, currColor.B);
            glVertex3f((i+1)/modelXSize + offset, (j+1)/modelYSize + offset, depthVals[(i+1)*modelXSize + j + 1]); //P3
            currColor = colors[i*modelXSize + j + 1];
            glColor3ub(currColor.R, currColor.G, currColor.B);
            glVertex3f(tmpX2, tmpY2, tmpZ2); //triangle P1, P3, P2
            glEnd();
        }
    }
}

void phong_mono_display(){
    cout << "\nPhong (Monochromatic) Display Active\n";
    init_material(Ka, Kd, Ks, 100 * Kp, avgColor.R/255.0, avgColor.G/255.0, avgColor.B/255.0);
    norm nTmp;
    for(size_t i = 0; i < modelXSize - 1; i++){
        for(size_t j = 0; j < modelYSize - 1; j++){
            glBegin(GL_TRIANGLES);
            nTmp = normalVals[i*modelXSize + j];
            glNormal3f(nTmp.nx, nTmp.ny, nTmp.nz);
            glVertex3f(i/modelXSize + offset, j/modelYSize + offset, depthVals[i*modelXSize + j]); //P0
            //save these two points for the second triangle! Saves time
            float tmpX1 = (i+1)/modelXSize + offset, tmpY1 = j/modelYSize + offset, tmpZ1 = depthVals[(i+1)*modelXSize + j]; //P1
            float tmpX2 = i/modelXSize + offset, tmpY2 = (j+1)/modelYSize + offset, tmpZ2 = depthVals[i*modelXSize + j + 1]; //P2
            glVertex3f(tmpX1, tmpY1, tmpZ1);
            glVertex3f(tmpX2, tmpY2, tmpZ2); //triangle P0, P1, P2
            //repeat for the other triangle
            glVertex3f(tmpX1, tmpY1, tmpZ1);
            glVertex3f((i+1)/modelXSize + offset, (j+1)/modelYSize + offset, depthVals[(i+1)*modelXSize + j + 1]); //P3
            glVertex3f(tmpX2, tmpY2, tmpZ2); //triangle P1, P3, P2
            glEnd();
        }
    }
}

void phong_full_display(){
    cout << "\nPhong (Full) Display Active\n";
    norm nTmp;
    for(size_t i = 0; i < modelXSize - 1; i++){
        for(size_t j = 0; j < modelYSize - 1; j++){
            color currColor = colors[i*modelXSize + j];
            init_material(Ka, Kd, Ks, 100 * Kp, currColor.R/255.0, currColor.G/255.0, currColor.B/255.0);
            glBegin(GL_TRIANGLES);
            nTmp = normalVals[i*modelXSize + j];
            glNormal3f(nTmp.nx, nTmp.ny, nTmp.nz);
            //save these two points for the second triangle! Saves time
            glVertex3f(i/modelXSize + offset, j/modelYSize + offset, depthVals[i*modelXSize + j]); //P0
            float tmpX1 = (i+1)/modelXSize + offset, tmpY1 = j/modelYSize + offset, tmpZ1 = depthVals[(i+1)*modelXSize + j]; //P1
            float tmpX2 = i/modelXSize + offset, tmpY2 = (j+1)/modelYSize + offset, tmpZ2 = depthVals[i*modelXSize + j + 1]; //P2
            glVertex3f(tmpX1, tmpY1, tmpZ1);
            glVertex3f(tmpX2, tmpY2, tmpZ2); //triangle P0, P1, P2
            //repeat for the other triangle
            currColor = colors[(i+1)*modelXSize + j + 1];
            init_material(Ka, Kd, Ks, 100 * Kp, currColor.R/255.0, currColor.G/255.0, currColor.B/255.0);
            glVertex3f(tmpX1, tmpY1, tmpZ1);
            glVertex3f((i+1)/modelXSize + offset, (j+1)/modelYSize + offset, depthVals[(i+1)*modelXSize + j + 1]); //P3
            glVertex3f(tmpX2, tmpY2, tmpZ2); //triangle P1, P3, P2
            glEnd();
        }
    }
}

bool readColorAndDepth(const string& colorName, const string& depthName){
    ifstream colorFile; colorFile.open(colorName); color cTmp;
    unsigned int colorCounter = 0;
    while(colorFile){
        //Parse colorFile for color triplet tuple and push it back into color vector
        try{
            colorFile >> cTmp.R >> cTmp.G >> cTmp.B;
            if((cTmp.R != 255) || (cTmp.B != 255) || (cTmp.G != 255)){
                avgColor.R += cTmp.R; avgColor.B += cTmp.B; avgColor.G += cTmp.G;
                colorCounter++;
            }
            colors.push_back(cTmp);
        }catch(const runtime_error& e){
            //Happens if there is not a triplet or uneven number of color pairs
            cerr << "Bad color file!\n";
            colorFile.close();
            return false;
        }
    }
    avgColor.R /= colorCounter; avgColor.G /= colorCounter; avgColor.B /= colorCounter;
    colorFile.close();
    float depthRatio = 2550; //How much should the depth range be scaled down?
    ifstream depthFile; depthFile.open(depthName); unsigned short dTmp;
    while(depthFile){
        depthFile >> dTmp;
        depthVals.push_back(dTmp/depthRatio);
    }
    depthFile.close();
    norm nTmp;
    for(size_t i = 0; i < modelXSize; i++){
        for(size_t j = 0; j < modelYSize; j++){
            //V1.x = 1.0/modelXSize; (as it moves one unit)
            //V1.y = 0; (as the y value does not change)
            //V1.z = (depthVals[(i+1)*modelXSize + j] - depthVals[i*modelXSize + j]);
            //V2.x = 0;
            //V2.y = 1.0/modelYSize;
            //V2.z = (depthVals[i*modelXSize + j + 1] - depthVals[i*modelXSize + j]);
            //normal.x = V1.y * V2.z - V1.z * V2.y = 0 - ((depthVals[(i+1)*modelXSize + j] - depthVals[i*modelXSize + j]))*(1.0/modelYSize);
            //normal.y = V2.x * V1.z - V2.z * V1.x = 0 - ((depthVals[i*modelXSize + j + 1] - depthVals[i*modelXSize + j]))*(1.0/modelXSize);
            //normal.z = V1.x * V2.y - V1.y * V2.x = 1.0/modelXSize*1.0/modelYSize - 0
            float currDepth = depthVals[i*modelXSize + j]; 
            nTmp.nx = -((depthVals[(i+1)*modelXSize + j] - currDepth))/modelYSize;
            nTmp.ny = -((depthVals[i*modelXSize + j + 1] - currDepth))/modelXSize;
            normalVals.push_back(nTmp);
        }
    }
    return true;
}

void init(float scale){
   // Initialize OpenGL
   glClearColor(0.0, 0.0, 0.0, 1.0);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-worldSize, worldSize, -worldSize, worldSize, -worldSize/2.0*3.0, worldSize/2.0*3.0);
   glEnable(GL_DEPTH_TEST);
   glShadeModel(GL_SMOOTH);
   glEnable(GL_NORMALIZE);
   init_light(GL_LIGHT0, 0, 1, 1, 0.5, 0.5, 0.5);
   init_light(GL_LIGHT1, 0, 0, 1, 0.5, 0.5, 0.5);
   init_light(GL_LIGHT2, 0, 1, 0, 0.5, 0.5, 0.5);
   glScalef(scale, scale, scale);
}

void keyboard(unsigned char key, int x, int y){
    switch(key){
        case 'q':
            exit(0);
            break;
        //Change the current display mode to the given key
        case '1':
        case '2':
        case '3':
        case '4':
            displayMode = key;
            break;
        //Rotate object
        case 'x':
            x_angle += angleInc;
            break;
        case 'X':
            x_angle -= angleInc;
            break;
        case 'y':
            y_angle += angleInc;
            break;
        case 'Y':
            y_angle -= angleInc;
            break;
        case 'z':
            z_angle += angleInc;
            break;
        case 'Z':
            z_angle -= angleInc;
            break;
        default:
            break;
    }
    glutPostRedisplay();
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef(x_angle, 1.0, 0.0, 0.0);
    glRotatef(y_angle, 0.0, 1.0, 0.0);
    glRotatef(z_angle, 0.0, 0.0, 1.0);
    switch(displayMode){
        case '1':
            //PHONG SHADING MESSES WITH WIREFRAME; DISABLE
            glDisable(GL_LIGHTING);
            wireframe_display();
            break;
        case '2':
            //PHONG SHADING MESSES WITH COLOR MODEL; DISABLE
            glDisable(GL_LIGHTING);
            color_display();
            break;
        case '3':
            //ENABLE LIGHTING MODE FOR PHONG SHADING
            glEnable(GL_LIGHTING);
            phong_mono_display();
            break;
        case '4':
            //ENABLE LIGHTING MODE FOR PHONG SHADING
            glEnable(GL_LIGHTING);
            phong_full_display();
            break;
        default:
            cerr << "\nNo display associated with this number!\n";
            break;
    }
    glFlush();
}

int main(int argc, char *argv[]){
    const unsigned int screenSizeX = 1000, screenSizeY = screenSizeX;
    float scale = 2.5;
    string imageFile = "penny-image.txt"; string depthFile = "penny-depth.txt";
    cout << "Welcome to RGBD Model generator!\n"
         << "If you would like to use non-test files, use the format ./compile.sh imagefile.txt depthfile.txt\n"
         << "Press 'q' if you would like to quit the program.\n";
    cout << "COMMANDS:\nq) QUIT PROGRAM\n1) WIREFRAME MODEL DISPLAY\n2) COLOR MODEL DISPLAY\n3) MONOCHROMATIC PHONG MODEL DISPLAY\n"
         << "4) FULL PHONG MODEL DISPLAY\nx) INCREASE X ANGLE BY 5\nX) DECREASE X ANGLE BY 5\ny) INCREASE Y ANGLE BY 5\n"
         << "Y) DECREASE Y ANGLE BY 5\nz) INCREASE Z ANGLE BY 5\nZ) DECREASE Z ANGLE BY 5\n";

    //If the user has custom files, they are assigned here
    if(argc != 1){
        if(argc != 3){
            cerr << "Incorrect number of arguments. Use the format ./compile.sh imagefile.txt depthfile.txt\nClosing program; try again.\n";
            return 1;
        }else{
            imageFile = argv[1]; depthFile = argv[2];
        }
    }
    cout << "Using " << imageFile << " and " << depthFile << "\n";
    if(!readColorAndDepth(imageFile, depthFile)){
        cerr << "Files are incorrectly formatted. Please provide files with given color and depth values. Closing program; try again.\n";
        return 1;
    }
    //DO NOT PIPE COMMANDLINE ARGUMENTS INTO GLUTINIT!
    int THROWAWAY = 1; char* THROWAWAY2[1] = {const_cast<char*>(" ")};
    glutInit(&THROWAWAY, THROWAWAY2);
    glutInitWindowSize(screenSizeX, screenSizeY);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH);
    glutCreateWindow("Depth Modeling");
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    init(scale);
    glutMainLoop();
    return 0;
}