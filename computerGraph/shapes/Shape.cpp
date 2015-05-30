#include "Shape.h"
#include <iostream>
#include <fstream>

using namespace std; 

Triangle::Triangle(Point3 _p1, Point3 _p2, Point3 _p3, Vector3 _v1, Vector3 _v2, Vector3 _v3){
	//initialize
	p1=_p1;
	p2=_p2;
	p3=_p3;
	v1=_v1;
	v2=_v2;
	v3=_v3;
	//normalize vector, in "Vector3" file
	v1.normalize();
	v2.normalize();
	v3.normalize();
}

void Triangle::DrawTriangles(){
	glNormal3f(v1[0],v1[1],v1[2]);
	glVertex3f(p1[0],p1[1],p1[2]);
	glNormal3f(v2[0],v2[1],v2[2]);
	glVertex3f(p2[0],p2[1],p2[2]);
	glNormal3f(v3[0],v3[1],v3[2]);
	glVertex3f(p3[0],p3[1],p3[2]);
}


//**************function in class shape*******************//
void Shape::listbuild(){
	//create a list
	list=glGenLists(GLsizei(1));
	glNewList(list,GL_COMPILE);
	glBegin(GL_TRIANGLES);
	for(std::vector<Triangle>::iterator tr =triangles.begin(); tr != triangles.end(); tr++){
		tr->DrawTriangles();
	}
	glEnd();
	glEndList();

}

void Shape::drawnow(){
	//when there is no list, create a list, or we just need to show it
	if(list==0)
		listbuild();
	else
		glCallList(list);
}

Shape::Shape(){
	//initialize the list
	list=0;
}

Shape::Shape(int _t1){
	t1=_t1;
	list=0;
}

Shape::Shape(int _t1, int _t2){
	t1=_t1;
	t2=_t2;
	list=0;
}

Shape::~Shape(){
	//delete it
	if(list!=0)
		glDeleteLists(list,1);
	list=0;
}     


