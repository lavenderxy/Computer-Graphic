#include "cse452.h"
#include "MyBrush.h"
#include "BrushInterface.h"
#include <cmath>
#include <iostream>
#include <algorithm>
using namespace std; 

void MyBrush::changedBrush() {
	// this is called anytime the brush type or brush radius changes
	// it should recompute the brush mask appropriately
	const int radius = brushUI->getRadius();
	maskSize = 2*radius+1;
	mask.resize(maskSize*maskSize);
	BrushType brushType = brushUI->getBrushType();
	switch(brushType){
		case BRUSH_CONSTANT:
			//drawBrush();
			constantDistr(radius);
			break;
		case BRUSH_LINEAR:
			LinearDistr(radius);
			break;
		case BRUSH_QUADRATIC:
			QuadraticDistr(radius);
			break;
		case BRUSH_GAUSSIAN:
			GaussianDistr(radius);
			break;
		case BRUSH_SPECIAL:
			SpecialDistr(radius);
			break;
  }
}

void MyBrush::constantDistr(const int radius){
	for (int i=0; i<maskSize;i++){
		int x_dis=i-radius;
		int x_dis1=pow(x_dis,2);
		for (int j=0; j<maskSize;j++){
			int y_dis=j-radius;
			int y_dis1=pow(y_dis,2);
			if ((x_dis1+y_dis1)<radius*radius)
				makemask(1,i,j);
				//makemask(1);
			else
				makemask(0,i,j);
				//makemask(0);
		}
	}
}

void MyBrush::LinearDistr(const int radius){
	for (int i=0; i<maskSize;i++){
		int x_dis=i-radius;
		int x_dis1=pow(x_dis,2);
		for (int j=0; j<maskSize;j++){
			int y_dis=j-radius;
			int y_dis1=pow(y_dis,2);
			float tmp=1-sqrt(x_dis1+y_dis1)/radius;
			if ((x_dis1+y_dis1)<radius*radius)
				makemask(tmp,i,j);
			else
				makemask(0,i,j);

		}
	}
}

void MyBrush::QuadraticDistr(const int radius){
	for (int i=0; i<maskSize;i++){
		int x_dis=i-radius;
		int x_dis1=pow(x_dis,2);
		for (int j=0; j<maskSize;j++){
			int y_dis=j-radius;
			int y_dis1=pow(y_dis,2);
			float tmp=1-pow(sqrt(x_dis1+y_dis1)/radius,2);
			if ((x_dis1+y_dis1)<radius*radius)
				makemask(tmp,i,j);
			else
				makemask(0,i,j);

		}
	}
}

void MyBrush::GaussianDistr(const int radius){
	for (int i=0; i<maskSize;i++){
		int x_dis=i-radius;
		int x_dis1=pow(x_dis,2);
		for (int j=0; j<maskSize;j++){
			int y_dis=j-radius;
			int y_dis1=pow(y_dis,2);
			float tmp=pow(radius-sqrt(x_dis1+y_dis1),2);
			float tmp1=1-exp(-tmp/pow(radius,2));
			if ((x_dis1+y_dis1)<radius*radius)
				makemask(tmp1,i,j);
			else
				makemask(0,i,j);

		}
	}
}

void MyBrush::SpecialDistr(const int radius){
	for (int i=0; i<maskSize;i++){
		int x_dis=i-radius;
		int x_dis1=pow(x_dis,2);
		for (int j=0; j<maskSize;j++){
			int y_dis=j-radius;
			int y_dis1=pow(y_dis,2);
			makemask(1,i,j);
		}
	}
}

void MyBrush::drawBrush( ) {
	// apply the current brush mask to image location (x,y)
	// the mouse location is in mouseDrag

	const int radius = brushUI->getRadius();
	const float pixelFlow = brushUI->getFlow();
	const Color colBrush = brushUI->getColor();
	float brush_mask;
	
	//Create Brush distribution
	//set the boundary of brush
	int brushX_min,brushY_min,brushX_max,brushY_max;
	int a1,b1;
	brushX_min=max(0,mouseDrag[0]-radius);
	brushY_min=max(0,mouseDrag[1]-radius);
	brushX_max=min(screenWidth, mouseDrag[0]+radius);
	brushY_max= min(screenHeight, mouseDrag[1]+radius);
  
	//scan the brush area we create and put the color distribution on it
	for(int i=brushX_min; i<brushX_max;i++){
		//int a1= abs(mouseDrag[0]-i);
		a1=i-mouseDrag[0]+radius;
		for(int j= brushY_min;j< brushY_max ;j++){
			//int b1=abs(mouseDrag[1]-j);
			b1=j-mouseDrag[1]+radius;
			brush_mask = create_mask(a1,b1); //distribution value
			//fraction of the final pixel color contributed by the brush color
			Color colorBrush=brush_mask*colBrush*pixelFlow;
			Color colorCanvas=(1-brush_mask*pixelFlow)*getPixel(i,j);
			putPixel(i, j,colorBrush+colorCanvas);
	  }
  }
}

