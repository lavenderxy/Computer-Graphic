#include "../cse452.h"
#include "MyScene.h"
#include <FL/Fl_BMP_Image.H>
#include <FL/Fl_JPEG_Image.H>
#include <FL/Fl_PNG_Image.H>
#include <cmath>

// string constants used for parsing
// feel free to add any new keywords that you think may be helpfull
const std::string TOKEN_BACKGROUND     = "background";
const std::string TOKEN_COLOR          = "color";
const std::string TOKEN_OB             = "[";
const std::string TOKEN_CB             = "]";
const std::string TOKEN_CAMERA         = "camera";
const std::string TOKEN_EYE            = "eye";
const std::string TOKEN_LOOK           = "look";
const std::string TOKEN_FOCUS          = "focus";
const std::string TOKEN_UP             = "up";
const std::string TOKEN_ANGLE          = "angle";
const std::string TOKEN_NEAR_FAR       = "near-far";
const std::string TOKEN_LIGHT          = "light";
const std::string TOKEN_LIGHTTYPE      = "type";
const std::string TOKEN_POINT          = "point";
const std::string TOKEN_DIRECTIONAL    = "directional";
const std::string TOKEN_SPOTLIGHT      = "spotlight";
const std::string TOKEN_POSITION       = "position";
const std::string TOKEN_DIRECTION      = "direction";
const std::string TOKEN_FUNCTION       = "function";
const std::string TOKEN_APERTURE       = "aperture";
const std::string TOKEN_EXPONENT       = "exponent";
const std::string TOKEN_MASTERSUBGRAPH = "mastersubgraph";
const std::string TOKEN_TRANS          = "trans";
const std::string TOKEN_ROTATE         = "rotate";
const std::string TOKEN_TRANSLATE      = "translate";
const std::string TOKEN_SCALE          = "scale";
const std::string TOKEN_MATRIXRC       = "matrixRC";
const std::string TOKEN_MATRIXCR       = "matrixCR";
const std::string TOKEN_OBJECT         = "object";
const std::string TOKEN_CUBE           = "cube";
const std::string TOKEN_CYLINDER       = "cylinder";
const std::string TOKEN_CONE           = "cone";
const std::string TOKEN_SPHERE         = "sphere";
const std::string TOKEN_COW            = "cow";
const std::string TOKEN_AMBIENT        = "ambient";
const std::string TOKEN_DIFFUSE        = "diffuse";
const std::string TOKEN_SPECULAR       = "specular";
const std::string TOKEN_REFLECT        = "reflect";
const std::string TOKEN_TRANSPARENT    = "transparent";
const std::string TOKEN_EMIT           = "emit";
const std::string TOKEN_SHINE          = "shine";
const std::string TOKEN_IOR            = "ior";
const std::string TOKEN_TEXTURE        = "texture";
const std::string TOKEN_SUBGRAPH       = "subgraph";
const std::string TOKEN_TORUS           = "torus";

MyScene::MyScene() :
ambientLight(0,0,0)
{
    // initialize your variables here
	cube = new Cube(1);
	sphere = new Sphere(5);
	cone = new Cone(20,1);
	cylinder = new Cylinder(20,1);	
	torus = new Torus(20,20);	
    
    resetScene();
}

MyScene::~MyScene() {
    // destroy your variables here
    clearMasSub();
	delete cube;
	delete cylinder;
	delete cone;
	delete sphere;
	delete torus;
}


void MyScene::resetScene() {
    // reset scene to defaults (empty the scene tree)
    background = Color();
    camera.setFrom(Point3(0, 0, 1));
    camera.setLook(Vector3(0, 0, -1));
    camera.setUp(Vector3(0, 1, 0));
    camera.setZoom(60.0);
    camera.setNearFar(0.01, 100.0);
    camera.setProjectionCenter( Point3(0,0,0) );
    camera.setSkew(0.0);
    camera.setAspectRatioScale(1.0);

    lights.clear();

    ambientLight = Color(0,0,0);

    // reset your data here
	//clear object list
    objList.clear();
    //delete the transformation list
	for(map<string, Tree*>::iterator it = masterSubgraphs.begin(); it != masterSubgraphs.end();	it++){
    	(it->second)->clearTran(); 
	}
	//clear subgraph 
    clearMasSub();
}

