/*
*  Paluka-Midpoint_Displacement.cpp
*  Erik Paluka
*  October, 2011
*
*	This program generates a height map using the random midpoint displacement algorithm.
*	I read in a file ("a2-q5-input.txt"), which includes the starting position, step size, 
*	number of samples, and the height values for those samples. Using the starting position and step size, 
*	I create a vertex structure for each sample, and encapsulate in it its own y-coordinate, 
*	x-coordinate and its height (z-coordinate). Then (repeat this for the number of midpoint 
*	algorithm steps given), I iterate through those vertices, and create another vertex in 
*	between each vertex and its neighbour. The height of the new vertex is the sum of the 
*	heights of the neighbouring vertices divided by the number of neighbouring vertices. 
*	I also add a small random value to the height for more randomness. 
*	I then calculate the normal for each vertex by finding the normal of each 
*	triangle face attached to the vertex, and add the face normals together dividing 
*	by the number of face normals. After that, I draw each vertex. 
*/
#include <Windows.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

struct Vertex{
	float x, y, z;
	float nX, nY, nZ;
};

vector<Vertex> vertex;

float eyeX, eyeY, eyeZ, upX, upY, upZ,
	lookX, lookY, lookZ,fov, midSteps, 
	startX, startY, stepX, stepY,
	numX, numY, nearP, farP, nn, mm, 
	onumX, onumY, field_of_view, oeyeX, 
	oeyeY, oeyeZ;

//Finds the normal vector between 3 vertices
struct Vertex findNormal(struct Vertex p1, struct Vertex p2, struct Vertex p3){

	//Find the first vector using p1 and p2
	struct Vertex vector1 = {(p2.x - p1.x),(p2.y - p1.y),(p2.z - p1.z)};         
	//Find the second vector using p1 and p3
	struct Vertex vector2 = {(p3.x - p1.x),(p3.y - p1.y),(p3.z - p1.z)};                
	//Now calculate the normal by taking the cross product of the two vectors
	float xComp = (vector1.y*vector2.z) - (vector1.z*vector2.y);
	float yComp = (vector1.z*vector2.x) - (vector1.x*vector2.z);
	float zComp = (vector1.x*vector2.y) - (vector1.y*vector2.x);
	struct Vertex normal = {xComp, yComp, zComp};

	return normal;
}

