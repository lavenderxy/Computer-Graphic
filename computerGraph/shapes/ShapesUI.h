#ifndef _MYSHAPES_UI_H_
#define _MYSHAPES_UI_H_

#include "../UIInterface.h"
#include "../cse452.h"
#include <FL/Fl_Window.H>
#include "Shape.h"

//define the Shapes here
class Shape;
class ShapesInterface;
class ShapesUI : public UIInterface {
public:
    ShapesUI();
    ~ShapesUI();

    // Inherited from UIInterface
    void resize(int width, int height);
    void draw();
    int handle(int event);
    
    enum ShapeType {
        SHAPE_SPHERE,
        SHAPE_CONE,
        SHAPE_CYLINDER,
        SHAPE_CUBE,
		SHAPE_TORUS,
		SHAPE_CORKSCREW,
		SHAPE_BANANA
    };

    enum DisplayType {
        DISPLAY_WIREFRAME,
        DISPLAY_FLAT_SHADING,
        DISPLAY_SMOOTH_SHADING
    };

    void changedTessel();
    void changedShape();
    
    // Link to the shapes widget
    void setUI( const ShapesInterface *in_ui ) { shapesUI = in_ui; }

	void shapeconstruct();


private:
    int width, height;
    const ShapesInterface *shapesUI;

    // declare your variables here
	//declare the current shape
	Shape *Shapenow;
};

#endif /* _SHAPES_UI_H_ */
