#ifndef _SHAPE_H_
#define _SHAPE_H_

#include "vecmath/Vector3.h"
#include "vecmath/Point3.h"
#include <vector>
#include <cmath>
#include "ShapesUI.h"
#include "UIInterface.h"
#include "../cse452.h"

//include intersection part
#include "../intersection/HitRecord.h"

//define triangle

class Triangle{
	public:
		//build the triangle
		Triangle(Point3 _p1, Point3 _p2, Point3 _p3, Vector3 _v1, Vector3 _v2, Vector3 _v3);
		//three points for the triangle
		Point3 p1,p2,p3;
		//normal per vertex of triangles, we have three normals per triangle
		Vector3 v1,v2,v3;

		void DrawTriangles();
	
};  


class Shape {
public:
	Shape();
	Shape(int _t1);
	Shape(int _t1,int _t2);
	~Shape();

	//draw current shape
	void drawnow();

	//Intersection part
	virtual HitRecord intersect(const Point3& point, Vector3& distance);

//protect children in the current class
protected: 
	int t1;
	int t2;
	std::vector<Triangle> triangles;
	GLuint list;
	void listbuild();
};

class Cube : public Shape{
public:
	Cube(int _t1);
	HitRecord intersect(const Point3& point, Vector3& distance);
};
 

class Cylinder : public Shape{
public:
	Cylinder(int _t1, int _t2);
	HitRecord intersect(const Point3& point, Vector3& distance);
};

class Cone :public Shape{
public:
	Cone(int _t1, int _t2);
	HitRecord intersect(const Point3& point, Vector3& distance);
};


class Sphere:public Shape{
public:
	Sphere(int _t1);
	HitRecord intersect(const Point3& point, Vector3& distance);

private:
	void icotoSphere();
}; 

class Torus :public Shape{
public:
	Torus(int _t1, int _t2);
	HitRecord intersect(const Point3& point, Vector3& distance);
};

class Corkscrew :public Shape{
public:
	Corkscrew(int _t1, int _t2);
	HitRecord intersect(const Point3& point, Vector3& distance);
};

class Banana :public Shape{
public:
	Banana(int _t1,int _t2);
};


#endif 
