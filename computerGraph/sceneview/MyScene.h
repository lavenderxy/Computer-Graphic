#ifndef _MY_SCENE_H_
#define _MY_SCENE_H_

#include "cse452.h"
#include "Color.h"
#include "camera/Camera.h"
#include "parser.h"
#include "Light.h"
#include <FL/Fl_Image.H>
#include <string>
#include <vector>

/*****************add some support file******************/
#include "../Shapes/Shape.h"
#include <map>

using namespace std; 

/*
 * This class holds all of the data in the scene file.
 *  Camera
 *  Lights
 *  Subgraphs
 *  Root subgraph
 */

// ToDo: Define these

class Object;   //this is the base level of the tree. It stores shape and material information.
class Node;     // A node corresponds to a trans block. It is associated with a transformation matrix and has
			    // either a child object or child tree
class Tree;     // Your scene is composed of trees. A tree is simply a list of nodes

class MatrixObj; //create a matrix store the objects for the transformation matrix
class Transform;   //create transformation class

class MyScene {
public:
    MyScene();
    ~MyScene();

    // The start of the parsing routine
    // Called once when a new scene file is opened
    bool loadSceneFile(std::string filename);

    // Return the current error mesage (if any) generated by parsing
    std::string getErrorMessage() const;

    // Return the parsed camera
    Camera &changeCamera();

    // Return the parsed camera
    const Camera &getCamera() const;

    // Return the parsed camera
    const Color &getBackground() const;

    // Called when the window is re-sized
    void resize(int w, int h);
    // Called when the window needs to be re-drawn
    // Stub can be found in sceneview/MyScene_draw.cpp
    void draw();
    
    /* 
     * The following are for the user interaction assignment 
     * Stubs for them can be found in Interactive/MySceneSelect.cpp
     */
    // User clicked on the screen; find the corresponding shape
    bool selectNode(int x, int y);
    //
    void moveSelectedNode( const Matrix4 &matChange );
    
    /*
     * The following are for the ray tracing assignment
     * Stubs for them can be found in rendering/MyScene_render.cpp
     */
    // 
    void render(int type, int width, int height, unsigned char* pixels);
    void stopRender();
    double getRenderProgress();
   

	//add the value and method which are used in MyScene_render.cpp

	//put pixel when render 
	void pixelSet(int x, int y, int w, int h,const Color &col, unsigned char* pixel);
	//ray intersect with the object
	void rayIntersect(Point3 point, Vector3 distance, Point3& hitP, Vector3& hitN, MatrixObj*& hitObject );
	//raytracing rendering
	void rayTracingRend(int w, int h, unsigned char* pixel);
	
	//calculate ambient color
	Color ambIllum(MatrixObj* m);
	//calculate diffuse color
	Color diffIllum(MatrixObj* m, Point3 hitP, Vector3 hitN, Light* l);
	//calculate speculate color
	Color specIllum(MatrixObj* m, Vector3 ray, Point3 hitP, Vector3 hitN, Light* l);

	Color illuminate(Point3 point, Vector3 distance, int level);

	static const int RECURSIVE_LIMIT=3;
	bool isRender;   //set a bool value, true=render, false=stop render
	int widthRender, heightRender;  //set the width and height for rendering window
	double progressRender;  //set the progress as a value between 0 and 1  

    /*
     * The following are for the sceneview assignment
     * They can be found in sceneview/MyScene.cpp
     */
    // Clear out any existing data
    void resetScene();

    // Gets the background color from the scenefile
    bool parseBackground(Parser& p);

    // Gets the camera info (if any) from the scene file
    bool parseCamera(Parser& p);

    // Gets the light info (if any) from the scene file
    bool parseLight(Parser& p);

    /*
     * The following are the parsing routines you need to finish filling in.
     * Feel free to change the return type or pass in additional parameters.
     */
    // Called when subgraph _name_ [ is encountered in the scene file
    // Should return true when root subgraph encountered
    Tree* parseMasterSubgraph(Parser& p);

    // Called when trans [ is encountered
    Node* parseTrans(Parser& p);

    // Called when Object _name_ [ is encountered
    Object* parseObject(Parser& p);
  	
private:
    // Has loadSceneFile been called yet?
    bool isLoaded;
    // Keep track of the current error message
    std::string errorMessage;
    