/******************Shape function*****************/
Cube::Cube(int _t1){
	t1=_t1;

	Vector3 vx_p=Vector3 (1,0,0);
	Vector3 vx_n=Vector3 (-1,0,0);
	Vector3 vy_p=Vector3 (0,1,0);
	Vector3 vy_n=Vector3 (0,-1,0);
	Vector3 vz_p=Vector3 (0,0,1);
	Vector3 vz_n=Vector3 (0,0,-1);

	for(int i=0;i<t1;i++){
		for(int j=0;j<t1;j++){
			//Point3 (0.0,0.0,0.0) (double,double,double)
			//positive x axis 
			//(0.5,-0.5,-0.5),(0.5,0.5,-0.5),(0.5,0.5,0.5)(0.5,-0.5,0.5)
			triangles.push_back(Triangle(Point3(0.5,-0.5+(double)i/t1,-0.5+(double)j/t1),
										Point3(0.5,-0.5+(double)(i+1)/t1,-0.5+(double)j/t1),
										Point3(0.5,-0.5+(double)(i+1)/t1,-0.5+(double)(j+1)/t1),
										vx_p,vx_p,vx_p));
			triangles.push_back(Triangle(Point3(0.5,-0.5+(double)i/t1,-0.5+(double)j/t1),
										Point3(0.5,-0.5+(double)(i+1)/t1,-0.5+(double)(j+1)/t1),
										Point3(0.5,-0.5+(double)i/t1,-0.5+(double)(j+1)/t1),
										vx_p,vx_p,vx_p));
			
			//negative y axis
			//(0.5,-0.5,0.5),(0.5,-0.5,-0.5),(-0.5,-0.5,-0.5)(-0.5,-0.5,0.5)
			triangles.push_back(Triangle(Point3(-0.5+(double)i/t1,-0.5,-0.5+(double)j/t1),
										Point3(-0.5+(double)(i+1)/t1,-0.5,-0.5+(double)j/t1),
										Point3(-0.5+(double)(i+1)/t1,-0.5,-0.5+(double)(j+1)/t1),
										vy_n,vy_n,vy_n));
			triangles.push_back(Triangle(Point3(-0.5+(double)i/t1,-0.5,-0.5+(double)j/t1),
										Point3(-0.5+(double)(i+1)/t1,-0.5,-0.5+(double)(j+1)/t1),
										Point3(-0.5+(double)i/t1,-0.5,-0.5+(double)(j+1)/t1),
										vy_n,vy_n,vy_n));

			//positive z axis
			//(0.5,-0.5,0.5),(0.5,0.5,0.5),(-0.5,0.5,0.5),(-0.5,-0.5,0.5)
			triangles.push_back(Triangle(Point3(-0.5+(double)(i+1)/t1,-0.5+(double)j/t1,0.5),
										Point3(-0.5+(double)(i+1)/t1,-0.5+(double)(j+1)/t1,0.5),
										Point3(-0.5+(double)i/t1,-0.5+(double)(j+1)/t1,0.5),
										vz_p,vz_p,vz_p));
			triangles.push_back(Triangle(Point3(-0.5+(double)(i+1)/t1,-0.5+(double)j/t1,0.5),
										Point3(-0.5+(double)i/t1,-0.5+(double)(j+1)/t1,0.5),
										Point3(-0.5+(double)i/t1,-0.5+(double)j/t1,0.5),
										vz_p,vz_p,vz_p));

			//negative x axis
			//(-0.5,0.5,-0.5),(-0.5,-0.5,-0.5),(-0.5,-0.5,0.5),(-0.5,0.5,0.5)
			triangles.push_back(Triangle(Point3(-0.5,-0.5+(double)(i+1)/t1,-0.5+(double)j/t1),
										Point3(-0.5,-0.5+(double)i/t1,-0.5+(double)j/t1),
										Point3(-0.5,-0.5+(double)i/t1,-0.5+(double)(j+1)/t1),
										vx_n,vx_n,vx_n));
			triangles.push_back(Triangle(Point3(-0.5,-0.5+(double)(i+1)/t1,-0.5+(double)j/t1),
										Point3(-0.5,-0.5+(double)i/t1,-0.5+(double)(j+1)/t1),
										Point3(-0.5,-0.5+(double)(i+1)/t1,-0.5+(double)(j+1)/t1),
										vx_n,vx_n,vx_n));

			//positive y axis
			//(0.5,0.5,-0.5),(-0.5,0.5,-0.5),(-0.5,0.5,0.5),(0.5,0.5,0.5)
			triangles.push_back(Triangle(Point3(-0.5+(double)(i+1)/t1,0.5,-0.5+(double)j/t1),
										Point3(-0.5+(double)i/t1,0.5,-0.5+(double)j/t1),
										Point3(-0.5+(double)i/t1,0.5,-0.5+(double)(j+1)/t1),
										vy_p,vy_p,vy_p));
			triangles.push_back(Triangle(Point3(-0.5+(double)(i+1)/t1,0.5,-0.5+(double)j/t1),
										Point3(-0.5+(double)i/t1,0.5,-0.5+(double)(j+1)/t1),
										Point3(-0.5+(double)(i+1)/t1,0.5,-0.5+(double)(j+1)/t1),
										vy_p,vy_p,vy_p));

			//negative z axis
			//(-0.5,-0.5,-0.5),(0.5,-0.5,-0.5),(0.5,0.5,-0.5),(-0.5,0.5,-0.5)
			triangles.push_back(Triangle(Point3(-0.5+(double)i/t1,-0.5+(double)j/t1,-0.5),
										Point3(-0.5+(double)i/t1,-0.5+(double)(j+1)/t1,-0.5),
										Point3(-0.5+(double)(i+1)/t1,-0.5+(double)(j+1)/t1,-0.5),
										vz_n,vz_n,vz_n));
			triangles.push_back(Triangle(Point3(-0.5+(double)i/t1,-0.5+(double)j/t1,-0.5),
										Point3(-0.5+(double)(i+1)/t1,-0.5+(double)(j+1)/t1,-0.5),
										Point3(-0.5+(double)(i+1)/t1,-0.5+(double)j/t1,-0.5),
										vz_n,vz_n,vz_n));

	
		}
	}

}

