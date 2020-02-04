#ifndef POINTLIGHT_H
#define POINTLIGHT_H
#include "Light.h"

// Llum puntual, ubicada en una posició a l'espai
// nota: mantenim a Light les funcions sobre l'atribut de posició
// (i per tant també l'atribut) per respectar l'estructura del projecte
// base, tot i que si no haguéssim de mantenir les funcions que ens donen
// tal i com estan a l'inici ho mouríem aquí, té més sentit que la posició
// sigui atribut de PointLight
class PointLight : public Light
{
public:

    PointLight(vec3 diffuse, vec3 ambient, vec3 specular, vec3 attenuation_coefficients, vec3 position);

    virtual ~PointLight();

};

#endif // POINTLIGHT_H