bool MyScene::loadSceneFile(std::string filename) {
    // load the scenefile with the given filename
    // return true if the file was loaded succesfully
    resetScene();
    // parse file:
    Parser p(filename);
    bool done = false;
    errorMessage = "";
    do {
        done = p.nextToken();
        if (p.getToken() == TOKEN_BACKGROUND)
            done = !parseBackground(p);
        else if (p.getToken() == TOKEN_CAMERA)
            done = !parseCamera(p);
        else if (p.getToken() == TOKEN_LIGHT)
            done = !parseLight(p);
        else if (p.getToken() == TOKEN_MASTERSUBGRAPH)
            done = (parseMasterSubgraph(p) == 0);
        else if (!done) {
            errorMessage = "Unrecognized token at root level: \"" + p.getToken() + "\"";
            done = true;
        }
    } while (!done);    
    // ToDo: check that there is a root mastersubgraph
    // and that no errors occured while loading
	isLoaded = (errorMessage.length()==0);
    isLoaded = isLoaded && (masterSubgraphs.find(string("root")) != masterSubgraphs.end());
    if (isLoaded) {
        // perform any post-loading computations here
        // (such as flattening the tree or building
        // ray-acceleration structures)
        Matrix4 id=Matrix4::identity();
		if(masterSubgraphs.find(string("root")) != masterSubgraphs.end())
			masterSubgraphs[string("root")]->objListmake(id, &objList);
	
    } else if (errorMessage.length() == 0)
        errorMessage = "Unable to locate root mastersubgraph";

    return isLoaded;
}

bool MyScene::parseBackground(Parser& p) {
    // parse the background block
    do {
        p.nextToken();
        if (p.getToken() == TOKEN_OB) {
        } else if (p.getToken() == TOKEN_CB) {
            break;
        } else if (p.getToken() == TOKEN_COLOR) {
          p.nextToken(); background[0] = static_cast<float>(p.getValue());
          p.nextToken(); background[1] = static_cast<float>(p.getValue());
          p.nextToken(); background[2] = static_cast<float>(p.getValue());
        } else {
            errorMessage = "Unrecognized token in background block: \"" + p.getToken() + "\"";
            return false;
        }
    } while (true);
    return true;
}

bool MyScene::parseCamera(Parser& p) {
    // parse the camera
    do {
        p.nextToken();
        if (p.getToken() == TOKEN_OB) {
        } else if (p.getToken() == TOKEN_CB) {
            break;
        } else if (p.getToken() == TOKEN_EYE) {
            Point3 eye;
            p.nextToken(); eye[0] = p.getValue();
            p.nextToken(); eye[1] = p.getValue();
            p.nextToken(); eye[2] = p.getValue();
            camera.setFrom(eye);
        } else if (p.getToken() == TOKEN_LOOK) {
            Vector3 look;
            p.nextToken(); look[0] = p.getValue();
            p.nextToken(); look[1] = p.getValue();
            p.nextToken(); look[2] = p.getValue();
            camera.setLook(look);
        } else if (p.getToken() == TOKEN_FOCUS) {
            Point3 focus;
            p.nextToken(); focus[0] = p.getValue();
            p.nextToken(); focus[1] = p.getValue();
            p.nextToken(); focus[2] = p.getValue();
            camera.setAt(focus);
        } else if (p.getToken() == TOKEN_UP) {
            Vector3 up;
            p.nextToken(); up[0] = p.getValue();
            p.nextToken(); up[1] = p.getValue();
            p.nextToken(); up[2] = p.getValue();
            camera.setUp(up);
        } else if (p.getToken() == TOKEN_ANGLE) {
            p.nextToken(); camera.setZoom(p.getValue());
        } else if (p.getToken() == TOKEN_NEAR_FAR) {
            double n, f;
            p.nextToken(); n = p.getValue();
            p.nextToken(); f = p.getValue();
            camera.setNearFar(n, f);
        } else {
            errorMessage = "Unrecognized token in camera block: \"" + p.getToken() + "\"";
            return false;
        }
    } while (true);
    return true;
}

