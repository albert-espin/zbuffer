#include "Camera.h"

Camera::Camera(vec4 lookfrom, vec4 lookat, vec4 vup, float vfov, int viewX, int viewY, float aperture, float focus_dist) { // vfov is top to bottom in degrees
    float aspect = float(viewX)/float(viewY);
    vp.pmin = vec2( 0., 0.);
    vp.a = viewX;
    vp.h = viewY;
    lens_radius = aperture / 2.0;
    float theta = vfov*M_PI/180;
    float half_height = tan(theta/2);
    float half_width = aspect * half_height;

    w = normalize(lookfrom - lookat);
    u = normalize(cross(vup, w));
    v = cross(w, u);

    origin = lookfrom;
    vrp = lookat;
    vUp = vup;

    window.pmin = vec2(-half_width, -half_height);
    window.a = 2*half_width;
    window.h = 2*half_height;

    dant = 0.01;
    dpost = 200;

    typeProjection = PARALLELA;
    CalculaModelView();
    CalculaProjection();

}

Camera::Camera()
{
    origin = vec4(0.0, 0.0, 1.0, 1.0);

    angX = 0;
    angY = 0;

    vp.a = 600;
    vp.h = 600;
    vp.pmin[0] = 0;
    vp.pmin[1] = 0;


    float half_height = 1.0;
    float half_width = 1.0;

    vrp = vec4( 0.0, 0.0, 0.0, 1.0);
    w = vec4(0.0, 0.0, -1.0, 0.0);
    vUp = vec4(0.0, 1.0, 0.0, 0.0);
    u = normalize(cross(vUp, w));
    v = cross(w, u);

    angX = 0.0;
    angY = 0.0;

    window.pmin = vec2(-half_width, -half_height);
    window.a = 2*half_width;
    window.h = 2*half_height;

    dant = 0.01;
    dpost = 100;

    typeProjection = PARALLELA;
    CalculaModelView();
    CalculaProjection();
}

void Camera::init(int a, int h, Capsa3D capsaMinima)
{
    vec3 p1;

    p1[0] = capsaMinima.pmin[0]+capsaMinima.a;
    p1[1] = capsaMinima.pmin[1]+capsaMinima.h;
    p1[2] = capsaMinima.pmin[2]+capsaMinima.p;

    // Calcul del vrp com el centre de la capsa minima contenedora 3D
    vrp[0] = (capsaMinima.pmin[0]+p1[0])/2.0;
    vrp[1] = (capsaMinima.pmin[1]+p1[1])/2.0;
    vrp[2] = (capsaMinima.pmin[2]+p1[2])/2.0;
    vrp[3] = 1.0;

    // Window
    window.pmin = vec2(-1, -1);
    window.a = 2;
    window.h = 2;

    // Viewport
    vp.a = a;
    vp.h = h;
    vp.pmin[0] = 0;
    vp.pmin[1] = 0;

    CalculaModelView();
    CalculaProjection();

}

void Camera::actualitzaCamera(Capsa3D capsaMinima) {
    vec3 p1;
    // Calcul del vrp com el centre de la capsa minima contenedora 3D
    p1[0] = capsaMinima.pmin[0]+capsaMinima.a;
    p1[1] = capsaMinima.pmin[1]+capsaMinima.h;
    p1[2] = capsaMinima.pmin[2]+capsaMinima.p;

    // Calcul del vrp com el centre de la capsa minima contenedora 3D
    vrp[0] = (capsaMinima.pmin[0]+p1[0])/2.0;
    vrp[1] = (capsaMinima.pmin[1]+p1[1])/2.0;
    vrp[2] = (capsaMinima.pmin[2]+p1[2])/2.0;
    vrp[3] = 1.0;


    CalculaModelView();

    CalculaWindow(capsaMinima);

    CalculaProjection();

}

/* Nomes es crida aquest metode si han canviat els angles */
void Camera::rotaCamera(Capsa3D capsa) {

    vec3 angles(angX, angY, 0.0);
    anglesToAxis(angles);

    origin = vrp + (length(origin-vrp)*w);
    CalculaModelView();
}

/* Pas a la GPU */

