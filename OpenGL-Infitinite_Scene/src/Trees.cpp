/*
*  Trees.cpp
*
*  Erik Paluka
*  December, 2011
*
*	
*/

#include "Paluka-Infinite_Scene.h"



GLUquadric *cyl	= gluNewQuadric();
float thick;
float orig_thick;
int GROWTH = 8;

void expand(float num, int k){
	//Set the beginning axiom for the L-System
	//and expand it
	trees[k].str = "X";

	for(int j = 0; j <= TREE_HEIGHT; j++){
		string ch = "";

		for (int i = 0; i < trees[k].str.length(); i++){
			ch = trees[k].str.at(i);

			if (ch.compare("D") == 0){
				trees[k].str.replace(i, 1, "DD");
				i = i + 1;
			} else if (ch.compare("X") == 0){

				if (num < 0.45){
					// trees[k].str.replace(i, 1, "D[LX][RX][LX]D[RX]LX");
					trees[k].str.replace(i, 1, "D[LXS]D[RXS]LX");

				} else {
					//trees[k].str.replace(i, 1, "D[RX][LX][RX]D[LX]RX");
					trees[k].str.replace(i, 1, "D[RXS]D[LXS]RX");
				}
				// i = i + 20;
				i = i + 14;
			} 

		}
	}

	///////////////////////////////////
	trees[k].theta = rand()*(TREE_THETA)/RAND_MAX;
	trees[k].y = (RADIUS)*cos(trees[k].theta);
	trees[k].z = (RADIUS)*sin(trees[k].theta);
	trees[k].lineWidth = (rand()*2)/RAND_MAX + 1.0;

	if (k % 3 == 0){
		trees[k].x = (rand()*WIDTH_PATH)/RAND_MAX + WIDTH_PATH + 10;
	} else {
		trees[k].x = -(rand()*(WIDTH_PATH*10)/RAND_MAX + WIDTH_PATH + 10);
	}



}

void rotL(){
	float angle = (float) rand()/RAND_MAX*2 + 20;
	glRotatef(angle, 1, 0, 0);
	glRotatef(angle*4, 0, upY, upZ);
}
void rotR(){
	float angle = (float) rand()/RAND_MAX*2 + 20;
	glRotatef(-angle, 1, 0, 0);
	glRotatef(angle*4, 0, upY, upZ);
}


void drawLine(float width, float length){
	glPushAttrib(GL_LIGHTING_BIT);//saves current lighting stuff


	//glColor3f(0.37, 0.18, 0.0);
	GLfloat ambient[4] = {0.37, 0.18, 0.0, 1.0};    // ambient reflection
	//GLfloat specular[4] = {1.0, 1.0, 1.0, 1.0};   // specular reflection
	GLfloat diffuse[4] = {0.37, 0.18, 0.0, 1.0};   // diffuse reflection


	// set the ambient reflection for the object
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	// set the diffuse reflection for the object
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
	// set the specular reflection for the object      
	//glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	glLineWidth(width);

	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(0, upY*length, upZ*length);
	glEnd();

	glTranslatef(0, upY*length, upZ*length);
	glPopAttrib();
}


void drawTree1(int j){

	glPushMatrix();
	glTranslatef(trees[j].x, trees[j].y, trees[j].z);

	string ch = "";
	string LSystem = trees[j].str;

	for (int i = 0; i < LSystem.length(); i++){
		ch = LSystem.at(i);

		if (ch.compare("D") == 0 || ch.compare("X") == 0){
			drawLine(trees[j].lineWidth, LENGTH);
		} else if (ch.compare("[") == 0){
			glPushMatrix();
		} else if (ch.compare("]") == 0){
			glPopMatrix();
		} else if (ch.compare("R") == 0){
			rotR();
		} else if (ch.compare("L") == 0){
			rotL();
		} else if (ch.compare("S") == 0){
			drawSnow();
		}
	}
	glPopMatrix();

}

/////////////////////////////////////////////////////////Other tree

