#include "../cse452.h"
#include "Camera.h"
#include <cmath>
#include <FL/Fl.H>
using namespace std;

Camera::Camera() 
{
    // initialize your data here   
    T = Matrix4::identity();
	T_inv = Matrix4::identity();
	world2Camera = Matrix4::identity(); 
	camera2World = Matrix4::identity(); 
	R = Matrix4::identity(); 
	R_inv = Matrix4::identity(); 
	Sxyz = Matrix4::identity();
	Sxyz_inv = Matrix4::identity(); 
	Sxy = Matrix4::identity(); 
	Sxy_inv = Matrix4::identity(); 
	D = Matrix4::identity(); 
	pro = Matrix4::identity(); 

	P=Point3(0.0,0.0,0.0);

	L=Vector3(0,0,-1);
	U=Vector3(0,1,0);

	dn=0.0,df=1.0;

	skew=0.0;
	aspraScale=1.0;
	centerPro=Point3(0.0,0.0,0.0);
}

Camera::~Camera() {
    // destroy your data here
}

// The following three should be unit length and orthogonal to each other
// u vector
Vector3 Camera::getRight() const
{
    // Change this
    return u;
}

// v vector
Vector3 Camera::getUp() const
{
    // Change this
    return v;
}

// - n vector
Vector3 Camera::getLook() const
{
    // Change this
    return -n;
}

double Camera::getSkew() const
{
    // Change this to implement the extra credit
    return skew;
}

double Camera::getAspectRatioScale() const
{
    // Change this to implement the extra credit
	return aspraScale;
}

Point3 Camera::getProjectionCenter() const
{
    // Change this to implement the extra credit
	return centerPro;
}

Matrix4 Camera::getProjection() const {
    // return the current projection and scale matrix

    // Change this
    return pro;
}

Matrix4 Camera::getWorldToCamera() const {
    // return the current world to camera matrix
    // Rotation and translation

    // Change this
    return world2Camera;
}

Matrix4 Camera::getRotationFromXYZ() const
{
    // return just the rotation matrix

    // Change this
    return R;
}

Matrix4 Camera::getRotationToXYZ() const
{
    // return just the rotation matrix

    // Change this
    return R_inv;
}

Matrix4 Camera::getCameraToWorld() const {
    // return the current camera to world matrix
    // This is the inverse of the rotation, translation, and scale

    // Change this
    return camera2World;
}

int Camera::getWidth()  const{
    // return the current image width

    // Change this
    return width;
}

int Camera::getHeight()  const{
    // return the current image height

    // Change this
    return height;
}

Point3 Camera::getEye()  const{
    // return the current eye location

    // Change this
    return P;
}

double Camera::getZoom() const
{
    // Change this
    return zoom;
}

void Camera::setFrom(const Point3& from) {
    // set the current viewpoint (eye point)
    P=from;
    
	//get translation function
	translation_cal();
	//world to camera
	worldtocam();
	//camera to world
	camtoworld();
}

void Camera::setAt(const Point3& at) {
    // set the point the camera is looking at
    // calling this requires that the from (or eye) point already be valid
    L=at-P;
    
	//set the three vectors u,v,n
	camera_uvn();
	rotation_cal();
	//world to camera
	worldtocam();
	//camera to world
	camtoworld();
}

void Camera::setLook(const Vector3& l) {
    // set the direction the camera is looking at
    L= l;
    Right=L^U;
    
	//set the three vectors u,v,n
	camera_uvn();
	rotation_cal();
	//world to camera
	worldtocam();
	//camera to world
	camtoworld();
}

void Camera::setUp(const Vector3& up) {
    // set the upwards direction of the camera
    U=up;
	Right = L^U;
	
    //set the three vectors u,v,n
	camera_uvn();
	rotation_cal();
	//world to camera
	worldtocam();
	//camera to world
	camtoworld();
}

void Camera::setWidthHeight(int w, int h) {
    // set the current width and height of the film plane
    width=w; 
	height=h;
    
    scalexy_cal();
    project();
	camtoworld();
}

void Camera::setZoom(double z) {
    // set field of view (specified in degrees)
    zoom = z;
    
    scalexy_cal();
    project();
	camtoworld();
}

void Camera::setNearFar(double n, double f) {
    // set the near and far clipping planes
    dn=n;
	df=f;
    
    scalexyz_cal();
	perspective();
	project();
	camtoworld();
}

void Camera::setSkew( double d )
{
	skew=d;
	perspective();
	project();
}

void Camera::setAspectRatioScale( double d )
{
	aspraScale=d;
	perspective();
	project();
}

void Camera::setProjectionCenter( const Point3 &p )
{
	centerPro=p;
	perspective();
	project();
}