bool MyScene::parseLight(Parser& p) {
    // parse a light source block
    Light l;
    do {
        p.nextToken();
        if (p.getToken() == TOKEN_OB) {
        } else if (p.getToken() == TOKEN_CB) {
            break;
        } else if (p.getToken() == TOKEN_LIGHTTYPE) {
            p.nextToken();
            if (p.getToken() == TOKEN_POINT)
                l.setType( Light::POINTLIGHT );
            else if (p.getToken() == TOKEN_DIRECTIONAL)
                l.setType( Light::DIRECTIONAL );
            else if (p.getToken() == TOKEN_SPOTLIGHT)
                l.setType( Light::SPOTLIGHT );
            else {
                errorMessage = "Unknown light type: \"" + p.getToken() + "\"";
                return false;
            }
        } else if (p.getToken() == TOKEN_POSITION) {
            Point3 pos;
            p.nextToken(); pos[0] = p.getValue();
            p.nextToken(); pos[1] = p.getValue();
            p.nextToken(); pos[2] = p.getValue();
            l.setPosition( pos );
        } else if (p.getToken() == TOKEN_COLOR) {
            Color color; 
            p.nextToken(); color[0] = static_cast<float>(p.getValue());
            p.nextToken(); color[1] = static_cast<float>(p.getValue());
            p.nextToken(); color[2] = static_cast<float>(p.getValue());
            l.setColor( color );
        } else if (p.getToken() == TOKEN_FUNCTION) {
            float f0,f1,f2;
            p.nextToken(); f0 = static_cast<float>(p.getValue());
            p.nextToken(); f1 = static_cast<float>(p.getValue());
            p.nextToken(); f2 = static_cast<float>(p.getValue());
            l.setFalloff( f0, f1, f2 );
        } else if (p.getToken() == TOKEN_DIRECTION) {
            Vector3 dir;
            p.nextToken(); dir[0] = p.getValue();
            p.nextToken(); dir[1] = p.getValue();
            p.nextToken(); dir[2] = p.getValue();
            dir.normalize();
            l.setDirection( dir );
        } else if (p.getToken() == TOKEN_APERTURE) {
            p.nextToken(); 
        	float apt = p.getValue();
        	if(apt<=0.0) apt = 0.01;
        	else if(apt>180.0) apt = 180.0;
            l.setAperture( apt );
        } else if (p.getToken() == TOKEN_EXPONENT) {
            p.nextToken();
        	double exp = p.getValue();
        	if(exp<0.0) exp = 0.0; 
        	l.setExponent( exp );
        } else if (p.getToken() == TOKEN_AMBIENT) {
            p.nextToken(); ambientLight[0] = static_cast<float>(p.getValue());
            p.nextToken(); ambientLight[1] = static_cast<float>(p.getValue());
            p.nextToken(); ambientLight[2] = static_cast<float>(p.getValue());
        } else {
            errorMessage = "Unrecognized token in light block: \"" + p.getToken() + "\"";
            return false;
        }

    } while (true);
    assert( l.getColor()[0] >= 0.0 && l.getColor()[0] <= 1.0 );
    assert( l.getColor()[1] >= 0.0 && l.getColor()[1] <= 1.0 );
    assert( l.getColor()[2] >= 0.0 && l.getColor()[2] <= 1.0 );
    assert( l.getAperture() >  0.0 && l.getAperture() <= 180.0 );
    assert( l.getExponent() >= 0.0 );
    lights.push_back(l);
    return true;
}

Tree* MyScene::parseMasterSubgraph(Parser& p) {
    // ToDo: parse a named subgraph tree
    Tree* subgraph = new Tree();
    
    //we haven't seen anything yet
    
    p.nextToken();
    std::string name = p.getToken();

    do {
        p.nextToken();
        if (p.getToken() == TOKEN_OB) {
        } else if (p.getToken() == TOKEN_CB) {        	
            break;
        } else if (p.getToken() == TOKEN_TRANS) {
            // parse node and add it to the tree
            // call parseTrans(p);
			subgraph->addTransform(static_cast<Transform*>(parseTrans(p)));
        } else {
            errorMessage = "Unrecognized token in mastersubgraph \"" + name + "\": \"" + p.getToken() + "\"";
            return 0;
        }
    } while (true);

    // add the new master subgraph to the master subgraph list
	masterSubgraphs[name] = subgraph;
    // ToDo: Fix this
    return subgraph;
}