void display(void){
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective (field_of_view, 1.0, nearP, farP);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(oeyeX, oeyeY, oeyeZ, lookX, lookY, lookZ, upX, upY, upZ);
	GLfloat ambient[4] = { 0.7, 0.0, 0.4, 1.0 };    // ambient reflection
	GLfloat specular[4] = { 0.7, 0.8, 0.1, 1.0 };   // specular reflection
	GLfloat diffuse[4] = { 0.7, 0.8, 0.1, 1.0 };   // diffuse reflection

	// start by clearing the screen and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// set the ambient reflection for the object
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	// set the diffuse reflection for the object
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
	// set the specular reflection for the object      
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	// set the size of the specular highlights
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 20.0);

	static float r = 0;
	r += 0.5;
	glPushMatrix();
	glRotatef(r, 0, 0, 1);
	glTranslated(-(onumX*stepX)/2, -(onumY*stepY)/2, 0);

	
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Find the normals of the vertices
	for(int i = 0; i < (numY-1); i++){
		for(int j = 0; j < (numX-1); j++){
			if (i == 0 && j == 0){
				Vertex normal = findNormal(vertex.at(i*numX + j), vertex.at((i+1)*numX + j), vertex.at(i*numX + (j+1)));
				vertex.at(i*numX + j).nX = normal.x;
				vertex.at(i*numX + j).nY = normal.y;
				vertex.at(i*numX + j).nZ = normal.z;
			} else if (i == 0){
				Vertex normal1 = findNormal(vertex.at(i*numX + j-1), vertex.at((i+1)*numX + j-1), vertex.at(i*numX + j));
				Vertex normal2 = findNormal(vertex.at((i+1)*numX + j-1), vertex.at((i+1)*numX + j), vertex.at(i*numX + j));
				Vertex normal3 = findNormal(vertex.at(i*numX + j), vertex.at((i+1)*numX + j), vertex.at(i*numX + j+1));

				vertex.at(i*numX + j).nX = (normal1.x + normal2.x + normal3.x)/3;
				vertex.at(i*numX + j).nY = (normal1.y + normal2.y + normal3.y)/3;
				vertex.at(i*numX + j).nZ = (normal1.z + normal2.z + normal3.z)/3;
			} else if (j == 0){
				Vertex normal1 = findNormal(vertex.at((i-1)*numX + j), vertex.at(i*numX + j), vertex.at((i-1)*numX + j+1));
				Vertex normal2 = findNormal(vertex.at((i-1)*numX + j+1), vertex.at(i*numX + j), vertex.at(i*numX + j+1));
				Vertex normal3 = findNormal(vertex.at(i*numX + j), vertex.at((i+1)*numX + j), vertex.at(i*numX + j+1));

				vertex.at(i*numX + j).nX = (normal1.x + normal2.x + normal3.x)/3;
				vertex.at(i*numX + j).nY = (normal1.y + normal2.y + normal3.y)/3;
				vertex.at(i*numX + j).nZ = (normal1.z + normal2.z + normal3.z)/3;
			} else {
				Vertex normal1 = findNormal(vertex.at((i-1)*numX + j), vertex.at(i*numX + j-1), vertex.at(i*numX + j));
				Vertex normal2 = findNormal(vertex.at((i+1)*numX + j-1), vertex.at(i*numX + j-1), vertex.at(i*numX + j));
				Vertex normal3 = findNormal(vertex.at((i+1)*numX + j-1), vertex.at((i+1)*numX + j), vertex.at(i*numX + j));
				Vertex normal4 = findNormal(vertex.at(i*numX + j+1), vertex.at((i+1)*numX + j), vertex.at(i*numX + j));
				Vertex normal5 = findNormal(vertex.at((i-1)*numX + j+1), vertex.at(i*numX + j+1), vertex.at(i*numX + j));
				Vertex normal6 = findNormal(vertex.at((i-1)*numX + j), vertex.at((i-1)*numX + j+1), vertex.at(i*numX + j+1));

				vertex.at(i*numX + j).nX = (normal1.x + normal2.x + normal3.x + normal4.x + normal5.x + normal6.x)/6;
				vertex.at(i*numX + j).nY = (normal1.y + normal2.y + normal3.y + normal4.y + normal5.y + normal6.y)/6;
				vertex.at(i*numX + j).nZ = (normal1.z + normal2.z + normal3.z + normal4.z + normal5.z + normal6.z)/6;
			}
		}
	}


	// Draw the terrain
	glBegin(GL_TRIANGLES);
	for(int i = 0; i < (numY-1); i++){
		for(int j = 0; j < (numX-1); j++){

			int index = (i*numX + j);
			glNormal3f(vertex.at(index).nX, vertex.at(index).nY, vertex.at(index).nZ);
			glVertex3f(vertex.at(index).x, vertex.at(index).y, vertex.at(index).z);

			index = ((i+1)*numX + j);
			glNormal3f(vertex.at(index).nX, vertex.at(index).nY, vertex.at(index).nZ);
			glVertex3f(vertex.at(index).x, vertex.at(index).y, vertex.at(index).z);

			index = (i*numX + (j+1));
			glNormal3f(vertex.at(index).nX, vertex.at(index).nY, vertex.at(index).nZ);
			glVertex3f(vertex.at(index).x, vertex.at(index).y, vertex.at(index).z);

			index = (i*numX + (j+1));
			glNormal3f(vertex.at(index).nX, vertex.at(index).nY, vertex.at(index).nZ);
			glVertex3f(vertex.at(index).x, vertex.at(index).y, vertex.at(index).z);

			index = ((i+1)*numX + (j+1));
			glNormal3f(vertex.at(index).nX, vertex.at(index).nY, vertex.at(index).nZ);
			glVertex3f(vertex.at(index).x, vertex.at(index).y, vertex.at(index).z);

			index = ((i+1)*numX + j);
			glNormal3f(vertex.at(index).nX, vertex.at(index).nY, vertex.at(index).nZ);
			glVertex3f(vertex.at(index).x, vertex.at(index).y, vertex.at(index).z);

		}
	}
	glEnd();
	glPopMatrix();
	glutSwapBuffers();
	glutPostRedisplay();
}


void keyboard(unsigned char key, int x, int y)
{
   switch (key) {
	  case 113:			// q - Exit the program
		 exit(0);
		 break;
	  case 114:			// r - Reset the camera
		  field_of_view = fov;
		  oeyeX = eyeX;
		  oeyeY = eyeY;
		  oeyeZ = eyeZ;
		  glutPostRedisplay();
		  break;
	  case 122:			// z - Reduce the field of view of the camera
		  field_of_view -= 5;
		  glutPostRedisplay();
		  break;
	  case 120:			// x - Increase the field of view of the camera
	      field_of_view += 5;
		  glutPostRedisplay();
		  break;
	case 100:			// z - Reduce the field of view of the camera
		  oeyeX -= 5;
		  glutPostRedisplay();
		  break;
	  case 102:			// x - Increase the field of view of the camera
		  oeyeY -= 5;
		  glutPostRedisplay();
		  break;
	case 104:			// z - Reduce the field of view of the camera
		  oeyeZ -= 5;
		  glutPostRedisplay();
		  break;
	 case 106:			// z - Reduce the field of view of the camera
		  oeyeX += 5;
		  glutPostRedisplay();
		  break;
	  case 108:			// x - Increase the field of view of the camera
		  oeyeY += 5;
		  glutPostRedisplay();
		  break;
	case 110:			// z - Reduce the field of view of the camera
		  oeyeZ += 5;
		  glutPostRedisplay();
		  break;

	  
   }
}