    // Store camera info; remember that your default camera should be
    // set to reasonable values in case there is no camera information
    // in the scene file
    Camera camera;

    // Stores the background color found in the scenefile (if any).
    // Defaults to black
    Color background;

    // The stored lights
    Color ambientLight;
    std::vector<Light> lights;

    // your functions/variables go here
    
	list<MatrixObj> objList; // list of  objects in subgraph
	map<string, Tree*> masterSubgraphs;  //keep track of named mastersubgraphs, key is the name of subgraph, map value is tree type
    Shape* cube;
	Shape* cylinder;
	Shape* cone;
	Shape* sphere;
	Shape* torus;
	
	//set a bool value, weather the transformation happen in the subgraph,
	bool transeen;
	//set a bool value, weather there is an object in mastersubgraph
	bool objectseen;  
	//clear subgraph, delete the tree
    void clearMasSub();
	
};

/***************added struct and class****************************/

class MatrixObj{
public:
	MatrixObj(Matrix4 matrixOrigin_, Object* objectOrigin_); 
	~MatrixObj();
	Matrix4 matrixOrigin;
	Object* objectOrigin;
	bool treein;
};

class Node {
public:
	Node();
	~Node();

	virtual void objListmake(Matrix4& m, list<MatrixObj>* l);

protected:
	bool isNode;  //it can't be set as private value, or we can't use it in other class
};

//store shape and surface qualities, the base of a tree
class Object : public Node {
public:
	Object(Object*o);
	Object();  //without shape
	Object(Shape* s);  //with shape
	~Object();
	
	//set object , color and texture
	void drawObject();	
	void objListmake(Matrix4& m, list<MatrixObj>* l);	

	//the ambient color
	void ambientSet(Color col){
		amb=col;
	};
	Color ambientGet(){
		return amb;
	};

	//the diffuse color
	void diffuseSet(Color col){
		diff=col;
	};
	Color diffuseGet(){
		return diff;
	};

	//the  reflected color
	void reflectSet(Color col){
		ref=col;
	};
	Color reflectGet(){
		return ref;
	};

	//the specular color
	void specularSet(Color col){
		spe=col;
	};
	Color specularGet(){
		return spe;
	};

	//the specular exponent
	void shineSet(double s){
		shine=s;
	};
	double shineGet(){
		return shine;
	};


	//the transparency
	void transparentSet(Color col){
		trans=col;
	};
	Color transparentGet(){
		return trans;
	};

	//the emit
	void emitSet(Color col){
		emit=col;
	};
	Color emitGet(){
		return emit;
	};
	
	//the index of refraction
	void iorSet(double s){
		ior=s;
	};
	double iorGet(){
		return ior;
	};

	//the texture map
	void textureSet(string s){
		texture=s;
	};
	string textureGet(){
		return texture;
	};

	//the value of texture<U>
	void uSet(double s){
		textureU=s;
	};
	double uGet(){
		return textureU;
	};

	//the value of texture<V>
	void vSet(double s){
		textureV=s;
	};
	double vGet(){
		return textureV;
	};
	
	//set shape
	void shapeSet(Shape* s){
		shape=s;
	};
	Shape* shapeGet(){
		return shape;
	};	
	
private:
	Color diff, spe, amb, ref, trans,emit;
	double shine, ior, textureU, textureV;
	string texture;
	Shape *shape;
};

//tree is the list of the nodes
class Tree : public Node{
public:
	Tree();
	~Tree();
	//add a bew transform at the lasst position
	void addTransform(Transform* t); 
	void clearTran();
	void objListmake(Matrix4& m, list<MatrixObj>* l); 
	
private:
	list<Transform*> translist;
};

class Transform: public Node{ 
public:
	Transform();
	~Transform();
	
	void addMatrix(Matrix4 m); 
	void addNode(Node *n); 
	void addScale(double x=1, double y=1, double z=1);
	void addTranslation(double x=0, double y=0, double z=0);
	void addRotation(Vector3 axis, double angle);
	void objListmake(Matrix4& m, list<MatrixObj>* l); 

private:
	Matrix4 matrixTran,matrixTranirr,
			rotation,rotationirr,
			translation,translationirr,
			scaling,scalingirr;
	Node* nodeTran;
};

#endif /* _MY_SCENE_H_ */
