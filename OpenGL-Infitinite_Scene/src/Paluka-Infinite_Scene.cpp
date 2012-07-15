/*
*  Paluka-Infinite_Scene.cpp
*	
*  Moving through a Scene
*
*  Erik Paluka
*  December, 2011
*
*	
*/

#include "Paluka-Infinite_Scene.h"

//True for curve in path, false for straight path
bool FLAG_CURVE = true; 
//True for trees made from textures, false for procedurally generated trees
//Change TREE_HEIGHT and GROWTH and NUM_TREES if FLAG_TEX_TREES is set to false!
bool FLAG_TEX_TREES = true; 

tree *trees;

int NUM_TREES = 0;
snowFlake flakes[NUM_FLAKES];

bool flag = false;
bool createdTreeImages = false;
bool randRot = false;

int imageA;
int imgTreeW = 500;
int imgTreeH = 600;

float fieldOfView = 45;
float theta = 0;
float step = 0.0;

float eyeOffset = RADIUS/800;

float eyeX = 0;
float eyeY = (RADIUS+eyeOffset)*cos(theta);
float eyeZ = (RADIUS+eyeOffset)*sin(theta);

float lookOffset = RADIUS/RADIUS;

float lookX = 0;
float lookY = (RADIUS+lookOffset)*cos(theta+THETA_CHANGE);
float lookZ = (RADIUS+lookOffset)*sin(theta+THETA_CHANGE);

float upY = cos(theta);
float upZ = sin(theta);

float wU2 = 12.0;
float wV2 = 3.0;
float wU = 0.0;
float wV = 0.0;

vector<Point> *leftGroundV = new vector<Point>();
vector<Point> *rightGroundV = new vector<Point>();
vector<Point> *pathV = new vector<Point>();
vector<Point> *leftCurbV = new vector<Point>();
vector<Point> *rightCurbV = new vector<Point>();
vector<Point> *waterV = new vector<Point>();

//Used for the textures
unsigned int background;
unsigned int path;
unsigned int curb;
//unsigned int grass;
//unsigned int snow;
unsigned int water;
unsigned int snow2;

unsigned int tree0;
unsigned int tree1;
unsigned int tree2;
unsigned int tree3;
unsigned int tree4;
unsigned int tree5;
unsigned int tree6;




void drawGround(vector<Point> *vertices, unsigned int texture, float u2, float v2, float u, float v){
	glPushAttrib(GL_LIGHTING_BIT);//saves current lighting stuff
	if(texture == snow2){


		GLfloat ambient[4] = {1.0, 1.0, 1.0, 0.1};    // ambient reflection
		GLfloat specular[4] = {1.0, 1.0, 1.0, 1.0};   // specular reflection
		GLfloat diffuse[4] = {1.0, 1.0, 1.0, 1.0};   // diffuse reflection


		// set the ambient reflection for the object
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
		// set the diffuse reflection for the object
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
		// set the specular reflection for the object
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);

	}

	glBindTexture(GL_TEXTURE_2D, texture); 
	glBegin(GL_QUADS);
	int i;

	for(i = 0; i <= vertices->size()-2; i = i+2){
		if (i == 0){
			glTexCoord2d(u, v); 
			glVertex3f(vertices->at(i).x, vertices->at(i).y, vertices->at(i).z);

			glTexCoord2d(u2, v); 
			glVertex3f(vertices->at(i+1).x, vertices->at(i+1).y, vertices->at(i+1).z);


			glTexCoord2d(u2, v2); 
			glVertex3f(vertices->at(i+2).x, vertices->at(i+2).y, vertices->at(i+2).z);

			glTexCoord2d(u, v2); 
			glVertex3f(vertices->at(i+3).x, vertices->at(i+3).y, vertices->at(i+3).z);
			i++;
		} else {


			glTexCoord2d(u, v); 
			glVertex3f(vertices->at(i).x, vertices->at(i).y, vertices->at(i).z);

			glTexCoord2d(u2, v); 
			glVertex3f(vertices->at(i-1).x, vertices->at(i-1).y, vertices->at(i-1).z);


			glTexCoord2d(u2, v2); 
			glVertex3f(vertices->at(i+1).x, vertices->at(i+1).y, vertices->at(i+1).z);

			glTexCoord2d(u, v2); 
			glVertex3f(vertices->at(i+2).x, vertices->at(i+2).y, vertices->at(i+2).z);
		}


	}
	glTexCoord2d(0, 0); 
	glVertex3f(vertices->at(i).x, vertices->at(i).y, vertices->at(i).z);

	glTexCoord2d(1, 0); 
	glVertex3f(vertices->at(i-1).x, vertices->at(i-1).y, vertices->at(i-1).z);

	glTexCoord2d(1, 0.01); 
	glVertex3f(vertices->at(1).x, vertices->at(1).y, vertices->at(1).z);

	glTexCoord2d(0, 0.01); 
	glVertex3f(vertices->at(0).x, vertices->at(0).y, vertices->at(0).z);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, NULL);
	glPopAttrib();

}


