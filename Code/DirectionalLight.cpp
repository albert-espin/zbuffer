#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(vec3 diffuse, vec3 ambient, vec3 specular, vec3 direction) : Light(diffuse, ambient, specular, vec3(1))
{
    this->direction = direction;
    this->lightType = LightType::Direccional;
}

DirectionalLight::~DirectionalLight()
{

}

vec3 DirectionalLight::getDirection()
{
    return direction;
}

void DirectionalLight::setDirection(vec3 direction)
{
    this->direction = direction;
}
