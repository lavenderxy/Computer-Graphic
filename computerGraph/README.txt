Xinyu Yan
xinyu.yan@wustl.edu
VS2012

Lab6 Raytracing

1.A description of any additional classes, methods, or files you have added.

added method:
class MyScene:
a. void pixelSet(int x, int y, int w, int h, const Color &col,unsigned char*pixel);  //put pixel when render

b. void rayIntersect(Point3 point, Vector3 distance, Point3& hitP, Vector3& hitN, MatrixObj*& hitObject ); //ray intersect with the object

c. void rayTracingRend(int w, int h, unsigned char* pixel);  //raytracing rendering

d. Color ambIllum(MatrixObj* m);  //calculate ambient color

e. Color diffIllum(MatrixObj* m, Point3 hitP, Vector3 hitN, Light* l);//calculate diffuse color

f. Color specIllum(MatrixObj* m, Vector3 ray, Point3 hitP, Vector3 hitN, Light* l); //calculate speculate color

g. Color illuminate(Point3 point, Vector3 distance, int level); //illuminate all the color
    my reflected ray is added in the illuminate method

2. Any known bugs
There is no bug.


3. A brief description of my extra credit
   I added refractive transparency and transparency in the illuminate method. 








 





                    