int main(int argc, char** argv)
{

	int in;
	ifstream readFile;
	readFile.open("a2-q5-input.txt");

	if (!readFile){
		cout << "Problem opening file." << endl;
		return 1;
	}
	readFile >> eyeX;
	readFile >> eyeY;
	readFile >> eyeZ;
	readFile >> upX;
	readFile >> upY;
	readFile >> upZ;
	readFile >> lookX;
	readFile >> lookY;
	readFile >> lookZ;
	readFile >> fov;
	readFile >> nearP;
	readFile >> farP;
	readFile >> midSteps;
	readFile >> startX;
	readFile >> stepX;
	readFile >> numX;
	readFile >> startY;
	readFile >> stepY;
	readFile >> numY;

	onumX = numX;
	onumY = numY;
	oeyeX = eyeX;
	oeyeY = eyeY;
	oeyeZ = eyeZ;
	field_of_view = fov;

	for(int i = 0; i < numY; i++){
		for(int j = 0; j < numX; j++){
			Vertex v;
			v.x = startX + (j*stepX);
			v.y = startY + (i*stepY);
			readFile >> v.z;
			vertex.push_back(v);
		}
	}
	readFile.close();
	vector<Vertex> vNew;

	for(int k = 0; k < midSteps; k++){
		int i;
		int j;
		for(i = 0; i < (numY-1); i++){	
			for(j = 0; j < numX-1; j++){
				Vertex v;
				v.x = (vertex.at(i*numX + j).x + vertex.at(i*numX + j + 1).x)/2;
				v.y = (vertex.at(i*numX + j).y + vertex.at(i*numX + j + 1).y)/2;
				v.z = (vertex.at(i*numX + j).z + vertex.at(i*numX + j + 1).z)/2 + rand()%2 - 1;
				vNew.push_back(vertex.at(i*numX + j));
				vNew.push_back(v);
			}
			vNew.push_back(vertex.at(i*numX + j));
			
			for(j = 0; j < numX-1; j++){
				Vertex v;
				v.x = (vertex.at(i*numX + j).x + vertex.at(i*numX + j).x)/2;
				v.y = (vertex.at(i*numX + j).y + vertex.at((i+1)*numX + j).y)/2;
				v.z = (vertex.at(i*numX + j).z + vertex.at((i+1)*numX + j).z)/2 + rand()%2 - 1;
				vNew.push_back(v);
				v.x = (vertex.at(i*numX + j).x + vertex.at(i*numX + j + 1).x)/2;
				v.y = (vertex.at(i*numX + j).y + vertex.at((i+1)*numX + j + 1).y)/2;
				v.z = (vertex.at(i*numX + j).z + vertex.at((i+1)*numX + j + 1).z)/2 + rand()%2 - 1;
				vNew.push_back(v);
			}
			Vertex v;
			v.x = (vertex.at(i*numX + j).x + vertex.at(i*numX + j).x)/2;
			v.y = (vertex.at(i*numX + j).y + vertex.at((i+1)*numX + j).y)/2;
			v.z = (vertex.at(i*numX + j).z + vertex.at((i+1)*numX + j).z)/2 + rand()%2 - 1;
			vNew.push_back(v);
		}
		
		for(j = 0; j < numX-1; j++){
				Vertex v;
				v.x = (vertex.at(i*numX + j).x + vertex.at(i*numX + j + 1).x)/2;
				v.y = (vertex.at(i*numX + j).y + vertex.at(i*numX + j + 1).y)/2;
				v.z = (vertex.at(i*numX + j).z + vertex.at(i*numX + j + 1).z)/2 + rand()%2 - 1;
				vNew.push_back(vertex.at(i*numX + j));
				vNew.push_back(v);
		}
		vNew.push_back(vertex.at(i*numX + j));
		vertex = vNew;
		vNew.clear();
		numX += numX -1;
	    numY += numY -1;
	}

	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize (500, 500); 
	glutInitWindowPosition (100, 100);
	glutCreateWindow ("Erik Paluka - Advanced Computer Graphics - Assignment 2");

	// set the lighting
	glShadeModel(GL_SMOOTH);
	GLfloat lightP[4] = {0.0,0.0, 800.0,1.0};
	glLightfv(GL_LIGHT0, GL_POSITION, lightP);

	// set the ambient light colour    
	GLfloat lightA[4] = {0.9,0.0,0.4,1.0};
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightA);

	// set the specular light colour         
	GLfloat lightS[4] = {0.9,0.3,0.4,1.0};
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightS);

	// set the diffuse light colour
	GLfloat lightD[4] = {0.9,0.9,0.9,1.0};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightD);       

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);  
	glEnable(GL_NORMALIZE);


	/* Use depth buffering for hidden surface elimination. */
	glEnable(GL_DEPTH_TEST);
	

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}