Cylinder::Cylinder(int _t1, int _t2){
	t1=_t1;
	t2=_t2;

	//at least, we need 3 slide to build a cylinder
	if(t1<3)
		t1=3;

	for(int i=0; i<t1; i++){
		//set the top and bottom normal vector
		Vector3 v_top=Vector3 (0.0,1.0,0.0);
		Vector3 v_bottom=Vector3(0.0,-1.0,0.0);
		

		//set the value of axis in top and bottom
		double x_up= 0.5*cos(2*M_PI/t1*(i%t1));
		double z_up= 0.5*sin(2*M_PI/t1*(i%t1));

		double x_down=0.5*cos(2*M_PI/t1*((i+1)%t1));
		double z_down=0.5*sin(2*M_PI/t1*((i+1)%t1));

		//set the side vector
		Vector3 v_side1=Vector3 (x_up,0.0,z_up);
		Vector3 v_side2=Vector3 (x_down,0.0,z_down);
		v_side1.normalize();
		v_side2.normalize();

		//draw the top with triangles
		triangles.push_back(Triangle(Point3(x_up,0.5,z_up),Point3(0.0,0.5,0.0),Point3(x_down,0.5,z_down),
									v_top,v_top,v_top));
		//draw the bottom with triangles
		triangles.push_back(Triangle(Point3(x_down,-0.5,z_down),Point3(0.0,-0.5,0.0),Point3(x_up,-0.5,z_up),
									v_bottom,v_bottom,v_bottom));

		//draw the side of cylinder
		for(int j=0; j<t2; j++){
		triangles.push_back(Triangle(Point3(x_up,-0.5+j/(double)t2,z_up),Point3(x_down,-0.5+(j+1)/(double)t2,z_down),
									Point3(x_down,-0.5+j/(double)t2,z_down),v_side1,v_side2,v_side2));
		triangles.push_back(Triangle(Point3(x_up,-0.5+j/(double)t2,z_up),Point3(x_up,-0.5+(j+1)/(double)t2,z_up),
									Point3(x_down,-0.5+(j+1)/(double)t2,z_down),v_side1,v_side1,v_side2));

		}
	}
}

Cone::Cone(int _t1, int _t2){
	t1=_t1;
	t2=_t2;

	
	//at least, we need 3 slide to build a cylinder
	if(t1<3)
		t1=3;

	for(int i=0; i<t1; i++){
		//set the top and bottom normal vector
		Vector3 v_top=Vector3 (0.0,1.0,0.0);
		Vector3 v_bottom=Vector3(0.0,-1.0,0.0);
		

		//set the value of axis in top and bottom
		double x_up= 0.5*cos(2*M_PI/t1*(i%t1));
		double z_up= 0.5*sin(2*M_PI/t1*(i%t1));

		double x_down=0.5*cos(2*M_PI/t1*((i+1)%t1));
		double z_down=0.5*sin(2*M_PI/t1*((i+1)%t1));

		//set the side vector
		Vector3 v_side1=Vector3 (x_up,0.0,z_up);
		Vector3 v_side2=Vector3 (x_down,0.0,z_down);
		v_side1.normalize();
		v_side2.normalize();

		//set the value of axis for cone slide
		//g= r*(1-(y+0.5))=r*(0.5-y)

		//draw the bottom with triangles
		triangles.push_back(Triangle(Point3(x_down,-0.5,z_down),Point3(0.0,-0.5,0.0),Point3(x_up,-0.5,z_up),
									v_bottom,v_bottom,v_bottom));
		
		//draw the side of cone
		
		for(int j=0; j<t2; j++){
			if (j==(t2-1))
				triangles.push_back(Triangle(Point3(x_up*(1-j/(double)t2),-0.5+j/(double)t2,z_up*(1-j/(double)t2)),
											Point3(0.0,0.5,0.0),
											Point3(x_down*(1-j/(double)t2),-0.5+j/(double)t2,z_down*(1-j/(double)t2)),
											v_side1,v_top,v_side2));
			else{
				triangles.push_back(Triangle(Point3(x_up*(1-j/(double)t2),-0.5+j/(double)t2,z_up*(1-j/(double)t2)),
											Point3(x_down*(1-(j+1)/(double)t2),-0.5+(j+1)/(double)t2,z_down*(1-(j+1)/(double)t2)),
											Point3(x_down*(1-j/(double)t2),-0.5+j/(double)t2,z_down*(1-j/(double)t2)),
											v_side1,v_side2,v_side2));

				triangles.push_back(Triangle(Point3(x_up*(1-j/(double)t2),-0.5+j/(double)t2,z_up*(1-j/(double)t2)),
											Point3(x_up*(1-(j+1)/(double)t2),-0.5+(j+1)/(double)t2,z_up*(1-(j+1)/(double)t2)),
											Point3(x_down*(1-(j+1)/(double)t2),-0.5+(j+1)/(double)t2,z_down*(1-(j+1)/(double)t2)),
											v_side1,v_side1,v_side2));   
			}
		}  
	}
}



