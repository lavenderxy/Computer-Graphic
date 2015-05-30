#include "../cse452.h"
#include "../sceneview/MyScene.h"
#include "RenderingInterface.h"
#include <FL/gl.h>
#include <cfloat>

void MyScene::render(int type, int width, int height, unsigned char* pixels) {
    if (!isLoaded) {
        return;
    }

    // Add your rendering code here.
    // Keep track of your progress as a value between 0 and 1
    // so the progress bar can update as the rendering progresses
	
	//the valuable is defined in MyScene.h
	isRender=true;
	progressRender=0.0;
	widthRender=width;
	heightRender=height;

    switch (type) {
        case RenderingUI::RENDER_SCANLINE:  
			//just render pixels
			for (int j=0;j<height && isRender;j++){
				for (int i=0;i<width;i++){
					pixelSet(i,j,width, height-j-1,background,pixels);			
				}
				progressRender = (double)(j+1)/heightRender;
				Fl::check();
			}
			break;
        case RenderingUI::RENDER_RAY_TRACING:  
			rayTracingRend(width, height, pixels);
			break;
        case RenderingUI::RENDER_PATH_TRACING:  
			//just render pixels
			for (int j=0;j<height && isRender;j++){
				for (int i=0;i<width;i++){
					pixelSet(i,j,width, height-j-1,background,pixels);			
				}
				progressRender = (double)(j+1)/heightRender;
				Fl::check();
			}
			break;
        default: break;
    }
}

void MyScene::stopRender()
{
    // Because this is threaded code, this function
    // can be called in the middle of your rendering code.
    // You should then stop at the next scanline
	isRender=false;
}

double MyScene::getRenderProgress() {
    // return the current progress as a value between 0 and 1
	return progressRender;
}

// add extra methods here

//set the pixel in screen
void MyScene::pixelSet(int x, int y, int w, int h, const Color &col, unsigned char* pixel){
	const int i=((heightRender-y-1)*widthRender+x)*3;
	pixel[i] = (unsigned char) (col[0]*255.0f);
	pixel[i+1] = (unsigned char) (col[1]*255.0f);
	pixel[i+2] = (unsigned char) (col[2]*255.0f);
}

//find the first and positive intersection with object
void MyScene::rayIntersect(Point3 point, Vector3 distance, Point3& hitP, Vector3& hitN, MatrixObj*& hitObject){
	double tmin=DBL_MAX, t, u,v;
	Point3 hitPoint;
	Vector3 hitNormal;
	HitRecord hitObj;

	Matrix4 mat, matInv, matHit, matInvHit;
	Object *object;

	for (list<MatrixObj>::iterator it = objList.begin(); it !=objList.end(); it++){
		mat= it->matrixOrigin;
		matInv= mat.inverse();
		object= it->objectOrigin;

		hitObj =  object->shapeGet()->intersect(matInv*point, matInv*distance);
		hitObj.sortHits();

		//get the first one of hit
		if (hitObj.getFirstHit(t,u,v,hitPoint,hitNormal)){
			if (t>0 && t<tmin){
				tmin=t;
				hitP=hitPoint;
				hitN=hitNormal;
				hitObject = &(*it);
				matHit = mat;
				matInvHit = matInv;
			}
		}
			
	}

	hitP = matHit * hitP;
	hitN = matHit * hitN;
	hitN.normalize();
}

Color MyScene::illuminate(Point3 point, Vector3 ray, int level){
	Color colorSum=Color();
	Color ambientCol=Color();
	Color diffuseCol=Color(); 
	Color specularCol=Color();
	Color reflectCol=Color();
	Color transparentCol=Color();
	bool isRendering; //set a bool value, true=render now, false=not render now

	MatrixObj* object=0;
	Point3 hitP;
	Vector3 hitN;

	rayIntersect(point,ray,hitP,hitN,object);
	
	isRendering=true;
	if(hitN*ray>0)
		isRendering=false;

	if(object !=0 && isRendering){

		Object*obj =object->objectOrigin;

		//set ambient color
		ambientCol = ambIllum(object);

		//set diffused and speculate color
		for (std::vector<Light>::iterator light = lights.begin(); light != lights.end(); light++){
			diffuseCol +=diffIllum (object, hitP, hitN, &(*light));
			specularCol +=specIllum (object, ray, hitP, hitN, &(*light));
		}

		//set the reflective color
		//Object *obj = object->objectOrigin;
		Color reflect = obj ->reflectGet();

		if (level>0 && reflect.getMax()>0){
			Vector3 rayNow=ray;
			Color colrefNow=Color();
			rayNow = 2*(rayNow*hitN)*hitN-rayNow;
			rayNow.normalize();
			colrefNow=illuminate(hitP+rayNow,rayNow, level-1);
			reflectCol=colrefNow*reflect;
		}

		//set the transparent color
		Color transparent =obj->transparentGet();
		
		if (level>0 && reflect.getMax()>0){
			Color coltranNow=Color();
			coltranNow=illuminate(hitP+ray,ray,level-1);
			transparentCol=coltranNow*transparent;
		}

		//calculate the sum of color
		colorSum=reflectCol+specularCol+diffuseCol+ambientCol;
		//normalize color
		float maxvalue=1;
		for (int i=0;i<3;i++){
			if (colorSum[i]<0)
				colorSum[i]=0;
			else{
				if(colorSum[i]>maxvalue)
					maxvalue=colorSum[i];
			}
		}

		for (int j=0; j<3;j++){
			if (maxvalue>1){
				colorSum[j]=colorSum[j]/maxvalue;
			}
		}	
		return colorSum;
	}
	else
		return background;
}

