#ifndef Object_H
#define Object_H

#include <QObject>
#include <vector>
#include <Common.h>
#include <cara.h>
#include <QGLShaderProgram>
#include <QOpenGLTexture>
#include "Material.h"

using namespace std;

typedef Common::vec4  point4;

// Classe que conte la representacio d'un Objecte: geometria, topologia, material i textures
class Object : public QObject {

    Q_OBJECT
protected:
    QString nom;
    vector<Cara> cares;
    vector<point4> vertexs;

    // taula hash que associa cada vèrtex amb la seva normal
    std::map<vec4_comparable, vec3> vertexNormalsMap;

    GLuint buffer;

    int     numPoints;
    point4 *points;
    point4 *normals;

    // Els colors s'usen en la primera execució però després son prescindibles
    //point4 *colors;

    int Index; // index de control del numero de vertexs a passar a la GPU

    QGLShaderProgram *program;



    // material de l'objecte
    Material * material;

    // textura difusa de l'objecte
    QOpenGLTexture * diffuseTexture;

    // textura normal de l'objecte
    QOpenGLTexture * normalTexture;

    // textura per l'objecte que fa de fons de l'escena
    QOpenGLTexture * environmentTexture;

public:

    Object(const int npoints, QObject *parent = 0);
    Object(const int npoints, QString n);


    // constructor que rep el material
    Object(const int npoints, QString n, Material * material);

    ~Object();

    virtual void readObj(QString filename);

    virtual void make();
    virtual void toGPU(QGLShaderProgram *p);
    virtual void toGPUTexture(QGLShaderProgram *pr);

    virtual void draw();
    virtual void drawTexture();

    Capsa3D calculCapsa3D();

    // calcula les normals de cada vèrtex com el promig de les normals de les
    // cares que el contenen
    void calculateVertexNormals();

private:

    void construeix_cara ( char **words, int nwords);
    void initTextura();
};


#endif // Object_H