Node* MyScene::parseTrans(Parser& p) {
    // parse a trans block node
	Transform* transform = new Transform();
	Object* object = new Object();
	
    transeen=false; 
	objectseen=false;
	
    do {
        p.nextToken();
        if (p.getToken() == TOKEN_OB) {
        } else if (p.getToken() == TOKEN_CB) {
        	if(objectseen==false && transeen==true){
				errorMessage = "No object to apply transformations to! \"";
				return 0;
			}
            break;
        } else if (p.getToken() == TOKEN_ROTATE) {
        	transeen=true;
            Vector3 axis;
            p.nextToken(); axis[0] = p.getValue();
            p.nextToken(); axis[1] = p.getValue();
            p.nextToken(); axis[2] = p.getValue();
            p.nextToken(); double angle = p.getValue() * M_PI / 180.0;
            // ToDo: add a rotation to the matrix stack here
            transform->addMatrix(Matrix4::rotation(axis,angle));  
			//transform->addRotation(axis,angle);

        } else if (p.getToken() == TOKEN_TRANSLATE) {
        	transeen=true;
            Vector3 v;
            p.nextToken(); v[0] = p.getValue();
            p.nextToken(); v[1] = p.getValue();
            p.nextToken(); v[2] = p.getValue();
            // ToDo: add a translation to the matrix stack here
            transform->addMatrix(Matrix4::translation(v));
			//transform ->addTraslation(v[0],v[1],v[2]);

        } else if (p.getToken() == TOKEN_SCALE) {
        	transeen=true;
            Vector3 v;
            p.nextToken(); v[0] = p.getValue();
            p.nextToken(); v[1] = p.getValue();
            p.nextToken(); v[2] = p.getValue();
            // ToDo:add a scale to the matrix stack here
            transform->addMatrix(Matrix4::scaling(v));
			//transform->addScale(v[0],v[1],v[2]);

        } else if (p.getToken() == TOKEN_MATRIXRC) {
        	transeen=true;
            Vector4 r0, r1, r2, r3;
            p.nextToken(); r0[0] = p.getValue();
            p.nextToken(); r0[1] = p.getValue();
            p.nextToken(); r0[2] = p.getValue();
            p.nextToken(); r0[3] = p.getValue();
      
            p.nextToken(); r1[0] = p.getValue();
            p.nextToken(); r1[1] = p.getValue();
            p.nextToken(); r1[2] = p.getValue();
            p.nextToken(); r1[3] = p.getValue();
      
            p.nextToken(); r2[0] = p.getValue();
            p.nextToken(); r2[1] = p.getValue();
            p.nextToken(); r2[2] = p.getValue();
            p.nextToken(); r2[3] = p.getValue();
      
            p.nextToken(); r3[0] = p.getValue();
            p.nextToken(); r3[1] = p.getValue();
            p.nextToken(); r3[2] = p.getValue();
            p.nextToken(); r3[3] = p.getValue();
            // ToDo: add the arbitrary matrix to the matrix stack here
            transform->addMatrix(Matrix4(r0, r1, r2, r3));

        } else if (p.getToken() == TOKEN_MATRIXCR) {
        	transeen=true;
            Vector4 r0, r1, r2, r3;
            p.nextToken(); r0[0] = p.getValue();
            p.nextToken(); r1[0] = p.getValue();
            p.nextToken(); r2[0] = p.getValue();
            p.nextToken(); r3[0] = p.getValue();
      
            p.nextToken(); r0[1] = p.getValue();
            p.nextToken(); r1[1] = p.getValue();
            p.nextToken(); r2[1] = p.getValue();
            p.nextToken(); r3[1] = p.getValue();
      
            p.nextToken(); r0[2] = p.getValue();
            p.nextToken(); r1[2] = p.getValue();
            p.nextToken(); r2[2] = p.getValue();
            p.nextToken(); r3[2] = p.getValue();
      
            p.nextToken(); r0[3] = p.getValue();
            p.nextToken(); r1[3] = p.getValue();
            p.nextToken(); r2[3] = p.getValue();
            p.nextToken(); r3[3] = p.getValue();
            // ToDo: add the arbitrary matrix to the matrix stack here
            transform->addMatrix(Matrix4(r0, r1, r2, r3));

        } else if (p.getToken() == TOKEN_OBJECT) {
            // ToDo: parse the object and add it to the node
            // call parseObject(p) here;
            if(objectseen==false){
				transform->addNode(static_cast<Node*>(parseObject(p)));
				objectseen=true;
			}
			else{
				while(p.getToken() != TOKEN_CB)
					p.nextToken();
			}

        } 
        else if (p.getToken() == TOKEN_SUBGRAPH) {
            // ToDo: find the subgraph and add it to the current node
            if(objectseen==false){
            	p.nextToken();
            	std::string subgraphName = p.getToken();
				if(masterSubgraphs.find(subgraphName) != masterSubgraphs.end())
                	transform->addNode(masterSubgraphs[subgraphName]);
            	else{
            		errorMessage = "Unrecognized subgraph name: \"" + subgraphName + "\"";
					return transform;
            	}
        		objectseen=true;
        	}
        	else
				p.nextToken();
        }
         else {
            errorMessage = "Unrecognized token in trans block: \"" + p.getToken() + "\"";
            // ToDo: Clean up
            resetScene();
            return 0;
        }
    } while (true);

    // ToDo: Fix this
    return transform;
}

