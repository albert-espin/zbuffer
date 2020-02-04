#include "SpotLight.h"

SpotLight::SpotLight(vec3 diffuse, vec3 ambient, vec3 specular, vec3 attenuation_coefficients, vec3 position, vec3 direction, float angle) : Light(diffuse, ambient, specular, attenuation_coefficients)
{
    this->position = position;
    this->direction = direction;
    this->angle = angle;
    this->lightType = LightType::Spot;
}

SpotLight::~SpotLight()
{

}

vec3 SpotLight::getDirection()
{
    return direction;
}

void SpotLight::setDirection(vec3 direction)
{
    this->direction = direction;
}


float SpotLight::getAngle()
{
    return angle;
}

void SpotLight::setAngle(float angle)
{
    this->angle = angle;
}
