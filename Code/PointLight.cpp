#include "PointLight.h"

PointLight::PointLight(vec3 diffuse, vec3 ambient, vec3 specular, vec3 attenuation_coefficients, vec3 position) : Light(diffuse, ambient, specular, attenuation_coefficients)
{
    this->position = vec4(position, 1);
    this->lightType = LightType::Puntual;
}

PointLight::~PointLight()
{

}