void Camera::moveForward(double dist) {
    // move the camera forward (in the viewing direction)
    // by the amount dist
    P+=dist*(-n);
	translation_cal();
	worldtocam();
	camtoworld();
}

void Camera::moveSideways(double dist) {
    // move the camera sideways (orthogonal to the viewing direction)
    // by the amount dist
    P+=dist*u;
	translation_cal();
	worldtocam();
	camtoworld();
}

void Camera::moveVertical(double dist) {
    // move the camera vertically (along the up vector)
    // by the amount dist
    P+=dist*v;
	translation_cal();
	worldtocam();
	camtoworld();
}

void Camera::rotateYaw(double angle) {
    // rotate the camera left/right (around the up vector)
    Matrix4 r=Matrix4::rotation(v,angle);
	L=r*L;
	Right=L^U;

	camera_uvn();
	rotation_cal();
	worldtocam();
	camtoworld();
}

void Camera::rotatePitch(double angle) {
    // rotate the camera up/down (pitch angle)
	Matrix4 r=Matrix4::rotation(u,angle);
	L=r*L;
	Right=L^U;

	camera_uvn();
	rotation_cal();
	worldtocam();
	camtoworld();
}

void Camera::rotateAroundAtPoint(int axis, double angle, double focusDist) {
    // Rotate the camera around the right (0), up (1), or look (2) vector
    // around the point at eye + look * focusDist
	Point3 P1=P+L*focusDist;
	Point3 P11=Point3(-P1[0],-P1[1],-P1[2]);
	Matrix4 ma=Matrix4::translation(P11);
	Matrix4 I_BAR;
	Matrix4 Trans;

	if(axis==0)
		I_BAR=Matrix4::rotation(u,angle);
	else if (axis==1)
		I_BAR=Matrix4::rotation(v,angle);
	else
		I_BAR=Matrix4::rotation(n,angle);

	Matrix4 ma_inv= Matrix4::translation(P1);
	Trans=ma_inv*R_inv*I_BAR*R*ma;

	P=Trans*P;
	U=Trans*U;
	L=Trans*L;
	Right=U^L;

	camera_uvn();
	translation_cal();
	rotation_cal();
	worldtocam();
	camtoworld();
}



void Camera::moveKeyboard( )
{
    // you may change key controls for the interactive
    // camera controls here, make sure you document your changes
    // in your README file

	if (Fl::event_key('w'))
        moveForward(+0.05);
    if (Fl::event_key('s'))
        moveForward(-0.05);
    if (Fl::event_key('a'))
        moveSideways(-0.05);
    if (Fl::event_key('d'))
        moveSideways(+0.05);
    if (Fl::event_key(FL_Up))
        moveVertical(+0.05);
    if (Fl::event_key(FL_Down))
        moveVertical(-0.05);
    if (Fl::event_key(FL_Left))
        rotateYaw(+0.05);
    if (Fl::event_key(FL_Right))
        rotateYaw(-0.05);
    if (Fl::event_key(FL_Page_Up))
        rotatePitch(+0.05);
    if (Fl::event_key(FL_Page_Down))
        rotatePitch(-0.05);

}

void Camera::camera_uvn(){
	n=Vector3(-L[0],-L[1],-L[2]); 
	n.normalize();
	v=U-(U*n)*n; 
	v.normalize();
	u = v^n; 
	u.normalize();
}

void Camera::worldtocam(){
	world2Camera=R*T;
}
void Camera::camtoworld(){
	camera2World=T_inv*R_inv*Sxy_inv*Sxyz_inv;
}

void Camera::rotation_cal(){
	R = Matrix4::rotation(u,v,n);
	R_inv=R.transpose();
}

void Camera::translation_cal(){
	Point3 p_eye= Point3(-P[0],-P[1],-P[2]);
	T= Matrix4::translation(p_eye);
	T_inv=Matrix4::translation(P);
}

void Camera::scalexy_cal(){
	double alpha=width/(double)height;
	double theta_h=zoom*M_PI/180;
	double theta_w=2*atan(tan(theta_h/2)*alpha);
		
	Sxy=Matrix4::scaling(1./(tan(theta_w/2)),1./(tan(theta_h/2)),1.0);
	Sxy_inv=Matrix4::scaling(tan(theta_w/2),tan(theta_h/2),1.0);
}


void Camera::scalexyz_cal(){
	Sxyz=Matrix4::scaling(1./df,1./df,1./df);
	Sxyz_inv=Matrix4::scaling(df,df,df);
}


void Camera::perspective(){
	k=dn/df;
	D= Matrix4(Vector4(1,0,0,0),Vector4(0,1,0,0),Vector4(0,0,1/(k-1),k/(k-1)),Vector4(0,0,-1,0));
}

void Camera::project(){
	pro=D*Sxyz*Sxy;
}