/*
void MyBrush::makemask(float val){
	for (int i=0;i<maskSize;i++)
		mask[i]=val;
}*/

float MyBrush::create_mask(int x, int y){
	//mask is an array, store pixel in to the mask, create the mask
	int tmp1,tmp2;
	tmp1=x*maskSize; //assume that a maskSize is a part, choose the xth part, first col or second...
	tmp2=tmp1+y; //when locate the col, locate the row, then create the specific mask 
	return mask[tmp2];
}

void MyBrush::makemask(float val, int x, int y){
	int tmp1,tmp2;
	tmp1=x*maskSize;
	tmp2=tmp1+y;
	mask[tmp2]=val;
}


void MyBrush::drawLine( ) {
	// draw a thick line from mouseDown to mouseDrag
	// the width of the line is given by the current brush radius
	const int radius = brushUI->getRadius();
    const Color colBrush = brushUI->getColor();
	int x0=mouseDown[0], y0=mouseDown[1];
	int x1=mouseDrag[0], y1=mouseDrag[1];
	float slope, slope1;
  
	if(x0==x1){
		int yVal=min(y0,y1);
		int yVal1=max(y0,y1);
		for(int y=yVal; y<yVal1;y++)
			for(int deltax=-radius/2; deltax<=radius/2; deltax++){
				int x=deltax+x0;
				if(x >= 0 && x < screenWidth && y >= 0 && y < screenHeight)
					putPixel(x,y,colBrush);
		}
	}
	else if(y0==y1){
		int xVal=min(x0,x1);
		int xVal1=max(x0,x1);
		for(int x=xVal; x<xVal1;x++)
			for(int deltay=-radius/2; deltay<=radius/2; deltay++){
				int y=deltay+y0;
				if(x >= 0 && x < screenWidth && y >= 0 && y < screenHeight)
					putPixel(x,y,colBrush);
		}
	}
	else{
		slope=float(y1-y0)/(x1-x0); //without float control, the line |slope|<1 will not draw
		slope1=-1/slope;
		//using similar trangle, (r/2)/delta(x)=sqrt(slope*slope+1)/slope
		//(r/2)/delta(y)=sqrt(slope*slope+1)/1
		
		float deltax,deltay;
		deltax=radius*slope/float(2*sqrt(pow(slope,2)+1));
		deltay=radius/float(2*sqrt(pow(slope,2)+1));
		
		int x0tmp,y0tmp,x0tmp1,y0tmp1;
		int x1tmp,y1tmp,x1tmp1,y1tmp1;
		
		x0tmp=int(deltax+x0);y0tmp=int(-deltay+y0);
		x0tmp1=int(-deltax+x0);y0tmp1=int(deltay+y0);
		x1tmp=int(deltax+x1),y1tmp=int(-deltay+y1);
		x1tmp1=int(-deltax+x1),y1tmp1=int(deltay+y1);

		if(abs(slope)<1){
			drawLine1(x0tmp,x1tmp,y0tmp,y1tmp,colBrush);
			drawLine1(x0tmp1,x1tmp1,y0tmp1,y1tmp1,colBrush);  
		}
		else {
			drawLine2(x0tmp,x1tmp,y0tmp,y1tmp,colBrush);
			drawLine2(x0tmp1,x1tmp1,y0tmp1,y1tmp1,colBrush); 
		}
		if(abs(slope1)<1){
			drawLine1(x0tmp,x0tmp1,y0tmp,y0tmp1,colBrush);
			drawLine1(x1tmp,x1tmp1,y1tmp,y1tmp1,colBrush);  
		 }
		 else {
			drawLine2(x0tmp,x0tmp1,y0tmp,y0tmp1,colBrush);
			drawLine2(x1tmp,x1tmp1,y1tmp,y1tmp1,colBrush); 
		 }

		 colorin(x0tmp, x0tmp1, x1tmp,  x1tmp1, y0tmp, y0tmp1,  y1tmp, y1tmp1,colBrush);

	}

}

