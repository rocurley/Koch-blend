#include <string>
#include "vector.h"
#include "color.h"
using namespace std;

class flake;
class flake{
public:
	int degree;
	int direction; //Index of basis from child to parent
	flake* parent; //Has to be a pointer or won't compile b/c flake is not
				   //yet full declared... No clue why this makes sense...
	flake* child [6];
	vector position;
	color clr;
	flake** ancestors;
	flake* DEBUG[3];
	static const double rBase;
	static const double rChild;
	static const int pointerLength;
	flake();
	flake(flake*,int);
	int getDegree();
	string svgStr();
	flake* spawn();
	void spawn(flake*);
	bool withinHex(vector);
	void getAncestors(flake*);
};
const double flake::rBase=500;
const double flake::rChild=2*pow(3.0,-0.5)*rBase;
const int ptrLen=sizeof(int *);
flake::flake(){
}
flake::flake(flake* initParent,int initDirection){
	parent=initParent;
	direction=initDirection;
	degree=(*parent).degree+1;
	position=(*parent).position-vector::basis[direction]*(rChild*pow(3.0,-(*parent).degree));
	clr=color::black;
	ancestors = (flake**) malloc(ptrLen*(degree+2));
	ancestors[0]=parent;
	memcpy(ancestors+1,(*parent).ancestors,ptrLen*(degree+1));
	for(int i=0;i<3;i++){
		vector testPoint=position+vector::basis[(direction+2*i)%6]*(rChild*pow(3.0,-degree));
		for(int j=0;j<degree+2;j++){
			if((*ancestors[j]).withinHex(testPoint)){
				clr=clr+((*ancestors[j]).clr);
				DEBUG[i]=ancestors[j];
				break;
			}
		}
		
	}
	clr=color::normalize(clr);
}
int flake::getDegree(){
	return degree;
}
string flake::svgStr(){
	char buffer [200];//This is pretty stupid, make it good...
	size_t len=sprintf(buffer,"<circle cx=\"%f\" cy=\"%f\" r=\"%f\" fill=\"rgb(%i,%i,%i)\"/>\n"\
			,position.x,position.y,(rBase*pow(3.0,-degree))\
			,int(clr.r),int(clr.g),int(clr.b));
	return string(buffer,len);
}
void flake::spawn(flake* pointer){
	//This version writes the flakes to a specified memory location.
	for(int i=0;i<6;i++){
		pointer[i]=flake(this,i);
	}
}
bool flake::withinHex(vector point){
	double x=point.x-position.x;
	double y=point.y-position.y;
	double u= x*pow(3.0,-0.5)+y;
	double v=-x*pow(3.0,-0.5)+y;
	double radius=rChild*pow(3.0,-degree);
	return ( radius>u && u>0 &&  radius>v && v>0) || 
		   (-radius<u && u<0 && -radius<v && v<0) || 
		   ((0.5>(y/radius) && (y/radius)>-0.5) && 
		   (pow(3.0,0.5)/2>(x/radius) && (x/radius)>-pow(3.0,0.5)/2));
}

class hardFlake: public flake{
public:
	hardFlake(vector,color,int);
	hardFlake(vector,color,int,flake**);
};

hardFlake::hardFlake(vector initPosition,color initColor,int initDegree){
	position=initPosition;
	clr=initColor;
	degree=initDegree;
}
hardFlake::hardFlake(vector initPosition,color initColor,int initDegree,
		flake** initAncestors){
	position=initPosition;
	clr=initColor;
	degree=initDegree;
	ancestors=initAncestors;
}



