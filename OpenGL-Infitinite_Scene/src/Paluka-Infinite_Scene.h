/*
*  Paluka-Infinite_Scene.h
*
*  Erik Paluka
*  December, 2011
*
*	
*/

//#ifndef paluka
//#define paluka

#include <Windows.h>
//#include <Mmsystem.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <string>
#include <math.h>
#include <time.h>
#include <vector>
#include "Trees.h"
#include "Textures.h"

using namespace std;


const int NUM_FLAKES = 4000;

const int WIDTH_PATH = 10;
const int GROUND_WIDTH_MULT = 30;

//Dimensions of background image
const int B_WIDTH = 4288;
const int B_HEIGHT = 2848;
//Dimensions of path image
const int P_WIDTH = 2560;
const int P_HEIGHT = 1920;
//Dimensions of curb image
const int C_WIDTH = 2560;
const int C_HEIGHT = 1920;
//Dimensions of grass image
const int G_WIDTH = 3648;
const int G_HEIGHT = 2736;
//Dimensions of snow image
const int S_WIDTH = 2816;
const int S_HEIGHT = 2112;
//Dimensions of water image
const int W_WIDTH = 340;
const int W_HEIGHT = 340;
//Dimensions of grassy snow image
const int S2_WIDTH = 340;
const int S2_HEIGHT = 340;


const float FARPLANE = 100000;
const float RADIUS = 10000;
const float RADIUSG = RADIUS + 1.5;
const float THETA_CHANGE = PI/1000.0;
const float MAX_BUMP = 50.0;
const float MIN_BUMP = -20.0;




extern int NUM_TREES;

extern float upY;
extern float upZ;


struct tree{
	float x;
	float y;
	float z;

	float theta;
	unsigned int list;

	float lineWidth;

	// L-System Begginning Axiom
	string str;

	vector<float> rotations_y;
	vector<float> rotations_x;
	vector<float> rotations_z;

}; 	extern tree *trees;


struct Point{
	float x;
	float y;
	float z;
};

struct snowFlake{
	float x;
	float y;
	float z;

	float vY;
	float vZ;

	float sizeX;
	float sizeY;
	float sizeZ;

	int life;
}; extern snowFlake flakes[];


void drawGround(vector<Point> *vertices, unsigned int texture, int u, int v);

void createTexTrees(int j);

void animate();

void resetSnow(int i);

void drawSnow(int i);

void drawBackground(unsigned int texture, int width, int height);

void createGround(float startX, float endX, string side);

void createTreeImage(char* name);

//Creates all the vertices for the path and saves them into a vector
void createPath();

void display(void);

void keyboard(unsigned char key, int x, int y);

void arrows(int key, int x, int y);

int main(int argc, char** argv);

//#endif