void MyBrush::drawLine1(int x0, int x1, int y0, int y1,Color colorLine){
	//when draw the line in right-left,interchange x0,x1 and y0,y1.
	if (x0>x1){
		int tmp;
		tmp=x0;x0=x1;x1=tmp;
		tmp=y0;y0=y1;y1=tmp;
	}  

	int dx=x1-x0, dy=y1-y0;
	int d=2*dy-dx;
	int d1=2*dy+dx;
	int incrE=2*dy;
	int incrNE=2*(dy-dx);
	int incrSE=2*(dy+dx);
	int x=x0, y=y0;
	if(x>=0 && x<screenWidth && y>=0 && y<screenHeight)
		putPixel(x,y,colorLine);

	while(x<x1){
		if(d<=0 && y1>=y0)
			d=d+incrE;       //East case
		else if(d>0 && y1>=y0){
			d=d+incrNE;     // Northeast case
			++y;
		}
		else if(d<=0 && y1<=y0)
			d=d-incrE;      //East case
		else{
			d=d-incrSE;		//Southease case
			--y;
		}  
		/*
		if(d<=0)
			d=d-incrE;
		else{
			d=d-incrSE;
			--y;
		}   */
		++x;
		if(x>=0 && x<screenWidth && y>=0 && y<screenHeight)
			putPixel(x,y,colorLine);
	}
}


void MyBrush::drawLine2(int x0, int x1, int y0, int y1,Color colorLine){
	//when the slope is out of -1~1, interchange x and y;
	int tmp1,tmp2;
	tmp1=x0;x0=y0;y0=tmp1;
	tmp2=x1;x1=y1;y1=tmp2;
		
	if (x0 > x1){
		int tmp;
		tmp=x1;x1=x0;x0=tmp;
		tmp=y1;y1=y0;y0=tmp;			
	}   
	
	int dx=x1-x0, dy=y1-y0;
	int d=2*dy-dx;
	int d1=2*dy+dx;
	int incrE=2*dy;
	int incrNE=2*(dy-dx);
	int incrSE=2*(dy+dx);
	int x=x0, y=y0;
	if(y>=0 && y<screenWidth && x>=0 && x<screenHeight)
		putPixel(y,x,colorLine);

	while(x<x1){
		if(d<=0 && y1>=y0)
			d=d+incrE;       //East case
		else if(d>0 && y1>=y0){
			d=d+incrNE;     // Northeast case
			++y;
		}
		else if(d<=0 && y1<=y0)
			d=d-incrE;      //East case
		else{
			d=d-incrSE;     //Southeast case
			--y;
		}  
		/*
		if(d<=0)
			d=d-incrE;
		else{
			d=d-incrSE;
			--y;
		}   */
		++x;
		if(y>=0 && y<screenWidth && x>=0 && x<screenHeight)
			putPixel(y,x,colorLine);
	}
	
}

void MyBrush::colorin(int x0tmp, int x0tmp1, int x1tmp, int x1tmp1, int y0tmp, int y0tmp1, int y1tmp, int y1tmp1, Color colorLine){
	//create a vector space to store pixel
	std::vector<int> pixeldraw;
	
	//find the equations of four boundary lines
	//by using the four vertexs
	//y=kx+b
	float m1=float(y1tmp-y0tmp)/(x1tmp-x0tmp);float b1=y1tmp-m1*x1tmp;
	float m2=float(y1tmp1-y1tmp)/(x1tmp1-x1tmp);float b2=y1tmp1-m2*x1tmp1;
	float m3=float(y1tmp1-y0tmp1)/(x1tmp1-x0tmp1);float b3=y0tmp1-m3*x0tmp1;
	float m4=float(y0tmp1-y0tmp)/(x0tmp1-x0tmp);float b4=y0tmp-m4*x0tmp;

	int minx1,minx2,minx;
	int maxx1,maxx2,maxx;

	minx1=min(x0tmp,x1tmp);
	minx2=min(x0tmp1,x1tmp1);
	minx=min(minx1,minx2);

	maxx1=max(x0tmp,x1tmp);
	maxx2=max(x0tmp1,x1tmp1);
	maxx=max(maxx1,maxx2);

	int y,num=0;
	for(int x=minx; x<maxx; x++){
		if(x>=min(x1tmp,x0tmp) && x<=max(x1tmp,x0tmp)){
			y=m1*x+b1;
			pixeldraw.push_back(y);
		}

		if(x>=min(x1tmp1,x1tmp) && x<=max(x1tmp1,x1tmp)){
			y=m2*x+b2;
			pixeldraw.push_back(y);
		}

		if(x>=min(x1tmp1,x0tmp1) && x<=max(x1tmp1,x0tmp1)){
			y=m3*x+b3;
			pixeldraw.push_back(y);
		}

		if(x>=min(x0tmp1,x0tmp) && x<=max(x0tmp1,x0tmp)){
			y=m4*x+b4;
			pixeldraw.push_back(y);
		}

		int ymin,ymax;

		num=pixeldraw.size();

		if (num==2){
			ymin=min(pixeldraw[0],pixeldraw[1]);
			ymax=max(pixeldraw[0],pixeldraw[1]);
			for(y=ymin;y<=ymax;y++){
				if(x>=0 && x< screenWidth && y>=0 && y<screenHeight )
					putPixel(x,y,colorLine);
			}
		}
		else if(num==3){
			int tmp1,tmp2;
			tmp1=min(pixeldraw[0],pixeldraw[1]);
			ymin=min(tmp1,pixeldraw[2]);
			tmp2=max(pixeldraw[0],pixeldraw[1]);
			ymax=max(tmp2,pixeldraw[2]);

			for(y=ymin;y<=ymax;y++)
				if(x>= 0 && x<screenWidth && y>= 0 && y<screenHeight)
					putPixel(x,y,colorLine);
		}
		
		pixeldraw.resize(0);

	}
}