Sphere::Sphere(int _t1){
	t1=_t1;
	
	/**************************create the icosahedron********************************/
	double v1=0.85*0.5;
	double v2=sqrt(5)/5*0.5;
	double v3=0.68819*0.5;
	double v4=0.262865*0.5;
	double v5=0.809*0.5;

	//create the vertex and normals
	Point3 point1=Point3(0,0,1*0.5);                   Vector3 vector1=Vector3(0,0,1*0.5);
	Point3 point2=Point3(-v3,0.5*0.5,v2);			   Vector3 vector2=Vector3(-v3,0.5*0.5,v2);
	Point3 point3=Point3(-v3,-0.5*0.5,v2);			   Vector3 vector3=Vector3(-v3,-0.5*0.5,v2);
	Point3 point4=Point3(v4,-v5,v2);				   Vector3 vector4=Vector3(v4,-v5,v2);
	Point3 point5=Point3(v1,0,v2);					   Vector3 vector5=Vector3(v1,0,v2);
	Point3 point6=Point3(v4,v5,v2);					   Vector3 vector6=Vector3(v4,v5,v2);
	Point3 point7=Point3(-v1,0,-v2);				   Vector3 vector7=Vector3(-v1,0,-v2);
	Point3 point8=Point3(-v4,-v5,-v2);				   Vector3 vector8=Vector3(-v4,-v5,-v2);
	Point3 point9=Point3(0,0,-1*0.5);				   Vector3 vector9=Vector3(0,0,-1*0.5);
	Point3 point10=Point3(-v4,v5,-v2);				   Vector3 vector10=Vector3(-v4,v5,-v2);
	Point3 point11=Point3(v3,-0.5*0.5,-v2);			   Vector3 vector11=Vector3(v3,-0.5*0.5,-v2);
	Point3 point12=Point3(v3,0.5*0.5,-v2);			   Vector3 vector12=Vector3(v3,0.5*0.5,-v2);

	//five triangles from vertex point1
	
	triangles.push_back(Triangle(point1,point2,point3,vector1,vector2,vector3));
	triangles.push_back(Triangle(point1,point3,point4,vector1,vector3,vector4));
	triangles.push_back(Triangle(point1,point4,point5,vector1,vector4,vector5));
	triangles.push_back(Triangle(point1,point5,point6,vector1,vector5,vector6));
	triangles.push_back(Triangle(point1,point6,point2,vector1,vector6,vector2));	

	//five triangles from point7
	
	triangles.push_back(Triangle(point7,point3,point2,vector7,vector3,vector2));
	triangles.push_back(Triangle(point7,point2,point10,vector7,vector2,vector10));
	triangles.push_back(Triangle(point7,point10,point9,vector7,vector10,vector9));
	triangles.push_back(Triangle(point7,point9,point8,vector7,vector9,vector8));
	triangles.push_back(Triangle(point7,point8,point3,vector7,vector8,vector3));    

	//five triangles from point11
	
	triangles.push_back(Triangle(point11,point8,point9,vector11,vector8,vector9));
	triangles.push_back(Triangle(point11,point9,point12,vector11,vector9,vector12));
	triangles.push_back(Triangle(point11,point12,point5,vector11,vector12,vector5));
	triangles.push_back(Triangle(point11,point5,point4,vector11,vector5,vector4));
	triangles.push_back(Triangle(point11,point4,point8,vector11,vector4,vector8));   

	//another three triangles from point12
	
	triangles.push_back(Triangle(point12,point9,point10,vector12,vector9,vector10));
	triangles.push_back(Triangle(point12,point10,point6,vector12,vector10,vector6));
	triangles.push_back(Triangle(point12,point6,point5,vector12,vector6,vector5));   

	//last two to connect the whole icosahedron
	
	triangles.push_back(Triangle(point3,point8,point4,vector3,vector8,vector4));
	triangles.push_back(Triangle(point6,point10,point2,vector6,vector10,vector2));   


	/***************************split the triangle*********************************/
	
	if (t1>5)
		t1=5;

	if (t1>1){
		for (int i=1; i<t1; i++){
			std::vector<Triangle> split_tr;
			int num=triangles.size();
			for (int j=0; j<num; j++){
				Triangle tr= triangles[j];
				//create the midpoint
				Point3 point12= Point3 (0.5*(tr.p1[0]+tr.p2[0]),0.5*(tr.p1[1]+tr.p2[1]),0.5*(tr.p1[2]+tr.p2[2]));
				Point3 point23= Point3 (0.5*(tr.p2[0]+tr.p3[0]),0.5*(tr.p2[1]+tr.p3[1]),0.5*(tr.p2[2]+tr.p3[2]));
				Point3 point13= Point3 (0.5*(tr.p1[0]+tr.p3[0]),0.5*(tr.p1[1]+tr.p3[1]),0.5*(tr.p1[2]+tr.p3[2]));
				//create the midpoint normal
				Vector3 vector12= Vector3 (0.5*(tr.p1[0]+tr.p2[0]),0.5*(tr.p1[1]+tr.p2[1]),0.5*(tr.p1[2]+tr.p2[2]));
				Vector3 vector23= Vector3 (0.5*(tr.p2[0]+tr.p3[0]),0.5*(tr.p2[1]+tr.p3[1]),0.5*(tr.p2[2]+tr.p3[2]));
				Vector3 vector13= Vector3 (0.5*(tr.p1[0]+tr.p3[0]),0.5*(tr.p1[1]+tr.p3[1]),0.5*(tr.p1[2]+tr.p3[2]));
				Vector3 vector11= Vector3 (tr.p1[0],tr.p1[1],tr.p1[2]);
				Vector3 vector22= Vector3 (tr.p2[0],tr.p2[1],tr.p2[2]);
				Vector3 vector33= Vector3 (tr.p3[0],tr.p3[1],tr.p3[2]);
				//draw the split triangles
				split_tr.push_back(Triangle(point13,tr.p1,point12,vector13,vector11,vector12));
				split_tr.push_back(Triangle(point23,point12,tr.p2,vector23,vector12,vector22));
				split_tr.push_back(Triangle(point23,point13,point12,vector23,vector13,vector12));
				split_tr.push_back(Triangle(tr.p3,point13,point23,vector33,vector13,vector23));

			}
			//interchange the split triangles with unsplit one
			triangles.swap(split_tr);
		}
	}
	icotoSphere();   
} 


