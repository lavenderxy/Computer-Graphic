#ifndef _MY_CAMERA_H_
#define _MY_CAMERA_H_

#include "cse452.h"
#include "Matrix4.h"

// you must implement all of the following methods

class Camera {
public:
    Camera();
    ~Camera();
    
    // Perspective plus scale (x,y, and z)
    Matrix4 getProjection() const;

    // Rotation and translation from world to camera
    Matrix4 getWorldToCamera() const;
    // Rotation, translation and scale from camera to world
    Matrix4 getCameraToWorld() const;
    // Just rotation from x,y,z axes to u,v,n
    Matrix4 getRotationFromXYZ() const;
    // Just rotation from u,v,n to x,y,z axes (canonical)
    Matrix4 getRotationToXYZ() const;

    // screen width/height
    int getWidth() const;
    int getHeight() const;

    // Camera orientation, position _after_ normalization
    Point3  getEye() const;
    // These should be unit length and orthogonal to each other
    // u vector
    Vector3 getRight() const;
    // v vector
    Vector3 getUp() const;
    // -n vector
    Vector3 getLook() const;

    // Perspective data
    Point3 getProjectionCenter() const;//****extra
    double getZoom() const;//****extra
    double getSkew() const;//****extra
    // "Squishing" in x,y (not Width/Heigh)
    double getAspectRatioScale() const;//****extra

    // For setting camera
    void setFrom(const Point3& from);
    void setAt(const Point3& at);
    void setLook(const Vector3& l);
    void setUp(const Vector3& up);
    void setWidthHeight(int w, int h);
    void setZoom(double z);
    void setNearFar(double n, double f);
    void setProjectionCenter( const Point3 &in_pt );//****extra
    // Extra "squishing" in x,y (not Width/Height)
    void setAspectRatioScale( double );//****extra
    void setSkew( double );//****extra

    // This is what gets called when a key is pressed
    void moveKeyboard();

    // user interaction methods
    // These rotate the camera around itself
    void moveForward(double dist);
    void moveSideways(double dist);
    void moveVertical(double dist);
    void rotateYaw(double angle);
    void rotatePitch(double angle);

    // IBar: This rotates the camera around a point at a distance focusDist
    // from the eye point of the camera. If axis is 0, it rotates the camera itself
    // around the Right vector, 1 is around the up axis, and 2 is around the look vector
    void rotateAroundAtPoint(int axis, double angle, double focusDist);

private:
    // declare your variables here:

    Point3 P; //declare the eye point
	Matrix4 T;//declare the translate function
	Matrix4 T_inv; // declare the inverse of translate function
	Matrix4 world2Camera; //declare the world to camera functin
	Matrix4 camera2World; //declare the camera to world function
	Matrix4 R; //declare the rotation matrix
	Matrix4 R_inv; //declare the inverse of rotation matrix
	Matrix4 Sxyz; //declare the scale function
	Matrix4 Sxyz_inv; //declare the inverse of scale function
	Matrix4 Sxy; //declare the scale function
	Matrix4 Sxy_inv; //declare the inverse of scale function
	Matrix4 D; //declare the perspective transformation
	Matrix4 pro; //declare the projection matrix

	Vector3 u,v,n; //declare three camera vectors
	Vector3 L; //declare look vector
	Vector3 U; //declare up vector
	Vector3 Right; //declare right vector

	double df,dn;
	double width,height;
	double zoom;
	double k;

	void camera_uvn();         //calculate the three camera vectors u,v,n
	void camtoworld();         //calculate camera to world matrix 
	void worldtocam();         //calculate world to camera matrix
	void rotation_cal();       //calculate rotate matrix R and inverse rotate matrix R_inv 
	void translation_cal();    //calculate translate matrix T and inverse translate matrix T_inv 
	void scalexy_cal();        //calculate scalexy matrix Sxy and inverse scalexy matrix Sxy_inv
	void scalexyz_cal();       //calculate scalexyz matrix Sxyz and inverse scalexyz matrix Sxyz_inv
	void perspective();        //calculate perspective transformation matrix D
	void project();            //calculate projection matrix 
	 
	double skew;
	double aspraScale;
	Point3 centerPro;

};

#endif /* _MY_CAMERA_H_ */