Object* MyScene::parseObject(Parser& p) {
    // ToDo: parse an object block
	Object* object = new Object();

    p.nextToken();
    if (p.getToken() == TOKEN_CUBE) {
        // object is a cube
        object->shapeSet(cube);

    } else if (p.getToken() == TOKEN_CYLINDER) {
        // object is a cylinder
        object->shapeSet(cylinder);

    } else if (p.getToken() == TOKEN_CONE) {
        // object is a cone
        object->shapeSet(cone);

    } else if (p.getToken() == TOKEN_SPHERE) {
        // object is a sphere
        object->shapeSet(sphere);

    } else if (p.getToken() == TOKEN_TORUS) {
        object->shapeSet(torus);

    } else {
        errorMessage = "Unrecognized object type: \"" + p.getToken() + "\"";
        return 0;
    }
    do {
        p.nextToken();
        if (p.getToken() == TOKEN_OB) {
        } else if (p.getToken() == TOKEN_CB) {
            break;
        } else if (p.getToken() == TOKEN_AMBIENT) {
            double r, g, b;
            p.nextToken(); r = p.getValue();
            p.nextToken(); g = p.getValue();
            p.nextToken(); b = p.getValue();
            // store ambient color here
			object->ambientSet(Color(r,g,b));

        } else if (p.getToken() == TOKEN_DIFFUSE) {
            double r, g, b;
            p.nextToken(); r = p.getValue();
            p.nextToken(); g = p.getValue();
            p.nextToken(); b = p.getValue();
            // store diffuse color here
            object->diffuseSet(Color(r,g,b));

        } else if (p.getToken() == TOKEN_SPECULAR) {
            double r, g, b;
            p.nextToken(); r = p.getValue();
            p.nextToken(); g = p.getValue();
            p.nextToken(); b = p.getValue();
            // store specular color here
            object->specularSet(Color(r,g,b));

        } else if (p.getToken() == TOKEN_REFLECT) {
            double r, g, b;
            p.nextToken(); r = p.getValue();
            p.nextToken(); g = p.getValue();
            p.nextToken(); b = p.getValue();
            // store reflect color here
            object->reflectSet(Color(r,g,b));

        } else if (p.getToken() == TOKEN_TRANSPARENT) {
            double r, g, b;
            p.nextToken(); r = p.getValue();
            p.nextToken(); g = p.getValue();
            p.nextToken(); b = p.getValue();
            // store transparent color here
            object->transparentSet(Color(r,g,b));

        } else if (p.getToken() == TOKEN_SHINE) {
            double s;
            p.nextToken(); s = p.getValue();
            // store shine here
            object->shineSet(s);


        } else if (p.getToken() == TOKEN_IOR) {
            double ior;
            p.nextToken(); ior = p.getValue();
            // store ior here
            object->iorSet(ior);

        } else if (p.getToken() == TOKEN_TEXTURE) {
            std::string filename;
            double textureU, textureV;
            p.nextToken(); filename = p.getToken();
            p.nextToken(); textureU = p.getValue();
            p.nextToken(); textureV = p.getValue();
            // store texture information here
			object->textureSet(filename);
			object->uSet(textureU);
			object->vSet(textureV);

		}else if (p.getToken() == TOKEN_EMIT){
			double r, g, b;
            p.nextToken(); r = p.getValue();
            p.nextToken(); g = p.getValue();
            p.nextToken(); b = p.getValue();
            // store emitted color here
            object->emitSet(Color(r,g,b));

		} else {
            errorMessage = "Unrecognized token in object block: \"" + p.getToken() + "\"";
            return 0;
        }
    } while (true);

    // ToDo: Fix this
    return object;
}