//ray tracing rendering 
void MyScene::rayTracingRend(int w, int h, unsigned char* pixel){
	Vector3 rayTrace;
	Point3 pointWorld;
	Color col;
	for (int j=0; j<h && isRender; j++){
			for (int i=0; i<w;i++){
				//caculate the point in the world, from camera to warld
				pointWorld = camera.getCameraToWorld() *  Point3((i+0.5)*2/w-1,(1-(j+0.5)*2/h),-1);
				rayTrace=pointWorld-camera.getEye();
				rayTrace.normalize();
				
				col= illuminate(camera.getEye(), rayTrace, RECURSIVE_LIMIT);
				pixelSet(i,j,widthRender,heightRender,col,pixel);
			}
			progressRender = (double)(j+1)/h;
			Fl::check();
	}
}

//calculate color
Color MyScene::ambIllum(MatrixObj *m){
	Color ambcol= m->objectOrigin->ambientGet();
	return ambcol;
}

Color MyScene::diffIllum(MatrixObj* m, Point3 hitP, Vector3 hitN, Light* l){
	Vector3 pointToLight = l->getPos()-hitP;
	double length=pointToLight.length();
	double length1=length*length;

	bool isShadow=false;

	MatrixObj* objNow=0;
	Point3 hitPNow;
	Vector3 hitNNow;
	rayIntersect(hitP+0.001*unit(pointToLight), unit(pointToLight), hitPNow, hitNNow, objNow);
	if (objNow !=0) {
		if((hitPNow - hitP).length() < pointToLight.length())
			isShadow=true;
	}

	if (isShadow == false){
		double attenuate;
		Point3 fallOff=l->getFalloff();

		attenuate = 1./(fallOff[0]+fallOff[1]*length+fallOff[2]*length1);

		double hitLight = hitN * unit(pointToLight);
		Color diffuse=m->objectOrigin-> diffuseGet();
		if (hitLight<0)
			hitLight=0.0;
		return (l->getColor())*diffuse*hitLight*attenuate;
	}
	return Color();

}

Color MyScene::specIllum(MatrixObj* m, Vector3 ray, Point3 hitP, Vector3 hitN, Light*l ){
	
	Vector3 pointToLight = l -> getPos()-hitP;
	double length=pointToLight.length();
	double length1=length*length;

	bool isShadow=false;

	MatrixObj* objNow=0;
	Point3 hitPNow;
	Vector3 hitNNow;
	rayIntersect(hitP+0.001*unit(pointToLight), unit(pointToLight), hitPNow, hitNNow, objNow);
	if (objNow !=0) {
		if((hitPNow - hitP).length() < pointToLight.length())
			isShadow=true;
	}
	
	if (isShadow == false ){
		Object *object = m->objectOrigin;
		Color colLight = l->getColor();

		Vector3 reflectV;
		reflectV = 2*(pointToLight*hitN) * hitN -pointToLight;
		reflectV.normalize();

		double attenuate;
		Point3 fallOff=l->getFalloff();

		attenuate = 1./(fallOff[0]+fallOff[1]*length+fallOff[2]*length1);

		double rayRflect = ray * reflectV;
		double shine=object->shineGet();
		Color spec=object-> specularGet();
		if (rayRflect<0)
			rayRflect=0.0;
		return colLight*spec*attenuate*pow(rayRflect,shine);
	}
	return Color();
} 