void MyBrush::drawCircle() {
	// drawa thick circle at mouseDown with radius r 
	// the width of the circle is given by the current brush radius	

	const int radius = brushUI->getRadius();		
	const Color colBrush = brushUI->getColor();

	int x0=mouseDown[0],y0=mouseDown[1];
	int x1=mouseDrag[0],y1=mouseDrag[1];
	int x,y,R;
	R=int(floor( sqrt(pow((x1-x0),2)+pow((y1-y0),2))+0.5));
	//x=0;y=R;
	//int deltaE=2*x+3;
	//int deltaSE=2*(x-y)+5;
	//float decision=(x+1)*(x+1)+(y-0.5)*(y-0.5)-R*R;
	//x=0;y=R;
	//if(x>=0 && x<screenWidth && y>=0 && y<screenHeight)
	//putPixel(x,y,colBrush);

	int r;
	if (radius>=R)
		r=1;
	else
		r=R-radius;

	maskSize = 2*R+1;
	mask.resize(maskSize*maskSize);
	circlemake(R,r);

	int X_min,Y_min,X_max,Y_max;
	int a1,b1;
	float circle_mask;
	X_min=max(0,x0-R);
	Y_min=max(0,y0-R);
	X_max=min(screenWidth, x0+R);
	Y_max= min(screenHeight, y0+R);

	for(int i=X_min; i<X_max;i++){
		for(int j= Y_min;j< Y_max ;j++){
			b1=j-y0+R;
			a1=i-x0+R;
			circle_mask = create_mask(a1,b1); 
			Color colorBrush=circle_mask*colBrush;
			Color colorCanvas=(1-circle_mask)*getPixel(i,j);
			putPixel(i, j,colorBrush+colorCanvas);
		}
	}  


	/*
	while ((y)>(x)){
		if(decision<0){    //Move East
			x++;
			if(x>=0 && x<screenWidth && y>=0 && y<screenHeight)
				putPixel(x,y,colBrush);
			decision+=deltaE;
			deltaE +=2;
			deltaSE +=2;     //Update deltas
		}
		else{              //Move SouthEast
			y--;
			x++;
			if(x>=0 && x<screenWidth && y>=0 && y<screenHeight)
				putPixel(x,y,colBrush);
			decision +=deltaSE;
			deltaE+=2;
			deltaSE +=4;     //Update deltas
		}
	}     */


}

void MyBrush::circlemake(int R, int r){
	for (int i=0; i<maskSize;i++){
		int x_dis=i-R;
		int x_dis1=pow(x_dis,2);
		for (int j=0; j<maskSize;j++){
			int y_dis=j-R;
			int y_dis1=pow(y_dis,2);
		
			if ((x_dis1+y_dis1)<R*R && (x_dis1+y_dis1)>r*r)
				makemask(1,i,j);
			else
				makemask(0,i,j);

		}
	}
}


void MyBrush::drawPolygon() {
	// draw a polygon with numVertices whos coordinates are stored in the
	// polygon array: {x0, y0, x1, y1, ...., xn-1, yn-1}
	// we should assume that the last vertex is connected to the first one in order to always have a 
	// closed polygon boundary.
}

void MyBrush::filterRegion( ) {
	// apply the filter indicated by filterType to the square
	// defined by the two corner points mouseDown and mouseDrag
	// these corners are not guarenteed to be in any order
	// The filter width is given by the brush radius
	
}


