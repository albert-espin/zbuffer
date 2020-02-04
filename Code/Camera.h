/***************************************************/
/*           Definicio de la camera                */
/***************************************************/
#ifndef CAMERA_INCLOS
#define CAMERA_INCLOS
#include <stdio.h>
#include <Common.h>
#include <QGLShaderProgram>



#define PI 3.14159

#define MAXDPOST 100000
#define MINDANT -100000
#define EPS 0.001

#define DEGREES *180.0/PI


typedef enum {PARALLELA = 0, PERSPECTIVA = 1} TipProj; 


/* =================================== */
/* =                                 = */
/* =             Camera              = */
/* =                                 = */
/* =================================== */

class Camera
{
public:
    explicit Camera();

    Camera(vec4 lookfrom, vec4 lookat, vec4 vup, float vfov, int viewX, int viewY, float aperture, float focus_dist) ;
    ~Camera() {}

    void init(int a, int h, Capsa3D c);

    void actualitzaCamera(Capsa3D capsaMinima);
    void rotaCamera(Capsa3D capsa);

    void toGPU(QGLShaderProgram *program);


    void CalculaModelView();
    void CalculaProjection();
    void CalculaWindow(Capsa3D);

    void AjustaAspectRatioWd();
    void AmpliaWindow(double r);

    void anglesToAxis(vec3 angles);

    Capsa2D  CapsaMinCont2DXYVert( vec4 *v, int nv);

    void setViewport(int x, int y, int a, int h);
    void setModelView(QGLShaderProgram *program, mat4 m);
    void setProjection(QGLShaderProgram *program, mat4 p);

    vec4 origin;
    vec4 vrp;

    // window
    Capsa2D window;
    vec4 u, v, w;
    float lens_radius;

    double angX; // Angles de rotacio de la camera
    double angY;
    vec4  vUp;

    TipProj typeProjection;

    // Plans de clipping anterior i posterior
    float dant, dpost;

    // Viewport
    Capsa2D vp;

private:
    void VertexCapsa3D(Capsa3D capsaMinima, vec4 vaux[8]);

    mat4  modView; // Matriu model-view de la CPU
    mat4  proj;  // Matriu projection de la CPU
    GLuint  model_view;  // model-view matrix uniform shader variable (GPU)
    GLuint  projection;  // projection matrix uniform shader variable (GPU)
};


#endif









