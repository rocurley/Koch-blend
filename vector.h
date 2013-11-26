#include <string>
using namespace std;
class vector{
public:
	double x;
	double y;
	vector(double,double);
	vector();
	string toString();
	static const vector basis [6];
	static const vector altBasis [6];
};
vector::vector(double initX,double initY){
	x=initX;
	y=initY;
};
vector::vector(){
	x=0;
	y=0;
};
string vector::toString(){
	std::stringstream sstr; //makes an object that acts like a stream
	sstr << "("<<x<<","<<y<<")";
	return sstr.str();//Outputs everything written to the string
}
vector operator+(const vector in1,const vector in2){
	return vector(in1.x+in2.x,in1.y+in2.y);
};
vector operator-(const vector in1,const vector in2){
	return vector(in1.x-in2.x,in1.y-in2.y);
};
vector operator*(const double in1,const vector in2){
	return vector(in1*in2.x,in1*in2.y);
};
vector operator*(const vector in1,const double in2){
	return vector(in1.x*in2,in1.y*in2);
};
vector operator/(const vector in1,const double in2){
	return vector(in1.x/in2,in1.y/in2);
};
const vector vector::basis[6]={vector(0,1),vector(pow(3,.5)/2,0.5),vector(pow(3,.5)/2,-0.5),
	vector(0,-1),vector(-pow(3,.5)/2,-0.5),vector(-pow(3,.5)/2,0.5)};
const vector vector::altBasis[6]={vector(1,0),vector(0.5, pow(3,.5)/2),vector(-0.5, pow(3,.5)/2),
	vector(-1,0) ,vector(-0.5,-pow(3,.5)/2),vector(0.5,-pow(3,.5)/2)};