void resetSnow(int i){
	flakes[i].x = rand()*(WIDTH_PATH*GROUND_WIDTH_MULT*2.0)/RAND_MAX - WIDTH_PATH*GROUND_WIDTH_MULT;
	flakes[i].y = rand()*(500.0)/RAND_MAX - 250;
	flakes[i].z = rand()*(500.0)/RAND_MAX - 250;


	flakes[i].vY = -(rand()*(upY*2)/RAND_MAX + 0.1);
	flakes[i].vZ = -(rand()*(upZ*2)/RAND_MAX + 0.1);


	flakes[i].sizeX = rand()*(0.3)/RAND_MAX+0.1;
	flakes[i].sizeY = rand()*(0.3)/RAND_MAX+0.1;
	flakes[i].sizeZ = rand()*(0.3)/RAND_MAX+0.1;

	flakes[i].life = (rand()*2000)/RAND_MAX + 1000;


}

void drawSnow(int i){

	//float y1 = (RADIUS-1)*cos(theta);
	//float y2 = (RADIUS + 500)*cos(theta);

	//float z1 = (RADIUS-1)*sin(theta);
	//float z2 = (RADIUS + 500)*sin(theta);



	//float y = RADIUS*cos(theta+THETA_CHANGE*4) + flakes[i].y;
	//float z = RADIUS*sin(theta+THETA_CHANGE*4) + flakes[i].z;

	//float r = sqrt(y*y + z*z);

	//if ((flakes[i].life <= 0)
	//	|| (r < RADIUS)){
	//		resetSnow(i);
	//}

	if ((flakes[i].life <= 0) 
		|| (flakes[i].y <= -200) || (flakes[i].y >= 200)
		|| (flakes[i].z <= -200) || (flakes[i].z >= 200)){
			resetSnow(i);
	}



	glPushAttrib(GL_LIGHTING_BIT);//saves current lighting stuff

	GLfloat ambient[4] = {1.0, 1.0, 1.0, 0.3};    // ambient reflection
	GLfloat specular[4] = {1.0, 1.0, 1.0, 1.0};   // specular reflection
	GLfloat diffuse[4] = {1.0, 1.0, 1.0, 1.0};   // diffuse reflection


	// set the ambient reflection for the object
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	// set the diffuse reflection for the object
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
	// set the specular reflection for the object
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);

	glTranslatef(flakes[i].x, flakes[i].y, flakes[i].z);

	glBindTexture(GL_TEXTURE_2D, snow2); 
	glBegin(GL_TRIANGLES);

	glTexCoord2d(0,0);
	glVertex3f(0, 0, 0);

	glTexCoord2d(1,1);
	glVertex3f(0, flakes[i].sizeY, 0.1);

	glTexCoord2d(1, 0);
	glVertex3f(flakes[i].sizeX, 0.1, flakes[i].sizeZ);

	glEnd();
	glBindTexture(GL_TEXTURE_2D, NULL); 

	flakes[i].y += flakes[i].vY;
	flakes[i].z += flakes[i].vZ;
	flakes[i].life--;

	glPopAttrib();

}
void drawBackground(unsigned int texture, int width, int height){
	//Draw the back image
	glBindTexture(GL_TEXTURE_2D, texture);
	float y1 = (RADIUS + 1)*cos(theta + THETA_CHANGE * 100);
	float y2 = (RADIUS + height/5)*cos(theta + THETA_CHANGE * 100);

	float z1 = (RADIUS + 1)*sin(theta + THETA_CHANGE * 100);
	float z2 = (RADIUS + height/5)*sin(theta + THETA_CHANGE * 100);

	glBegin( GL_QUADS );
	glTexCoord2d(0, 1); 
	glVertex3f(-width/10, y1, z1);
	glTexCoord2d(0, 0); 
	glVertex3f(-width/10, y2, z2);
	glTexCoord2d(1, 0); 
	glVertex3f(width/10, y2, z2);
	glTexCoord2d(1, 1); 
	glVertex3f(width/10, y1, z1);
	glEnd();

	// Draw the right image

	float y3 = (RADIUS + 1)*cos(theta - THETA_CHANGE * 100);
	float y4 = (RADIUS + height/5)*cos(theta - THETA_CHANGE * 100);

	float z3 = (RADIUS + 1)*sin(theta - THETA_CHANGE * 100);
	float z4 = (RADIUS + height/5)*sin(theta - THETA_CHANGE * 100);

	glBegin( GL_QUADS );
	glTexCoord2d(0, 1); 
	glVertex3f(-width/10, y1, z1);
	glTexCoord2d(0, 0); 
	glVertex3f(-width/10, y2, z2);
	glTexCoord2d(1, 0); 
	glVertex3f(-width/10, y4, z4);
	glTexCoord2d(1, 1); 
	glVertex3f(-width/10, y3, z3);
	glEnd();


	// Draw the left image

	glBegin( GL_QUADS );
	glTexCoord2d(1, 1); 
	glVertex3f(width/10, y1, z1);
	glTexCoord2d(1, 0); 
	glVertex3f(width/10, y2, z2);
	glTexCoord2d(0, 0); 
	glVertex3f(width/10, y4, z4);
	glTexCoord2d(0, 1); 
	glVertex3f(width/10, y3, z3);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, NULL);

}




