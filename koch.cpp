#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "flake.h"
using namespace std;

void printFlakes(flake list[],int length,char* formatString,vector offset,FILE* fout){
	for (int i=0; i<length;i++){
		vector pos=offset+list[i].position;
		color clr=list[i].clr;
		fprintf(fout,formatString,pos.x,pos.y,(int)clr.r,(int)clr.g,(int)clr.b);
		//fprintf(fout, (list[i].svgStr()).c_str());
	}
}
/*
flake * generateFlakes(flake* list,int length){
	flake * output=  (flake*) malloc(length*sizeof(flake)*6);
	//cout << (length*6) <<"\n";
	for (int i=0; i<length;i++){
		//cout << output+(6*i);
		list[i].spawn(output+(6*i));
	}
	return output;
}
*/
void generateFlakes(flake* list,int length,flake* output){
	//This version writes the flakes to a specified memory location.
	for (int i=0; i<length;i++){
		list[i].spawn(output+(6*i));
	}
}
int main(int argc,char *argv[])
{
	if(argc!=2){
		cout << "Syntax Error :P";
		return 1;
	}
	int depth=atoi(argv[1]);
	if (depth<2){
		cout << "Minimum depth is 2";
		return 2;
	}
	//The following generates the SVG strings for different flakes
	int **lSystems=(int**) malloc(sizeof(int *)*depth);
	int temp[12]={0,5,1,0,2,1,3,2,4,3,5,4}; //Starts in upper left corner
	lSystems[0]=temp;
	int len=48;
	for (int i=1;i<depth;i++){
		lSystems[i]=(int*)malloc(sizeof(int)*len);
		for (int j=0;j<len/4;j++){
			int base=lSystems[i-1][j];
			lSystems[i][4*j  ]= base;
			lSystems[i][4*j+1]=(base+5)%6;
			lSystems[i][4*j+2]=(base+1)%6;
			lSystems[i][4*j+3]= base;
		}
		len*=4;
	}
	float seglen=flake::rBase*pow(3,0.5-(float)depth);
	char **flakeStr=(char**) malloc(sizeof(int *)*depth);
	char basisStrings[6][34];
	//This precomputes the 6 basis vectors required for tracing the flakes.
	for (int i=0;i<6;i++){
		sprintf(basisStrings[i],"l%+15.7e %+15.7e ",
			vector::basis[i].x*seglen,
			vector::basis[i].y*seglen);
	}
	len=12; //Note that len is being reused
	for (int i=0;i<depth;i++){
		//14 chars for each float (should allow depth 9 fine).
		//3 chars for L f f
		// floats will be _.________e+_ : %.7e
		flakeStr[i]=(char*)malloc(len*33+59);//59=lengths of front+tail+1 for null;
		strcpy(flakeStr[i],"<path d=\"M%+15.7e %+15.7e ");//Length 26, expands to 41
		char* location= flakeStr[i]+26;
		//Note that we are building a format string for use later
		for (int j=0;j<len;j++){
			strcpy(location,basisStrings[lSystems[i][j]]);
			location+=33;
		}
		strcpy(location,"Z\" fill=\"rgb(%03i,%03i,%03i)\"/>\n");//length 32
		len*=4;
	}
	//We now have format strings for every degree flake.
	int length=7+(int)(pow(6.0,depth-1)-6)*4/5; //geometric sum w/ratio 6
	flake * output=  (flake*) malloc(length*sizeof(flake));
	FILE * fout =fopen("out.svg", "w");
	fprintf(fout,"<?xml version=\"1.0\" standalone=\"no\"?>\n\
<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\"\n\
\"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n\
<svg width=\"100%\" height=\"100%\" version=\"1.1\">\n");
	//Three primary color flakes
	flake* flake0=output;
	flake0[0]=hardFlake(flake::rChild*vector::basis[5],color::red  ,0);
	flake0[1]=hardFlake(flake::rChild*vector::basis[1],color::blue ,0);
	flake0[2]=hardFlake(flake::rChild*vector::basis[3],color::green,0);
	flake* baseAncestor[4];
	for(int i=0;i<4;i++)
		baseAncestor[i]=&flake0[i];
	printFlakes(flake0,3,flakeStr[depth-1],flake::rBase*vector::altBasis[4],fout);
	//Three secondary color flakes and white
	flake* flake1=output+3;
	flake1[0]=hardFlake(flake::rChild*vector::basis[0],
		color::red+color::blue,1,baseAncestor);//magenta
	flake1[1]=hardFlake(vector(0,0),
		color::red+color::blue+color::green,1,baseAncestor);//white
	flake1[2]=hardFlake(flake::rChild*vector::basis[4],
		color::red+color::green,1,baseAncestor);//yellow
	flake1[3]=hardFlake(flake::rChild*vector::basis[2],
		color::green+color::blue,1,baseAncestor);//cyan
	printFlakes(flake1,4,flakeStr[depth-2],flake::rBase/3*vector::altBasis[4],fout);
	flake* position=output+7;
	flake* last=flake1;
	int numLast=4;
	for (int i=2;i<depth;i++){
		generateFlakes(last,numLast,last+numLast);
		last=last+numLast;
		numLast*=6;
		printFlakes(last,numLast,flakeStr[depth-i-1],
			flake::rBase*pow(3.0,-i)*vector::altBasis[4],fout);
	}
	fprintf(fout,"</svg>");
	fclose(fout);
	return 0;
}
