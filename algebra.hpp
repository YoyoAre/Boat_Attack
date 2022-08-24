#include<math.h>
#include<stdio.h>
#define PI 3.14159265358979323846
class Punto{
public:
	float x,y,z,w;
	Punto(float x,float y,float z,float w = 0){
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}
	Punto(){
		this->x = 0;
		this->y = 0;
		this->z = 0;
		this->w = 0;
	}
	void print(){
		printf("(%f,%f,%f)\n",x,y,z);
	}
	Punto operator =(Punto a){
		this->x = a.x;
		this->y = a.y;
		this->z = a.z;
		this->w = 0;
		return *this;
	}
	Punto operator +(Punto a){
		return Punto(x+a.x,y+a.y,z+a.z);
	}
	Punto operator *(int n){
		return Punto(x*n,y*n,z*n);
	}
	Punto operator *(float n){
		return Punto(x*n,y*n,z*n);
	}
	Punto operator -(Punto a){
		return *this+(a*-1);
	}
	Punto operator /(float n){
		return Punto(x/n,y/n,z/n);
	}
	Punto operator /(int n){
		return Punto(x/n,y/n,z/n);
	}
};
class Vector:public Punto{
public:
	Vector(float x,float y,float z,float w = 0):Punto(x,y,z,w){};
	Vector():Punto(0,0,0){};
	Vector(Punto a,Punto b):	Punto(b.x-a.x, b.y-a.y, b.z-a.z){};
	Vector(Vector a,Vector b):	Punto(b.x-a.x, b.y-a.y, b.z-a.z){};
	float norma(){
		return sqrt(pow(this->x,2) + pow(this->y,2) + pow(this->z,2));
	}
	
	Vector operator =(Vector a){
		this->x = a.x;
		this->y = a.y;
		this->z = a.z;
		this->w = a.w;
		return *this;
	}
	Vector operator +(Vector a){
		return Vector(x+a.x,y+a.y,z+a.z);
	}
	Vector operator *(int n){
		return Vector(x*n,y*n,z*n);
	}
	Vector operator *(float n){
		return Vector(x*n,y*n,z*n);
	}
	Vector operator -(Vector a){
		return *this+(a*-1);
	}
	Vector operator /(float n){
		return Vector(x/n,y/n,z/n);
	}
	Vector operator /(int n){
		return Vector(x/n,y/n,z/n);
	}
	
};
Vector normalizar(Vector a){
	float n = a.norma();
	return (a/n);
}
Vector cruz(Vector u,Vector v){
	//Devuelve el producto cruz de 2 vectores, que es normalmente, el vector normal de un plano
	return Vector(u.y*v.z-u.z*v.y, u.z*v.x-u.x*v.z, u.x*v.y-u.y*v.x);
}
float grados(float radian){
	return (radian * 180 / PI);
}
float radian(float grados){
	return (grados * PI / 180);
}
float angulo(float x, float y){
	if(y > 0){
		return grados(acos(x));
	}else{
		return 360 - grados(acos(x));
	}
}
float min(float a,float b){
	return a<b?a:b;
}
float max(float a,float b){
	return a>b?a:b;	
}