void createGround(float startX, float endX, string side){
	float newRadiusG = RADIUSG;
	float t;
	float curve = 0;
	bool flag1 = false;

	for(t = 0.0; t < PI*2.0; t = t + THETA_CHANGE){
		if (t == 0.0){
			if (side.compare("right") == 0){
				Point p = {-startX, RADIUSG*cos(t), RADIUSG*sin(t)};
				rightGroundV->push_back(p);
				Point p2 = {-startX*endX, newRadiusG*cos(t), newRadiusG*sin(t)};
				rightGroundV->push_back(p2);
			} else if (side.compare("left") == 0){
				Point p = {startX*endX, newRadiusG*cos(t), newRadiusG*sin(t)};
				leftGroundV->push_back(p);
				Point p2 = {startX, RADIUSG*cos(t), RADIUSG*sin(t)};
				leftGroundV->push_back(p2);
			}else if (side.compare("water") == 0){
				Point p = {startX*endX, newRadiusG*cos(t), newRadiusG*sin(t)};
				waterV->push_back(p);
				Point p2 = {startX, RADIUSG*cos(t), RADIUSG*sin(t)};
				waterV->push_back(p2);
			}
		} else { 
			float rNum = 0;

			if (side.compare("right") == 0){
				rNum = (float) rand()*MAX_BUMP/RAND_MAX;
			} else if ((side.compare("left") == 0) && (((int)(t*1000)) % 4 == 0)){
				rNum = -4;
			} else if ((side.compare("left") == 0) && (((int)(t*1000)) % 4 != 0)) {
				rNum = 4;
			}else if ((side.compare("water") == 0) && (((int)(t*1000)) % 4 == 0)){
				rNum = -4;
			} else if ((side.compare("water") == 0) && (((int)(t*1000)) % 4 != 0)) {
				rNum = 4;
			}

			newRadiusG = RADIUSG + rNum;

			if (t >= PI*2.0-THETA_CHANGE){
				curve = 0;
			}

			if (side.compare("right") == 0){
				Point p3 = {-startX*endX, newRadiusG*cos(t), newRadiusG*sin(t)};
				rightGroundV->push_back(p3);
				Point p4 = {-startX-curve, RADIUSG*cos(t), RADIUSG*sin(t)};
				rightGroundV->push_back(p4);

			} else if (side.compare("left") == 0){
				Point p3 = {startX-curve, RADIUSG*cos(t), RADIUSG*sin(t)};
				leftGroundV->push_back(p3);
				Point p4 = {startX*endX, newRadiusG*cos(t), newRadiusG*sin(t)};
				leftGroundV->push_back(p4);

			} else if (side.compare("water") == 0){
				Point p3 = {startX, newRadiusG*cos(t), newRadiusG*sin(t)};
				waterV->push_back(p3);
				Point p4 = {startX*endX, RADIUSG*cos(t), RADIUSG*sin(t)};
				waterV->push_back(p4);
			}
		}





		//Create the vertices for the curb
		if (t == 0.0){
			if (side.compare("right") == 0){
				Point p = {-WIDTH_PATH, RADIUS*cos(t), RADIUS*sin(t)};
				rightCurbV->push_back(p);
				Point p2 = {-WIDTH_PATH-0.001, RADIUSG*cos(t), RADIUSG*sin(t)};
				rightCurbV->push_back(p2);
			} else if (side.compare("left") == 0){
				Point p = {WIDTH_PATH+0.001, RADIUSG*cos(t), RADIUSG*sin(t)};
				leftCurbV->push_back(p);
				Point p2 = {WIDTH_PATH, RADIUS*cos(t), RADIUS*sin(t)};
				leftCurbV->push_back(p2);
			}
		} else {

			if (side.compare("right") == 0){
				Point p3 = {-WIDTH_PATH-curve-0.001, RADIUSG*cos(t), RADIUSG*sin(t)};
				rightCurbV->push_back(p3);
				Point p4 = {-WIDTH_PATH-curve, RADIUS*cos(t), RADIUS*sin(t)};
				rightCurbV->push_back(p4);

			} else if (side.compare("left") == 0){
				Point p3 = {WIDTH_PATH-curve, RADIUS*cos(t), RADIUS*sin(t)};
				leftCurbV->push_back(p3);
				Point p4 = {WIDTH_PATH-curve+0.001, RADIUSG*cos(t), RADIUSG*sin(t)};
				leftCurbV->push_back(p4);

			}
		}
		if(FLAG_CURVE){
			if (!flag1){
				curve = curve + 0.5;

			} else {
				curve = curve -0.5;
			}

			if (curve <= -10){
				flag1 = false;
			}else if (curve > 10){
				flag1 = true;
			}
		}
	}
}