void Sphere::icotoSphere(){
	std::vector <Triangle> sphere_tr;
	int length= triangles.size();
	for (int i=0; i<length;i++){
		//put the three vertexs and normals into array
		Triangle tr= triangles[i];
		Point3 point[3]={tr.p1,tr.p2,tr.p3};
		Vector3 vector[3];

		for (int j=0; j<3;j++){
			double side=sqrt(point[j][0]*point[j][0]+point[j][1]*point[j][1]+point[j][2]*point[j][2]);
			point[j][0]= point[j][0]*0.5/side;
			point[j][1]= point[j][1]*0.5/side;
			point[j][2]= point[j][2]*0.5/side;

			vector[j]=Vector3(point[j][0],point[j][1],point[j][2]);
		}

		sphere_tr.push_back(Triangle(point[0],point[1],point[2],vector[0],vector[1],vector[2]));
	}
	triangles.swap(sphere_tr);
}


Torus::Torus(int _t1, int _t2){
	t1=_t1;
	t2=_t2;
	
	double R=0.5;
	double r=R/2;
	if(t1<3)
		t1=3;
	if(t2<3)
		t2=3;
	
	for (int i=0;i<t1;i++){
		for (int j=0;j<t2;j++){
			double ai_up=cos(2*M_PI*i/(double)t1);
			double ai_down=cos(2*M_PI*(i+1)/(double)t1);
			double bi_up=sin(2*M_PI*i/(double)t1);
			double bi_down=sin(2*M_PI*(i+1)/(double)t1);

			double aj_up=cos(2*M_PI*j/(double)t2);
			double aj_down=cos(2*M_PI*(j+1)/(double)t2);
			double bj_up=sin(2*M_PI*j/(double)t2);
			double bj_down=sin(2*M_PI*(j+1)/(double)t2);

			double x1=(R+r*aj_up)*ai_up, y1=(R+r*aj_up)*bi_up, z1=r*bj_up;
			double x2=(R+r*aj_down)*ai_up, y2=(R+r*aj_down)*bi_up, z2=r*bj_down;
			double x3=(R+r*aj_up)*ai_down, y3=(R+r*aj_up)*bi_down, z3=r*bj_up;
			double x4=(R+r*aj_down)*ai_down, y4=(R+r*aj_down)*bi_down, z4=r*bj_down;

			triangles.push_back(Triangle(Point3(x2,y2,z2),Point3(x1,y1,z1),Point3(x3,y3,z3),
								Vector3(x2,y2,z2),Vector3(x1,y1,z1),Vector3(x3,y3,z3)));
			triangles.push_back(Triangle(Point3(x4,y4,z4),Point3(x2,y2,z2),Point3(x3,y3,z3),
								Vector3(x4,y4,z4),Vector3(x2,y2,z2),Vector3(x3,y3,z3)));

		}
	}

}

