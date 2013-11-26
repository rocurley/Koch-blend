#include <string>
using namespace std;
class color{
public:
	double r;
	double g;
	double b;
	color(double,double,double);
	color();
	string toString();
	static color normalize(const color);
	static const color red;
	static const color green;
	static const color blue;
	static const color black;
};
color::color(double initR,double initG,double initB){
	r=initR;
	g=initG;
	b=initB;
};
color::color(){
	r=0;
	g=0;
	b=0;
};
color operator+(const color in1,const color in2){
	return color(in1.r+in2.r,in1.g+in2.g,in1.b+in2.b);
};
color operator-(const color in1,const color in2){
	return color(in1.r-in2.r,in1.g-in2.g,in1.b-in2.b);
};
color operator*(const double in1,const color in2){
	return color(in1*in2.r,in1*in2.g,in1*in2.b);
};
color operator*(const color in1,const double in2){
	return color(in1.r*in2,in1.g*in2,in1.b*in2);
};
color operator/(const color in1,const double in2){
	return color(in1.r/in2,in1.g/in2,in1.b/in2);
};
string color::toString(){
	std::stringstream sstr; //makes an object that acts like a stream
	sstr << "("<<r<<","<<g<<","<<b<<")";
	return sstr.str();//Outputs everything written to the string
};
color color::normalize(color input){ //You only write static in the class def,
									 //not in the file, idk why...
	return (input*255/max(input.r,max(input.g,input.b)));
};
const color color::red=  color(255,0,0);
const color color::green=color(0,255,0);
const color color::blue= color(0,0,255);
const color color::black=color(0,0,0);