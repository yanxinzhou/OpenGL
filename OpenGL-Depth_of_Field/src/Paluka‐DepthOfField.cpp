/*
 *  Erik Paluka
 *  September 2011
 *
 *  This program draws a number of cubes along the y axis on the xyplane and produces depth of field (blur).
 *  It does this by simulating a pin hole camera with an aperture size, and jittering the camera.
 *  q - Exit the program
 *  r - Reset the camera
 *  z - Reduce the field of view of the camera
 *  x - Increase the field of view of the camera
 *  d - Reduce the depth of field (Increase aperture size of the pin hole camera)
 *  f - Increase the depth of field (Decrease aperture size of the pin hole camera)
 */
#include <Windows.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <iostream>
using namespace std;

void simulateDepthOfField();
void changeView(int);

struct COLOUR {
	float red, green, blue;
};

const int NUM_CUBES = 20;
const int JITTER_TIMES = 8;

float field_of_view = 45.0, aperture_size = 0.0;
struct COLOUR colour[NUM_CUBES];


void init(void) 
{
   glClear (GL_COLOR_BUFFER_BIT);
   glClear (GL_ACCUM_BUFFER_BIT);

   for(int i = 0; i < NUM_CUBES; i++){
	   colour[i].red = ((float) (rand() ))/RAND_MAX;
	   colour[i].green = ((float) (rand() ))/RAND_MAX;
	   colour[i].blue = ((float) (rand() ))/RAND_MAX;
   }
}

void display(void){
	glClear (GL_ACCUM_BUFFER_BIT);
	simulateDepthOfField();
	glutSwapBuffers();
}

void simulateDepthOfField(){
	for (int j = 0; j < JITTER_TIMES; j++){
		glClear(GL_COLOR_BUFFER_BIT);
		changeView(j);	// Jitter the pin hole camera

		for(int i = 0; i < NUM_CUBES; i++){ // Draw the cubes
		   glPushMatrix();
		   glColor3f(colour[i].red, colour[i].green, colour[i].blue);
		   glTranslatef(0.0, -(NUM_CUBES + (NUM_CUBES/3)), 0.0);
		   glTranslatef(0.0, 2.0*i, 0.0);
		   glutSolidCube(1.0);
		   glPopMatrix();
	   }
		glAccum(GL_ACCUM, 0.1);
	}
	glAccum(GL_RETURN, 1.0);
}

void changeView(int j){
	float offset = j*aperture_size*0.05;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective (field_of_view, 1.0, 1.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	//Jitter the camera. The amount of jitter depends on the value of 'eye'.
	if (j % 2 == 0){
		gluLookAt(10.0-offset,10.0+offset,10.0,0.0,0.0,0.0,0.0, 1.0, 0.0);
	} else {
		gluLookAt(10.0+offset,10.0-offset,10.0,0.0,0.0,0.0,0.0, 1.0, 0.0);
	}
}

void keyboard(unsigned char key, int x, int y)
{
   switch (key) {
	  case 113:			// q - Exit the program
		 exit(0);
		 break;
	  case 114:			// r - Reset the camera
		  field_of_view = 45;
		  aperture_size = 0.0;
		  glutPostRedisplay();
		  break;
	  case 122:			// z - Reduce the field of view of the camera
		  if (field_of_view > 10){
			  field_of_view -= 5;
			  glutPostRedisplay();
		  }
		  break;
	  case 120:			// x - Increase the field of view of the camera
		  if (field_of_view < 120){
			  field_of_view += 5;
			  glutPostRedisplay();
		  }
		  break;
	  case 100:			// d - Reduce the depth of field
		  aperture_size += 0.25;
		  glutPostRedisplay();
		  break;
	  case 102:			// f - Increase the depth of field
		  if (aperture_size > 0){
			  aperture_size -= 0.25;
			  glutPostRedisplay();
		  }
		  break;
   }
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_ACCUM);
   glutInitWindowSize (500, 500); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow ("Erik Paluka - Advanced Computer Graphics - Assignment 1");
   init();
   glutDisplayFunc(display); 
   glutKeyboardFunc(keyboard);
   glutMainLoop();
   return 0;
}