Corkscrew::Corkscrew(int _t1, int _t2){
	t1=_t1;
	t2=_t2;

	double R=0.5/2;
	double r=0.5/5;
	int n=6;

	for (int i=0; i<n*t1;i++){
		for (int j=0; j<t2; j++){
			double ai_up=cos(2*M_PI*i/(double)t1);
			double ai_down=cos(2*M_PI*(i+1)/(double)t1);
			double bi_up=sin(2*M_PI*i/(double)t1);
			double bi_down=sin(2*M_PI*(i+1)/(double)t1);

			double aj_up=cos(2*M_PI*j/(double)t2);
			double aj_down=cos(2*M_PI*(j+1)/(double)t2);
			double bj_up=sin(2*M_PI*j/(double)t2);
			double bj_down=sin(2*M_PI*(j+1)/(double)t2);

			double x1=(R+r*aj_up)*ai_up, y1=(R+r*aj_up)*bi_up, z1=0.3*i/t1+r*bj_up;
			double x2=(R+r*aj_down)*ai_up, y2=(R+r*aj_down)*bi_up, z2=0.3*i/t1+r*bj_down;
			double x3=(R+r*aj_up)*ai_down, y3=(R+r*aj_up)*bi_down, z3=0.3*(i+1)/t1+r*bj_up;
			double x4=(R+r*aj_down)*ai_down, y4=(R+r*aj_down)*bi_down, z4=0.3*(i+1)/t1+r*bj_down;

			triangles.push_back(Triangle(Point3(x2,y2,z2),Point3(x1,y1,z1),Point3(x3,y3,z3),
								Vector3(x2,y2,z2),Vector3(x1,y1,z1),Vector3(x3,y3,z3)));
			triangles.push_back(Triangle(Point3(x4,y4,z4),Point3(x2,y2,z2),Point3(x3,y3,z3),
								Vector3(x4,y4,z4),Vector3(x2,y2,z2),Vector3(x3,y3,z3)));
		}
	}

}  

Banana::Banana(int _t1, int _t2){
	t1=_t1;
	t2=_t2;

	fstream file;
	//file.open("banana_low.m",ios::in);

}



HitRecord Shape::intersect(const Point3& point, Vector3& distance){
	HitRecord record=HitRecord();
	return record;
}

HitRecord Cube::intersect(const Point3& point, Vector3& distance){
	HitRecord record=HitRecord();
	double t; Point3 hp;

	//positive x axis
	t=(0.5-point[0])/distance[0];
	hp=point+t*distance;
	if (t>=0 && hp[1]>=-0.5 && hp[1]<=0.5 && hp[2]>=-0.5 && hp[2]<=0.5){
		record.addHit(t,0.0,0.0,hp,Vector3(1.0,0.0,0.0));
	}

	//negative x axis
	t=(0.5+point[0])/-distance[0];
	hp=point+t*distance;
	if (t>=0 && hp[1]>=-0.5 && hp[1]<=0.5 && hp[2]>=-0.5 && hp[2]<=0.5){
		record.addHit(t,0.0,0.0,hp,Vector3(-1.0,0.0,0.0));
	}

	//positive y axis
	t=(0.5-point[1])/distance[1];
	hp=point+t*distance;
	if (t>=0 && hp[0]>=-0.5 && hp[0]<=0.5 && hp[2]>=-0.5 && hp[2]<=0.5){
		record.addHit(t,0.0,0.0,hp,Vector3(0.0,1.0,0.0));
	}

	//negative y axis
	t=(0.5+point[1])/-distance[1];
	hp=point+t*distance;
	if (t>=0 && hp[0]>=-0.5 && hp[0]<=0.5 && hp[2]>=-0.5 && hp[2]<=0.5){
		record.addHit(t,0.0,0.0,hp,Vector3(0.0,-1.0,0.0));
	}

	//positive z axis
	t=(0.5-point[2])/distance[2];
	hp=point+t*distance;
	if (t>=0 && hp[0]>=-0.5 && hp[0]<=0.5 && hp[1]>=-0.5 && hp[1]<=0.5){
		record.addHit(t,0.0,0.0,hp,Vector3(0.0,0.0,1.0));
	}

	//negative z axis
	t=(0.5+point[2])/-distance[2];
	hp=point+t*distance;
	if (t>=0 && hp[0]>=-0.5 && hp[0]<=0.5 && hp[1]>=-0.5 && hp[1]<=0.5){
		record.addHit(t,0.0,0.0,hp,Vector3(0.0,0.0,-1.0));
	}

	return record;
}

