#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

#include "Light.h"

// Llum direccional, amb focus a l'infinit
class DirectionalLight : public Light
{
public:
    DirectionalLight(vec3 diffuse, vec3 ambient, vec3 specular, vec3 direction);
    virtual ~DirectionalLight();

    vec3 getDirection();
    void setDirection(vec3 direction);

protected:

    // direcció de la llum
    vec3 direction;
};

#endif // DIRECTIONALLIGHT_H
