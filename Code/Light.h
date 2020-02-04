#ifndef Light_H
#define Light_H

#include <Common.h>

// Tipus de Lights
enum LightType {Puntual, Direccional, Spot};

// Classe que representa els atributs d'una Light
class Light {
    public:
        Light(LightType l);

        Light(vec3 diffuse, vec3 ambient, vec3 specular, vec3 attenuation_coefficients);

        virtual ~Light();

        vec4 getLightPosition();
        void setLightPosition(vec4 v);

        void switchOnOff();

        vec3 getIa() const;
        void setIa(const vec3 &value);

        vec3 getDiffuseIntensity();
        void setDiffuseIntensity(vec3 i);

        vec3 getIs() const;
        void setIs(const vec3 &value);

        vec3 getCoeficients() const;
        void setCoeficients(const vec3 &value);

        bool getEstaActivat() const;
        void setEstaActivat(bool value);

        LightType getTipusLight() const;
        void setTipusLight(const LightType &value);


        // calcula el factor d'atenuació de la intensitat lumínica per a la distància passada
        float obtainAttenuationFactorAtDistance(float dist);


protected:

        // component difusa
        vec3 diffuse;

        // component ambient
        vec3 ambient;

        // component especular
        vec3 specular;

        // coeficients a,b,c d'atenuació amb la distància
        vec3 attenuation_coefficients;

        // indicador de si la llum està activa
        bool isActive;

        // tipus de llum
        LightType lightType;


        // posició de la llum
        // nota: seria més adequat tenir aquest atribut només a les subclasses
        // que el necessitin, però per respectar l'estructura del projecte base
        // amb el getter i setter de la posició ens veiem forçats a posar-ho aquí
        vec4 position;
};

#endif // Light_H