HitRecord Cylinder::intersect(const Point3& point, Vector3& distance){
	HitRecord record=HitRecord();
	double a,b,c, top,bottom, t1,t2,delta;
	Vector3 hv;
	Point3 hp;

	a= distance[0]*distance[0]+distance[2]*distance[2];
	b= 2*(point[0]*distance[0]+point[2]*distance[2]);
	c= point[0]*point[0]+point[2]*point[2]-0.25;

	delta=b*b-4*a*c;
	t1= (-b + sqrt(delta))/(2*a);
	t2= (-b - sqrt(delta))/(2*a);

	//for the top and bottom
	top= (-0.5+point[1])/-distance[1];
	Point3 top_hp=point+top*distance;
	if (top>=0 && top_hp[0]*top_hp[0]+top_hp[2]*top_hp[2]<=0.25){
		record.addHit(top,0.0,0.0,top_hp,Vector3(0.0,1.0,0.0));
	}
	bottom=(0.5+point[1])/-distance[1];
	Point3 bottom_hp=point+bottom*distance;
	if (bottom>=0 && bottom_hp[0]*bottom_hp[0]+bottom_hp[2]*bottom_hp[2]<=0.25){
		record.addHit(bottom,0.0,0.0,bottom_hp,Vector3(0.0,-1.0,0.0));
	}

	if (delta<0)
		return record;

	if (t1>0){
		if (t2>0){
			hp=point+distance*t1;
			if (hp[1]>=-0.5 && hp[1]<=0.5){
				hv = Vector3(hp[0],0.0,hp[2]); hv.normalize();
				record.addHit(t1,0.0,0.0,hp,hv);
			}
			hp=point+distance*t2;
			if (hp[1]>=-0.5 && hp[1]<=0.5){
				hv = Vector3(hp[0],0.0,hp[2]); hv.normalize();
				record.addHit(t2,0.0,0.0,hp,hv);
			}
		}
		else {
			//only return t1
			hp=point+distance*t1;
			if (hp[1]>=-0.5 && hp[1]<=0.5){
				hv = Vector3(hp[0],0.0,hp[2]); hv.normalize();
				record.addHit(t1,0.0,0.0,hp,hv);
			}			
		}
	}
	else if (t1<0)
		return record;
	else{
		hp=point+distance*t1;
			if (hp[1]>=-0.5 && hp[1]<=0.5){
				hv = Vector3(hp[0],0.0,hp[2]); hv.normalize();
				record.addHit(t1,0.0,0.0,hp,hv);
			}	
	}

	return record;
}

HitRecord Cone::intersect(const Point3& point, Vector3& distance){
	HitRecord record=HitRecord();
	double a,b,c, top,bottom, t1,t2,delta;
	Vector3 hv;
	Point3 hp;

	a= distance[0]*distance[0]+distance[2]*distance[2]-0.25*distance[1]*distance[1];
	b= 2*(point[0]*distance[0]+point[2]*distance[2])+0.5*distance[1]*(0.5-point[1]);
	c= point[0]*point[0]+point[2]*point[2]-0.25*(0.5-point[1])*(0.5-point[1]);

	delta=b*b-4*a*c;
	t1= (-b + sqrt(delta))/(2*a);
	t2= (-b - sqrt(delta))/(2*a);

	//for the bottom
	bottom=(0.5+point[1])/-distance[1];
	Point3 bottom_hp=point+bottom*distance;
	if (bottom>=0 && bottom_hp[0]*bottom_hp[0]+bottom_hp[2]*bottom_hp[2]<=0.25){
		record.addHit(bottom,0.0,0.0,bottom_hp,Vector3(0.0,-1.0,0.0));
	}

	if (delta<0)
		return record;

	if (t1>0){
		if (t2>0){
			hp=point+distance*t1;
			if (hp[1]>=-0.5 && hp[1]<=0.5){
				hv = Vector3(hp[0],0.5*sqrt(hp[0]*hp[0]+hp[2]*hp[2]),hp[2]); hv.normalize();
				record.addHit(t1,0.0,0.0,hp,hv);
			}
			hp=point+distance*t2;
			if (hp[1]>=-0.5 && hp[1]<=0.5){
				hv = Vector3(hp[0],0.5*sqrt(hp[0]*hp[0]+hp[2]*hp[2]),hp[2]); hv.normalize();
				record.addHit(t2,0.0,0.0,hp,hv);
			}
		}
		else {
			//only return t1
			hp=point+distance*t1;
			if (hp[1]>=-0.5 && hp[1]<=0.5){
				hv = Vector3(hp[0],0.5*sqrt(hp[0]*hp[0]+hp[2]*hp[2]),hp[2]); hv.normalize();
				record.addHit(t1,0.0,0.0,hp,hv);
			}			
		}
	}
	else if (t1<0)
		return record;
	else{
		hp=point+distance*t1;
			if (hp[1]>=-0.5 && hp[1]<=0.5){
				hv = Vector3(hp[0],0.5*sqrt(hp[0]*hp[0]+hp[2]*hp[2]),hp[2]); hv.normalize();
				record.addHit(t1,0.0,0.0,hp,hv);
			}	
	}

	return record;
}