//Applies production rules recursively to the initial string, constructing
//a final string of symbols that will be rendered into a tree
string replaceRules(string s, vector<string> h, vector<string> b, int depth){

	for (int i=0;i<s.length();i++){
		for (int j=0;j<h.size();j++){
			if (h[j].compare(s.substr(i,1))==0){				
				s.replace(i,1,b[j]);
				i+=b[j].length();
				break;
			}
		}
	}		
	if(depth < GROWTH)
		return replaceRules(s,h,b,depth+1);
	else
		return s;

}

void positiveRotate(){
	float angle = (float) rand()/RAND_MAX*2 + 20;

	glRotatef(angle, 1, 0, 0);
	//glRotatef(angle, 0, 1, 0);
	glRotatef(angle, 0, upY, upZ);

}
void negativeRotate(){	   
	float angle = -((float) rand()/RAND_MAX*2 + 20);

	glRotatef(angle, 1, 0, 0);
	//glRotatef(angle, 0, 1, 0);
	glRotatef(angle, 0, upY, upZ);     


}
void drawSnow(){   
	glPushAttrib(GL_LIGHTING_BIT);//saves current lighting stuff


	//glColor3f(0.37, 0.18, 0.0);
	GLfloat ambient[4] = {1.0, 1.0, 1.0, 0.3};    // ambient reflection
	GLfloat specular[4] = {1.0, 1.0, 1.0, 1.0};   // specular reflection
	GLfloat diffuse[4] = {1.0, 1.0, 1.0, 1.0};   // diffuse reflection


	// set the ambient reflection for the object
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	// set the diffuse reflection for the object
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
	// set the specular reflection for the object      
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);

	//glColor3f(1,1,1); 	
	glPointSize(2);
	glBegin(GL_POINTS);			    
	glVertex3f(0,0,0);
	glEnd();
	glPopAttrib();


}

//Called during each time step of the animation
void drawTree2(int j){
	//Index to locate line thicknesses
	int index_line = 0;
	vector <float> thickness;
	string r = trees[j].str;

	//Initialize the stack to contain the model view matrix
	int count=0;
	for(int i=0;i<r.length();i++){
		if (r.substr(i,1).compare("d")==0){				 
			count++;
		}else{			 
			if(count>0){
				for(int j=0;j<count;j++){
					thickness.push_back(count);
				}
			}		
			count=0;
		}
	}
	glPushMatrix();
	glTranslatef(trees[j].x, trees[j].y, trees[j].z);

	for (int i=0;i<r.length();i++){	
		string symbol = r.substr(i,1);

		if(symbol.compare("d")==0){
			drawLine(thickness[index_line], UNIT_LENGTH);
			index_line++;
		}
		else if(symbol.compare("p")==0){
			positiveRotate();	
		}
		else if(symbol.compare("q")==0){
			negativeRotate();	
		}
		else if (symbol.compare("[")==0){
			glPushMatrix();
		}
		else if (symbol.compare("]")==0){	
			glPopMatrix();

		}
		//Snow
		else if(symbol.compare("l")==0){
			drawSnow();					
		}

	}
	glPopMatrix();

}

void createTree2(int j){
	vector<string> heads;
	vector<string> bodies;
	//Rule 1
	string head_1 = "X";
	heads.push_back(head_1);	
	string body_1 = "DS[RX]D[LX]RX";	
	bodies.push_back(body_1);
	//Rule 2
	string head_2 = "D";
	heads.push_back(head_2);
	string body_2 = "DD";
	bodies.push_back(body_2);
	//Initial string
	string initial = "X";



	trees[j].theta = rand()*(TREE_THETA)/RAND_MAX;

	if (j % 3 == 0){
		trees[j].x = (rand()*WIDTH_PATH)/RAND_MAX + WIDTH_PATH + 10;
	} else {
		trees[j].x = -(rand()*(WIDTH_PATH*10)/RAND_MAX + WIDTH_PATH + 10);
	}
	trees[j].y = (RADIUS)*cos(trees[j].theta);
	trees[j].z = (RADIUS)*sin(trees[j].theta);
	trees[j].str = replaceRules(initial,heads,bodies,1);	 

}