void Camera::toGPU(QGLShaderProgram *program)
{
// TO DO: A implementar a la fase 1 de la practica 2 i
    // a la fase 2 de la practica 2

    // obtenció de l'identificador de la posició de l'observador a GPU
    GLuint gl_IdObserverPosition = program->uniformLocation("observerPosition");
    GLuint gl_IdVRP = program->uniformLocation("vrp");

    // pas de la dada a GPU fent servir l'identificador
    glUniform4fv(gl_IdObserverPosition, 1, origin);
    glUniform4fv(gl_IdVRP, 1, vrp);

    // crida a les funcions per passar a GPU les matrius model-view i projection
    setModelView(program, model_view);
    setProjection(program, proj);
}

void Camera::setModelView(QGLShaderProgram *program, mat4 m)
{
 // TO DO: A implementar a la fase 2 de la practica 2

    // obtenció de l'identificador de la posició de l'observador a GPU
    GLuint model_view = program->uniformLocation("model_view");

    // pas de la dada a GPU fent servir l'identificador
    glUniformMatrix4fv(model_view, 1, GL_TRUE, modView);
}

void Camera::setProjection(QGLShaderProgram *program, mat4 p)
{
    // TO DO: A implementar a la fase 2 de la practica 2

    // obtenció de l'identificador de la posició de l'observador a GPU
    GLuint projection = program->uniformLocation("projection");

    // pas de la dada a GPU fent servir l'identificador
    glUniformMatrix4fv(projection, 1, GL_TRUE, proj);
}

void Camera::setViewport(int x, int y, int a, int h)
{
    vp.pmin[0] = x;
    vp.pmin[1] = y;
    vp.a = a;
    vp.h = h;
}

// Suposa que les dades d'obs, vrp i vup son correctes en la camera

void Camera::CalculaModelView()
{
    modView = LookAt(origin, vrp, vUp);

}

void Camera::CalculaProjection()
{
    int i, j;

    if (typeProjection==PARALLELA)
        proj = Ortho(window.pmin[0], window.pmin[0]+window.a, window.pmin[1], window.pmin[1]+window.h, dant, dpost );
    else
       proj = Frustum( window.pmin[0], window.pmin[0]+window.a, window.pmin[1], window.pmin[1]+window.h, dant, dpost );

}

///////////////////////////////////////////////////////////////////////////////
// convert Euler angles(x,y,z) to axis(side, up, forward)
// The order of rotation is Yaw->Pitch->Roll (Ry*Rx*Rz)
// Rx: rotation about X-axis, pitch
// Ry: rotation about Y-axis, yaw(heading)
// Rz: rotation about Z-axis, roll
//      Ry         Rx          Rz
// |Cy  0 -Sy| |1   0  0| | Cz Sz 0|   |CyCz-SySxSz  CySz+SySxCz  -SyCx|  <- side
// | 0  1   0|*|0  Cx Sx|*|-Sz Cz 0| = |-CxSz        CxCz         Sx   |  <- up
// |Sy  0  Cy| |0 -Sx Cx| |  0  0 1|   |SyCz+CySxSz  SySz-CySxCz  CyCx |  <- forward
///////////////////////////////////////////////////////////////////////////////
void Camera::anglesToAxis(vec3 angles)
{
    const float DEG2RAD = 3.141593f / 180;
    float sx, sy, sz, cx, cy, cz, theta;
    vec3 side, up, forward;

    // rotation angle about X-axis (pitch)

       if (angles[0]>90.0) {angles[0] = angles[0]-90.0; angles[2]=90;}
       if (angles[0]<-90.0) {angles[0] = angles[0]-90.0;angles[2]=-90;}


    theta = angles[0] * DEG2RAD;
    sx = sinf(theta);
    cx = cosf(theta);

    // rotation angle about Y-axis (yaw)
    theta = angles[1] * DEG2RAD;
    sy = sinf(theta);
    cy = cosf(theta);

    // rotation angle about Z-axis (roll)
    theta = angles[2] * DEG2RAD;
    sz = sinf(theta);
    cz = cosf(theta);

    // determine side vector
    side[0] = cy*cz - sy*sx*sz;
    side[1] = cy*sz + sy*sx*cz;
    side[2] = -sy*cx;
    u = side;
    u.w = 0.0;

    // determine up vector
    up[0] = -cx*sz;
    up[1] = cx*cz;
    up[2] = sx;
    v = up;
    v.w = 0.0;
    vUp = up;

    // determine forward vector
    forward[0] = sy*cz + cy*sx*sz;
    forward[1] = sy*sz - cy*sx*cz;
    forward[2] = cy*cx;
    w = forward;
    w.w = 0.0;
}


