#ifndef MATERIAL_H
#define MATERIAL_H

#include <Common.h>
#include <QGLShaderProgram>


// Classe que representa els materials d'un objecte
class Material {


public:
    Material();

     Material(vec3 diffuse, vec3 ambient, vec3 specular, float shininess, float alpha = 1.f);

    void toGPU(QGLShaderProgram *program);



    // Retorna la component difusa
    vec3 getDiffuse() const;

    // Retorna la component ambient
    vec3 getAmbient() const;

    // Retorna la component especular
    vec3 getSpecular() const;

    // Retorna la shininess
    float getShininess() const;

    // Retorna l'alfa
    float getAlpha() const;


protected:

    // component difusa
    vec3 diffuse;

    // component ambient
    vec3 ambient;

    // component especular
    vec3 specular;

    // exponent de reflexió especular
    float shininess;

    // alfa per codificar transparència (0) o opacitat(1)
    float alpha;

};

#endif // MATERIAL_H