//////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
// Cylinder

void drawLine3(float i){


	gluCylinder(cyl, i*0.2, i*0.2, UNIT_LENGTH, 20, 20);		
	glTranslatef(0,0,UNIT_LENGTH);



}

void createCTree(int j, string r){
	float rotation_max = 30;
	float  rotation_min = 10;
	//Assign random rotation angles
	for (int i=0; i < r.length(); i++){		 
		string next = r.substr(i, 1);			
		
		if(next.compare("R")==0){			
			float range = rotation_max-rotation_min;
			float rotation = rotation_min + (range * rand()) / (RAND_MAX + 1.0);		    
			trees[j].rotations_y.push_back(rotation);
			rotation = rotation_min + (range * rand()) / (RAND_MAX + 1.0);	
			trees[j].rotations_z.push_back(rotation);
			rotation = rotation_min + (range * rand()) / (RAND_MAX + 1.0);	
			trees[j].rotations_x.push_back(rotation);
		}
		if(next.compare("L")==0){

			float range = rotation_max-rotation_min;
			float rotation = rotation_min + (range * rand()) / (RAND_MAX + 1.0);		   
			trees[j].rotations_y.push_back(rotation);
			rotation = rotation_min + (range * rand()) / (RAND_MAX + 1.0);	
			trees[j].rotations_z.push_back(rotation);
			rotation = rotation_min + (range * rand()) / (RAND_MAX + 1.0);	
			trees[j].rotations_x.push_back(rotation);

		}	
	} 
}
//Called during each time step of the animation
void drawCTree(int j, string r){

	vector<float>thickness;
	//Indices to locate random rotation angles and line thicknesses
	int index_line = 0;
	int index_rotate=0;	
	
	
	//Initialize the stack to contain the model view matrix
	//Assign different thicknesses by counting consecutive d's in the result string
	int count=0;
	for(int i=0;i<r.length();i++){
		if (r.substr(i,1).compare("D")==0){				 
			count++;
		}else{			 
			if(count>0){
				for(int j=0;j<count;j++){
					thickness.push_back(count);
				}
			}		
			count=0;
		}
	}

	

	glPushAttrib(GL_LIGHTING_BIT);//saves current lighting stuff


	//glColor3f(0.37, 0.18, 0.0);
	GLfloat ambient[4] = {0.37, 0.18, 0.0, 1.0};    // ambient reflection
	//GLfloat specular[4] = {1.0, 1.0, 1.0, 1.0};   // specular reflection
	GLfloat diffuse[4] = {0.37, 0.18, 0.0, 1.0};   // diffuse reflection


	// set the ambient reflection for the object
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	// set the diffuse reflection for the object
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
	// set the specular reflection for the object      
	//glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	
	
	for (int i=0;i<r.length();i++){	
		string symbol = r.substr(i,1);

		if(symbol.compare("D")==0){


			drawLine3(thickness[index_line]);


			index_line++;
		}
		else if(symbol.compare("R")==0){
			glRotatef(trees[j].rotations_x[index_rotate],1,0,0);

			glRotatef(trees[j].rotations_y[index_rotate],0,1,0);
			glRotatef(trees[j].rotations_z[index_rotate],0,0,1);
			index_rotate++;
		}
		else if(symbol.compare("L")==0){
			glRotatef(-trees[j].rotations_x[index_rotate],1,0,0);

			glRotatef(-trees[j].rotations_y[index_rotate],0,1,0);
			glRotatef(-trees[j].rotations_z[index_rotate],0,0,1);
			index_rotate++;
		}
		else if (symbol.compare("[")==0){
			glPushMatrix();
		}
		else if (symbol.compare("]")==0){	
			glPopMatrix();

		}
		//Snow
		else if(symbol.compare("S")==0){
			drawSnow();		
		}

	}
	glPopAttrib();

}

