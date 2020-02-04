#include "Light.h"

/**
 * @brief Light::Light
 * @param t
 */
Light::Light(LightType t) {

    lightType = t;
}

Light::Light(vec3 diffuse, vec3 ambient, vec3 specular, vec3 attenuation_coefficients)
{
    this->diffuse = diffuse;
    this->ambient = ambient;
    this->specular = specular;
    this->attenuation_coefficients = attenuation_coefficients;

    // la llum comença activa per defecte
    this->isActive = true;
}

Light::~Light()
{

}




/**
 * @brief Light::getDiffuseIntensity
 * @return
 */
vec3 Light::getDiffuseIntensity() {


   return diffuse;
}

/**
 * @brief Light::setDiffuseIntensity
 * @param i
 */
void Light::setDiffuseIntensity(vec3 i) {

    // el float que es rep ha de multiplicar els tres valors de la intensitat difusa de la Light

    diffuse = i;
}
vec4 Light::getLightPosition() {
    return position;
}

void Light::setLightPosition(vec4 v) {
    this->position = v;
}

/**
 * @brief Light::switchOnOff
 */
void Light::switchOnOff() {

    // Si esta en on la posa a off i a l'inreves
 }

/**
 * @brief Light::getIa
 * @return
 */
vec3 Light::getIa() const
{


    return ambient;
}

/**
 * @brief Light::setIa
 * @param value
 */
void Light::setIa(const vec3 &value)
{

    // el float que es reb ha de multiplicar els tres valors de la intensitat difusa de la Light

    ambient = value;
}



/**
 * @brief Light::getIs
 * @return
 */
vec3 Light::getIs() const
{


    return specular;
}

/**
 * @brief Light::setIs
 * @param value
 */
void Light::setIs(const vec3 &value)
{

    // el float que es reb ha de multiplicar els tres valors de la intensitat difusa de la Light

    specular = value;
}

/**
 * @brief Light::getCoeficients
 * @return
 */
vec3 Light::getCoeficients() const
{

    return attenuation_coefficients;
}

/**
 * @brief Light::setCoeficients
 * @param value
 */
void Light::setCoeficients(const vec3 &value)
{

    // el float que es rep ha de multiplicar els tres valors de la intensitat difusa de la Light

    attenuation_coefficients = value * diffuse;
}

/**
 * @brief Light::getEstaActivat
 * @return
 */
bool Light::getEstaActivat() const
{

       return isActive;
}

/**
 * @brief Light::setEstaActivat
 * @param value
 */
void Light::setEstaActivat(bool value)
{

    isActive = value;
}

/**
 * @brief Light::getTipusLight
 *
 */
LightType Light::getTipusLight() const
{
    return lightType;
}
/**
 * @brief Light::setTipusLight
 * @param value
 */
void Light::setTipusLight(const LightType &value)
{

    lightType = value;
}

float Light::obtainAttenuationFactorAtDistance(float dist)
{
    // el factor es calcula com 1 / (a + b*d + c*d*d)
   return 1.f / (attenuation_coefficients[0] + attenuation_coefficients[1] * dist + attenuation_coefficients[2] * dist * dist);
}