/**
*	Creates all the vertices for the path and saves them into a vector
*
*
*/
void createPath(){
	float curve = 0;
	float t;
	bool flag1 = false;
	for(t = 0.0; t < PI*2.0; t = t + THETA_CHANGE){
		if (t == 0.0){
			Point p = {WIDTH_PATH, RADIUS*cos(t), RADIUS*sin(t)};
			pathV->push_back(p);
			Point p2 = {-WIDTH_PATH, RADIUS*cos(t), RADIUS*sin(t)};
			pathV->push_back(p2);

		} else if (t >= PI*2.0-THETA_CHANGE){


			Point p3 = {-WIDTH_PATH, RADIUS*cos(t), RADIUS*sin(t)};
			pathV->push_back(p3);
			Point p4 = {WIDTH_PATH, RADIUS*cos(t), RADIUS*sin(t)};
			pathV->push_back(p4);





		} else {


			Point p3 = {-WIDTH_PATH-curve, RADIUS*cos(t), RADIUS*sin(t)};
			pathV->push_back(p3);
			Point p4 = {WIDTH_PATH-curve, RADIUS*cos(t), RADIUS*sin(t)};
			pathV->push_back(p4);





		}
		if (FLAG_CURVE){
			if (!flag1){
				curve = curve + 0.5;

			} else {
				curve = curve -0.5;
			}

			if (curve <= -10){
				flag1 = false;
			}else if (curve > 10){
				flag1 = true;
			}
		}
	}
}