HitRecord Sphere::intersect(const Point3& point, Vector3& distance){
	HitRecord record = HitRecord();
	double a,b,c, top,bottom, t1,t2,delta;
	Vector3 hv;
	Point3 hp;

	a= distance[0]*distance[0]+distance[2]*distance[2]+distance[1]*distance[1];
	b= 2*(point[0]*distance[0]+point[2]*distance[2]+point[1]*distance[1]);
	c= point[0]*point[0]+point[2]*point[2]+point[1]*point[1]-0.25;

	delta=b*b-4*a*c;
	t1= (-b + sqrt(delta))/(2*a);
	t2= (-b - sqrt(delta))/(2*a);

	if (delta<0)
		return record;

	if (t1>0){
		if (t2>0){
			hp=point+distance*t1;
			hv = Vector3(hp[0],hp[1],hp[2]); hv.normalize();
			record.addHit(t1,0.0,0.0,hp,hv);
			
			hp=point+distance*t2;
			hv = Vector3(hp[0],hp[1],hp[2]); hv.normalize();
			record.addHit(t2,0.0,0.0,hp,hv);
			
		}
		else {
			//only return t1
			hp=point+distance*t1;
			hv = Vector3(hp[0],hp[1],hp[2]); hv.normalize();
			record.addHit(t1,0.0,0.0,hp,hv);		
		}
	}
	else if (t1<0)
		return record;
	else{
		hp=point+distance*t1;
		hv = Vector3(hp[0],hp[1],hp[2]); hv.normalize();
		record.addHit(t1,0.0,0.0,hp,hv);
		
	}

	return record;

}

HitRecord Torus::intersect(const Point3& point, Vector3& distance){
	HitRecord record= HitRecord(); 	
	double R=0.5, r=R/2;
	double a,b,c, top,bottom, t1,t2,delta;
	Vector3 hv;
	Point3 hp;

	a= distance[0]*distance[0]+distance[2]*distance[2]+distance[1]*distance[1];
	b= 2*(point[0]*distance[0]+point[2]*distance[2]+point[1]*distance[1]);
	c= point[0]*point[0]+point[2]*point[2]+point[1]*point[1]-r*r-R*R;

	double a4= (distance[0]*distance[0]+distance[2]*distance[2]+distance[1]*distance[1])*(distance[0]*distance[0]+distance[2]*distance[2]+distance[1]*distance[1]);
	double a3= 4*(distance[0]*distance[0]+distance[2]*distance[2]+distance[1]*distance[1])*(point[0]*distance[0]+point[2]*distance[2]+point[1]*distance[1]);
	double a2= 4*(distance[0]*distance[0]+distance[2]*distance[2]+distance[1]*distance[1])*(point[0]*distance[0]+point[2]*distance[2]+point[1]*distance[1])*(distance[0]*distance[0]+distance[2]*distance[2]+distance[1]*distance[1])*(point[0]*distance[0]+point[2]*distance[2]+point[1]*distance[1])
		+2*(distance[0]*distance[0]+distance[2]*distance[2]+distance[1]*distance[1])*(point[0]*point[0]+point[2]*point[2]+point[1]*point[1])-r*r-R*R+4*R*R*distance[1]*distance[1];
	

	delta=b*b-4*a*c;
	t1= (-b + sqrt(delta))/(2*a);
	t2= (-b - sqrt(delta))/(2*a);

	if (delta<0)
		return record;

	if (t1>0){
		if (t2>0){
			hp=point+distance*t1;
			hv = Vector3(hp[0],hp[1],hp[2]); hv.normalize();
			record.addHit(t1,0.0,0.0,hp,hv);
			
			hp=point+distance*t2;
			hv = Vector3(hp[0],hp[1],hp[2]); hv.normalize();
			record.addHit(t2,0.0,0.0,hp,hv);
			
		}
		else {
			//only return t1
			hp=point+distance*t1;
			hv = Vector3(hp[0],hp[1],hp[2]); hv.normalize();
			record.addHit(t1,0.0,0.0,hp,hv);		
		}
	}
	else if (t1<0)
		return record;
	else{
		hp=point+distance*t1;
		hv = Vector3(hp[0],hp[1],hp[2]); hv.normalize();
		record.addHit(t1,0.0,0.0,hp,hv);
		
	}

	return record;

}


HitRecord Corkscrew::intersect(const Point3& point, Vector3& distance){
	HitRecord record= HitRecord(); 	

	return record;
}