void  Camera::AmpliaWindow(double r)
{
  // Pre r = 1.5 => amplia a 150%
  double na, da;

  na  = window.a * (1.0 + r);
  da = na - window.a;
  window.pmin[0] = window.pmin[0] - 0.5*da;
  window.a  = na;
  na  = window.h * (1.0 + r);
  da = na - window.h;
  window.pmin[1] = window.pmin[1] - 0.5*da;
  window.h  = na;

  AjustaAspectRatioWd();
  CalculaProjection();
}


// Metode de calcul de la window a partir de la capsa contenidora
void Camera::CalculaWindow( Capsa3D c)
{
    vec4  vaux[8], vaux2[8];
    vec4 vup = vec4(0.0, 1.0, 0.0, 0.0);
    int i;

    modView = LookAt(origin, vrp, vup);

     /* Passo els 8 vertexs de la capsa per MVIS */
    VertexCapsa3D(c, vaux);

   for(i=0; i<8; i++) {
        vaux2[i]= modView*vaux[i];
    }
    window = CapsaMinCont2DXYVert(vaux2, 8);

    AmpliaWindow(0.05);      /* marges del 20%  */

}



// Donat un window i un viewport, amplia el window per tal que el seu
// aspect ratio sigui igual al del viewport

void    Camera::AjustaAspectRatioWd()
{

    double arvp, arwd;

    arvp = ((double) vp.h)/((double)(vp.a));
    arwd = window.h/window.a;
    if(arwd > arvp) {
        window.a = window.h/arvp;
    }
    else if (arwd <arvp) {
        window.h = window.a *arvp;

    }
    window.pmin[0] = -0.5*window.a;
    window.pmin[1] = -0.5*window.h;
}



Capsa2D  Camera::CapsaMinCont2DXYVert( vec4 *v, int nv)
{
    Capsa2D c;
    vec2    pmin, pmax;
    int     i;

    pmin[0] = v[0][0];       pmin[1] = v[0][1];
    pmax[0] = v[0][0];       pmax[1] = v[0][1];
    for(i=1; i<nv; i++) {
      if(v[i][0] <pmin[0])
        pmin[0] = v[i][0];
      if(v[i][1] <pmin[1])
        pmin[1] = v[i][1];
      if(v[i][0] >pmax[0])
        pmax[0] = v[i][0];
      if(v[i][1] >pmax[1])
        pmax[1] = v[i][1];
    }

    c.a = pmax[0]-pmin[0];
    c.h = pmax[1]-pmin[1];
    c.pmin[0] = -c.a/2.0;
    c.pmin[1] = -c.h/2.0;
    return c;
}


void Camera::VertexCapsa3D(Capsa3D capsaMinima, vec4 vaux[8])
{
    vec3 ptfi;

    ptfi[0] = capsaMinima.pmin[0]+capsaMinima.a;
    ptfi[1] = capsaMinima.pmin[1]+capsaMinima.h;
    ptfi[2] = capsaMinima.pmin[2]+capsaMinima.p;

    vaux[0] = vec4(capsaMinima.pmin[0], capsaMinima.pmin[1], capsaMinima.pmin[2], 1.0);
    vaux[1] = vec4(capsaMinima.pmin[0], capsaMinima.pmin[1], ptfi[2], 1.0);
    vaux[2] = vec4(capsaMinima.pmin[0],  ptfi[1], capsaMinima.pmin[2],1.0);
    vaux[3] = vec4(capsaMinima.pmin[0], ptfi[1], ptfi[2], 1.0);
    vaux[4] = vec4(ptfi[0],capsaMinima.pmin[1], capsaMinima.pmin[2],1.0);
    vaux[5] = vec4(ptfi[0], capsaMinima.pmin[1], ptfi[2],1.0);
    vaux[6] = vec4(ptfi[0], ptfi[1], capsaMinima.pmin[2], 1.0);
    vaux[7] = vec4(ptfi[0], ptfi[1], ptfi[2], 1.0);
}