std::string MyScene::getErrorMessage() const {
    // return the current error message
    return errorMessage;
}

const Camera & MyScene::getCamera() const {
    // return the current camera
    return camera;
}

Camera & MyScene::changeCamera() {
    // return the current camera
    return camera;
}

const Color &MyScene::getBackground() const {
    return background;
}

/******************added struct and class********************/
/****define for class MatrixObj***/
MatrixObj::MatrixObj(Matrix4 matrixOrigin_, Object* objectOrigin_){
	matrixOrigin = matrixOrigin_; objectOrigin = objectOrigin_;
}

MatrixObj::~MatrixObj(){
	if (!treein)
		delete objectOrigin;
}

/****define for class Node****/
Node::Node(){
	Node::isNode=false;
}
Node::~Node(){}

void Node::objListmake(Matrix4& m, list<MatrixObj>* l){}

/****define for class Object****/
Object::Object(Shape* s){
	Node::isNode=true;
	shape = s;
	amb = Color(0.0,0.0,0.0);
	diff = Color(1.0,1.0,1.0);
	spe = Color(0.0,0.0,0.0);
	ref = Color(0.0,0.0,0.0);
	trans = Color(0.0,0.0,0.0);
	emit = Color(0.0,0.0,0.0);
	shine = 1.0;
	ior = 1.0;
}

Object::Object(){
	Node::isNode=true;
	shape = 0;
	amb = Color(0.0,0.0,0.0);
	diff = Color(1.0,1.0,1.0);
	spe = Color(0.0,0.0,0.0);
	ref = Color(0.0,0.0,0.0);
	trans= Color(0.0,0.0,0.0);
	shine = 1.0;
	ior = 1.0;
}

Object::Object(Object* o){
	Node::isNode=true;
	shape = o->shapeGet();
	amb = o->ambientGet();
	diff = o->diffuseGet();
	spe = o->specularGet();
	ref = o->reflectGet();
	trans = o->transparentGet();
	emit = o->emitGet();
	shine = o ->shineGet();
	ior = o ->iorGet();
}

Object::~Object(){}

/****define for class tree****/
Tree::Tree(){
	Node::isNode=false;
}

Tree::~Tree(){
	clearTran();
}

/****define for class transform****/
Transform::Transform(){
	matrixTran = Matrix4::identity();
	matrixTranirr = Matrix4::identity();
	rotation = Matrix4::identity();
	rotationirr = Matrix4::identity();
	translation = Matrix4::identity();
	translationirr = Matrix4::identity();
	scaling = Matrix4::identity();
	scalingirr = Matrix4::identity();

	nodeTran = 0;
}

Transform::~Transform(){
	if(nodeTran != 0 && Node::isNode==true)
		delete nodeTran;
}

/******************added method or functions*******************/
/****method for object****/
void Object::objListmake(Matrix4& m, list<MatrixObj>* l){
		l->push_back(MatrixObj(m, this));
}

void Object::drawObject(){
	
	GLfloat ambMatrix[] = {amb[0], amb[1], amb[2], 1};
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambMatrix);
	GLfloat difMatrix[] = {diff[0], diff[1], diff[2], 1};
	glMaterialfv(GL_FRONT, GL_DIFFUSE, difMatrix);
	GLfloat specMatrix[] = {spe[0], spe[1], spe[2], 1};
	glMaterialfv(GL_FRONT, GL_SPECULAR, specMatrix);
	GLfloat shininess = static_cast<GLfloat>(shine);
	glMaterialfv(GL_FRONT, GL_SHININESS, &shininess);
	
	shape->drawnow();	
}

/****method for tree****/
void Tree::objListmake(Matrix4& m, list<MatrixObj>* l){
	for(list<Transform*>::iterator it = translist.begin(); it != translist.end(); it++){
		(*it)->objListmake(m, l);
	}
}

void Tree::addTransform(Transform* t){
	translist.push_back(t);
}