void createTreeImage(char* name){
	gluPerspective (fieldOfView, 1.0, 1.0, FARPLANE);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eyeX, eyeY, eyeZ, 0, 0 , 100, 0, 0, 1);

	trees[0].x = 0;
	trees[0].y = 0;
	trees[0].z = 20;
	trees[0].theta = 0;
	trees[0].lineWidth = 20;
	
	glPushMatrix();
	glRotatef(imageA, 0, 0, 1);
	createCTree(0, trees[0].str);
	drawCTree(0, trees[0].str);
	glPopMatrix();

	readBackBuffer(name, 250, 200, imgTreeW, imgTreeH);
}

void createTexTrees(int j){
			
			

			float v1 = eyeX-trees[j].x;
			float v2 = eyeY-trees[j].y;
			float v3 = eyeZ-trees[j].z;

			double m = sqrt(v1*v1 + v2*v2 + v3*v3);
			v1 = v1/m;
			v2 = v2/m;
			v3 = v3/m;

			float m2 = sqrt(lookX*lookX + lookY*lookY + lookZ*lookZ);
			float lx = lookX/m2;
			float ly = lookY/m2;
			float lz = lookZ/m2;
		
		double ang = acos(abs(v1*lx + v2*ly + v3*lz));
		glEnable(GL_BLEND);
		glDepthMask(false);

		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		if (ang >= (PI/2 - 1*PI/180)){
					glBindTexture(GL_TEXTURE_2D, tree0);
		} else if (ang >= (PI/2 - 2*PI/180)){
					glBindTexture(GL_TEXTURE_2D, tree1);
		} else if (ang >= PI/2 - 3*PI/180){
					glBindTexture(GL_TEXTURE_2D, tree2);
		} else if (ang >= PI/2 - 4*PI/180){
					glBindTexture(GL_TEXTURE_2D, tree3);
		} else if (ang >= PI/2 - 5*PI/180){
			glBindTexture(GL_TEXTURE_2D, tree4);
		} else if (ang >= PI/2 - 6*PI/180){
			glBindTexture(GL_TEXTURE_2D, tree5);
		} else {
			glBindTexture(GL_TEXTURE_2D, tree6);
		}
		
		float y2 = (RADIUS+20)*cos(trees[j].theta);
		float z2 = (RADIUS+20)*sin(trees[j].theta);

		glBegin(GL_QUADS);
		glTexCoord2d(0,0);
		glVertex3f(trees[j].x-10, trees[j].y, trees[j].z);

		glTexCoord2d(1,0);
		glVertex3f(trees[j].x+10, trees[j].y, trees[j].z);

		glTexCoord2d(1,1);
		glVertex3f(trees[j].x+10, y2, z2);

		glTexCoord2d(0,1);
		glVertex3f(trees[j].x-10, y2, z2);

		glEnd();
		glBindTexture(GL_TEXTURE_2D, NULL);
		glDisable(GL_BLEND);
		glDepthMask(true);
			
}

