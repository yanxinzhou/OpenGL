#include<string>
using namespace std;

const int TREE_HEIGHT = 2;
const float LENGTH = 5;//0.8;//1.5;
const float PI = 3.14159265;
const float TREE_THETA = PI*2.0;

const float UNIT_LENGTH = 2;

extern bool randRot;
extern int GROWTH;




void expand(float num, int k);
void rotL();
void rotR();
void drawLine(float lt, float t);
void drawTree1(int j);

void positiveRotate(int i);
void negativeRotate(int i);
void drawSnow();
void drawTree2(int j);
void createTree2(int j);
string replaceRules(string s, vector<string> h,vector<string> b, int depth);

void drawLine3(float i);
void drawCTree(int j, string s);
void createCTree(int j, string r);
