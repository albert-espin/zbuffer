#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include "Light.h"

// Llum que simula un focus
class SpotLight : public Light
{
public:
    SpotLight(vec3 diffuse, vec3 ambient, vec3 specular, vec3 attenuation_coefficients, vec3 position, vec3 direction, float angle);
    virtual ~SpotLight();

    vec3 getDirection();
    void setDirection(vec3 direction);

    float getAngle();
    void setAngle(float angle);

protected:

    // direcció de la llum
    vec3 direction;

    // angle d'obertura
    float angle;
};


#endif // SPOTLIGHT_H