void animate(){
	if (createdTreeImages || !FLAG_TEX_TREES){
		theta += step;
		eyeY = (RADIUS + eyeOffset)*cos(theta);
		eyeZ = (RADIUS + eyeOffset)*sin(theta);
		lookY = (RADIUS + lookOffset)*cos(theta + THETA_CHANGE*3);
		lookZ = (RADIUS + lookOffset)*sin(theta + THETA_CHANGE*3);
		upY = cos(theta);
		upZ = sin(theta);
	}

}

void display(void){
	// start by clearing the screen and depth buffer

	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (createdTreeImages || !FLAG_TEX_TREES){
		gluPerspective (fieldOfView, 1.0, 10.0, FARPLANE);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(eyeX, eyeY, eyeZ, lookX, lookY, lookZ, 0, upY, upZ);

		if (flag){
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		} else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		glPushMatrix();
		drawBackground(background, B_WIDTH*10, B_HEIGHT*10);
		glPopMatrix();


		glPushMatrix();
		drawGround(rightGroundV, snow2, 12, 3, 0, 0); //(u,v) = (10, 10) for grass
		drawGround(rightCurbV, curb, 1, 1, 0, 0);
		drawGround(pathV, path, 1, 1, 0, 0);
		drawGround(leftCurbV, curb, 1, 1, 0, 0);		
		drawGround(leftGroundV, snow2, 12, 3, 0, 0); //(u,v) = (10, 10) for grass


		// Used to make the texture of the water move
		wV = wV + 0.01;
		wU = wU + 0.01;
		wV2 = wV2 + 0.01;
		wU2 = wU2 + 0.01;

		drawGround(waterV, water, wU2, wV2, wU, wV);
		for(int i = 1; i < NUM_TREES; i++){
			if((trees[i].y > (eyeY - RADIUS/4)) && (trees[i].y < (eyeY + RADIUS/4))
				&& (trees[i].z > (eyeZ - RADIUS/4)) && (trees[i].z < (eyeZ + RADIUS/4))){
					glPushMatrix();
					if(FLAG_TEX_TREES){
						createTexTrees(i);
					} else {
						if(i % 2 == 0){
							drawTree1(i);
						}else{
							drawTree2(i);
						}
					}
					glPopMatrix();
			}
		}

		for(int i = 0; i < NUM_FLAKES; i++){
			glPushMatrix();
			glTranslatef(0, RADIUS*cos(theta+THETA_CHANGE*2), RADIUS*sin(theta+THETA_CHANGE*2)); 
			glRotatef(60.0, 0.0, 0.0, 1.0);
			drawSnow(i);
			glPopMatrix();
		} 


	} else if (!createdTreeImages && FLAG_TEX_TREES) {
		eyeZ = 0;
		eyeX = 330;
		eyeY = 500;
		int count = 0;
		createTree2(0);
		printf("Rendering a Tree, Reading the Back Buffer, and Creating Textures... ");
		for(int i = 0; i <= 180; i = i + 30){
			char name[10];

			sprintf(name, "%d.raw", count++);
			imageA = i;
			createTreeImage(name);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
		printf("Done!\n");
		printf("Loading Textures... ");
		tree0 = loadTextureRAW("0.raw", imgTreeW, imgTreeH, 4);
		tree1 = loadTextureRAW("1.raw", imgTreeW, imgTreeH, 4);
		tree2 = loadTextureRAW("2.raw", imgTreeW, imgTreeH, 4);
		tree3 = loadTextureRAW("3.raw", imgTreeW, imgTreeH, 4);
		tree4 = loadTextureRAW("4.raw", imgTreeW, imgTreeH, 4);
		tree5 = loadTextureRAW("5.raw", imgTreeW, imgTreeH, 4);
		tree6 = loadTextureRAW("6.raw", imgTreeW, imgTreeH, 4);
		printf("Done!\n");
		createdTreeImages = true;
		randRot = true;
		eyeX = 0;


	} 
	glutSwapBuffers();
	glutPostRedisplay();

}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 113:			// q - Exit the program
		exit(0);
		break;
	case 119:			// w - Reset the camera
		fieldOfView = 45;
		eyeX = 0;
		step = 0;
		theta = 0;
		glutPostRedisplay();
		break;
	case 122:			// z - Reduce the field of view of the camera
		if(fieldOfView > 10){
		fieldOfView -= 5;
		glutPostRedisplay();
		}
		break;
	case 120:			// x - Increase the field of view of the camera
		if(fieldOfView < 55){
		fieldOfView += 5;
		glutPostRedisplay();
		}
		break;
	case 97:			// a - Increase camera X-coordinate
		if(eyeX < 300){
			eyeX = eyeX + 5;
			glutPostRedisplay();
		}
		break;
	case 115:			// s - Decrease camera X-coordinate
		if(eyeX > -40){
			eyeX = eyeX - 5;
			glutPostRedisplay();
		}
		break; 
	case 118:			// v - Increase theta step
		if(step < 0.005){
			step += 0.0005;
			glutPostRedisplay();
		}
		break;
	case 99:			// c - Decrease theta step
		if(step > -0.005){
			step -= 0.0005;
			glutPostRedisplay();
		}
		break; 
	case 102:			// f - Decrease camera X-coordinate by 200 each time
		eyeX -= 200;
		glutPostRedisplay();
		break;
	case 100:			// d - Jump to 0 for X-coordinate
		eyeX = 0;
		glutPostRedisplay();
		break;
	case 101:			// e - Switch for glPolyMode(GL_LINE);
		if (flag){
			flag = false;
		} else {
			flag = true;
		}

		glutPostRedisplay();
		break;
	}
}