void Tree::clearTran(){
	for(list<Transform*>::iterator it = translist.begin(); it != translist.end(); it++)
		delete (*it);
	translist.clear();
}

/****method for transform****/
void Transform::objListmake(Matrix4& m, list<MatrixObj>* l){
	Matrix4 matrix = m*matrixTran;
	nodeTran->objListmake(matrix, l);
}

void Transform::addMatrix(Matrix4 m){
	matrixTran *= m;
}

void Transform::addNode(Node *n){
	nodeTran=n;
}

void Transform::addScale(double x, double y, double z){
	Matrix4 scale=Matrix4 (Vector4(x, 0.0, 0.0, 0.0),Vector4(0.0,y,0.0,0.0),Vector4(0.0,0.0,z,0.0),Vector4(0.0,0.0,0.0,1.0));
	Matrix4 scaleirr=Matrix4 (Vector4(1.0/x, 0.0, 0.0, 0.0),Vector4(0.0,1.0/y,0.0,0.0),Vector4(0.0,0.0,1.0/z,0.0),Vector4(0.0,0.0,0.0,1.0));

	scaling =scaling* scale;
	scalingirr = scalingirr*scaleirr;
	matrixTran = matrixTran*scale;
	matrixTranirr = matrixTranirr*scaleirr;
}

void Transform::addTranslation(double x, double y, double z){
	Matrix4 trans=Matrix4 (Vector4(1.0, 0.0, 0.0, x),Vector4(0.0,1.0,0.0,y),Vector4(0.0,0.0,1.0,z),Vector4(0.0,0.0,0.0,1.0));
	Matrix4 transirr=Matrix4 (Vector4(1.0, 0.0, 0.0, -x),Vector4(0.0,1.0,0.0,-y),Vector4(0.0,0.0,1.0,-z),Vector4(0.0,0.0,0.0,1.0));

	translation =translation*trans;
	translationirr =translationirr*transirr;
	matrixTran =matrixTran*trans;
	matrixTranirr =matrixTranirr*transirr;
}

void Transform::addRotation(Vector3 axis, double angle){
	axis.normalize();
	double cos=std::cos(angle);
	double sin=std::sin(angle);
	double cosirr=1-cos;
	Matrix4 rotate=Matrix4 (Vector4(cos+cosirr*axis[0]*axis[0], cosirr*axis[0]*axis[1]-sin*axis[2], cosirr*axis[0]*axis[2]+sin*axis[1], 0.0),
							Vector4(cosirr*axis[1]*axis[0]+sin*axis[2],cos+cosirr*axis[1]*axis[1] , cosirr*axis[1]*axis[2]-sin*axis[0], 0.0),
							Vector4(cosirr*axis[2]*axis[0]-sin*axis[1], cosirr*axis[2]*axis[1]+sin*axis[0], cos+cosirr*axis[2]*axis[2],  0.0),
							Vector4(0.0,0.0,0.0,1.0));

	cos=std::cos(-angle);
	sin=std::sin(-angle);
	cosirr=1-cos;
	Matrix4 rotateirr=Matrix4 (Vector4(cos+cosirr*axis[0]*axis[0], cosirr*axis[0]*axis[1]-sin*axis[2], cosirr*axis[0]*axis[2]+sin*axis[1], 0.0),
							Vector4(cosirr*axis[1]*axis[0]+sin*axis[2],cos+cosirr*axis[1]*axis[1] , cosirr*axis[1]*axis[2]-sin*axis[0], 0.0),
							Vector4(cosirr*axis[2]*axis[0]-sin*axis[1], cosirr*axis[2]*axis[1]+sin*axis[0], cos+cosirr*axis[2]*axis[2],  0.0),
							Vector4(0.0,0.0,0.0,1.0));

	rotation =rotation*rotate;
	rotationirr =rotationirr * rotateirr;
	matrixTran =matrixTran*rotate;
	matrixTranirr =matrixTranirr*rotateirr;

}

/****method for myscene****/
void MyScene::clearMasSub(){
    //clear the subgraph and the tree, clear the tree before clearing the map
    for(map<string, Tree*>::iterator it = masterSubgraphs.begin(); it != masterSubgraphs.end(); it++){
    	delete it->second; //it->second is a Tree*
	}
	//clear the map of subgraph
    masterSubgraphs.clear();
}