void arrows(int key, int x, int y){
	if(key == GLUT_KEY_LEFT){
		if(eyeX < 300){
			eyeX = eyeX + 5;
		}
	} else if (key == GLUT_KEY_UP){
		if(step < 0.005){
			step += 0.0005;
		}
	} else if (key == GLUT_KEY_DOWN){
		if(step > -0.005){
			step -= 0.0005;
		}
	} else if (key == GLUT_KEY_RIGHT){
		if(eyeX > -40){
			eyeX = eyeX - 5;
			glutPostRedisplay();
		}
	}
	glutPostRedisplay();
}

int main(int argc, char** argv){
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize (1000, 800); 
	glutInitWindowPosition (0, 0);
	glutCreateWindow ("Erik Paluka & Brittany Kondo - Advanced Computer Graphics - Final Project");
	
	string s;
	cout << "Should the path curve? Enter 'yes' or 'no'.\n";
	cin >> s;
	if (s.compare("yes") == 0 || s.compare("y") == 0 || s.compare("YES") == 0){
		cout << "\nYou answered yes.\nThere will be a curve in the path.\n\n";
		FLAG_CURVE = true;
	} else {
		cout << "\nYou answered no.\nThere will not be a curve in the path.\n\n";
		FLAG_CURVE = false;
	}
	cout << "Should the trees be textures or procedurally drawn?\nEnter 'yes' for texture trees or 'no' for procedurally drawn trees.\n";
	cin >> s;
	if (s.compare("yes") == 0 || s.compare("y") == 0 || s.compare("YES") == 0){
		cout << "\nYou answered yes.\nThe trees will be textures.\n\n";
		FLAG_TEX_TREES = true;
	} else {
		cout << "\nYou answered no.\nThe trees will be procedurally drawn.\n\n";
		FLAG_TEX_TREES = false;
	}
	int n;
	cout << "How many trees should be drawn?\n";
	cin >> n;
	if (n > 2500 && !FLAG_TEX_TREES){
		cout << "That is too many procedurally drawn trees. Number of trees set to 2500.\n\n";
		NUM_TREES = 2500;
		GROWTH = 2;
	} else if (n > 0 && !FLAG_TEX_TREES){
		cout << "Number of trees set to " << n << ".\n\n";
		NUM_TREES = n;
		GROWTH = 3;
	} else if (n > 0 && FLAG_TEX_TREES){
		cout << "Number of trees set to " << n << ".\n\n";
		NUM_TREES = n;
		GROWTH = 8;
	}
	glShadeModel(GL_SMOOTH);
	srand (time(NULL));
	trees = new tree[NUM_TREES];

	printf("Initializing Trees... ");
	for(int j = 1; j < NUM_TREES; j++){
		if (FLAG_TEX_TREES){
			trees[j].theta = rand()*(TREE_THETA)/RAND_MAX;
			trees[j].y = (RADIUS)*cos(trees[j].theta);
			trees[j].z = (RADIUS)*sin(trees[j].theta);

			if (j % 3 == 0){
				trees[j].x = rand()*(WIDTH_PATH)/RAND_MAX + WIDTH_PATH + 10;
			} else {
				trees[j].x = -(rand()*(WIDTH_PATH*10.0)/RAND_MAX + WIDTH_PATH + 10);
			}
		} else {
			if(j % 2 == 0){
				float num = (float) rand()/RAND_MAX;
				expand(num, j);
				//trees[j].list = glGenLists(1);
				//glNewList(trees[j].list, GL_COMPILE);
				//createTree(j);
				//glEndList();
			} else {
				//trees[j].list = glGenLists(1);
				//glNewList(trees[j].list, GL_COMPILE);
				createTree2(j);
				//glEndList();
			}
		}

	}
	printf("Done!\n");
	printf("Creating Vertices for the Ground... ");
	createGround((WIDTH_PATH+0.0001)*GROUND_WIDTH_MULT/5, GROUND_WIDTH_MULT/5, "water");
	createGround(WIDTH_PATH+0.0001, GROUND_WIDTH_MULT/5, "left");
	createPath();
	createGround(WIDTH_PATH-0.0001, GROUND_WIDTH_MULT, "right");
	printf("Done!\n");
	printf("Loading Textures... ");
	//Load the texture for the background
	background = loadTextureRAW("back.raw", B_WIDTH, B_HEIGHT, 3);   

	//Load the texture for the path
	path = loadTextureRAW("path.raw", P_WIDTH, P_HEIGHT, 3);     

	//Load the texture for the curb
	curb = loadTextureRAW("curb.raw", C_WIDTH, C_HEIGHT, 3);    

	//Load the texture for the grass
	//grass = loadTextureRAW("grass.raw", G_WIDTH, G_HEIGHT, 3);

	//Load the texture for the water
	water = loadTextureRAW("water.raw", W_WIDTH, W_HEIGHT, 3);

	//Load the texture for the snow
	//snow = loadTextureRAW("snow.raw", S_WIDTH, S_HEIGHT, 3); 

	//Load the texture for the snow
	snow2 = loadTextureRAW("snow2.raw", S2_WIDTH, S2_HEIGHT, 3); 
	printf("Done!\n");
	printf("Initializing Particle System (Snow)... ");
	for(int i = 0; i < NUM_FLAKES; i++){
		resetSnow(i);
	}
	printf("Done!\n");
	GLfloat lightP[4] = {0, 2*RADIUS, RADIUS/6, 1.0};
	glLightfv(GL_LIGHT0, GL_POSITION, lightP);

	// set the ambient light colour    
	GLfloat lightA[4] = {1.0, 1.0, 1.0, 0.3};
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightA);

	// set the specular light colour         
	GLfloat lightS[4] = {1.0, 1.0, 1.0, 1.0};
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightS);

	// set the diffuse light colour
	GLfloat lightD[4] = {1.0, 1.0, 1.0, 1.0};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightD);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);  
	/* Use depth buffering for hidden surface elimination. */
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(arrows);
	glutIdleFunc(animate);
	glutMainLoop();
	return 0;
}