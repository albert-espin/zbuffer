#ifndef SCENE_H
#define SCENE_H

#include <cmath>
#include <iostream>
#include <stdlib.h>

#include <list>
#include <vector>
#include <string>
#include <stdio.h>

#include <Common.h>
#include <Object.h>
#include <Light.h>
#include <PointLight.h>
#include <DirectionalLight.h>
#include <SpotLight.h>


using namespace std;

// Scene: contains all objects, lights to visualize the scene
// the viewer is placed in the z+ axis and the  visible frustum is enclosed to (-1,-1,-1) to (1,1,1)

class Scene {

public:
    Scene();
    ~Scene();

    void addObject(Object *obj);


    void draw();
    void drawTexture();

    void   addLight(Light *l);

    // elimina la llum marcada com a actual
    void removeCurrentLight();

    Light *getLightActual();
    void   setLightActual(Light* l);

    // mètode per enviar totes les dades de l'escena a GPU
    void toGPU(QGLShaderProgram *program, bool isTextureEnabled, bool isBumpMappingEnabled);
    void lightsToGPU(QGLShaderProgram *program);
    void setAmbientToGPU(QGLShaderProgram *program);

    // Normalitza un vector de 4 components sense alterar la quarta component,
    // aplicant la normalització només a les tres primeres components
    vec4 normalizeNo4th(vec4 vector);

    // crea l'objecte de decoració del fons
    void createEnvironmentObject(QGLShaderProgram *environmentProgram);

    vector<Object*> elements;
    vector<Light*>  lights;

    vec3    lightAmbientGlobal;

    // Capsa contenedora de l'escena
    Capsa3D capsaMinima;
    void    calculCapsaMinCont3DEscena();

    // objecte que simula el fons de l'escena
    Object * environmentObject;

    // punter al programa d'enviromental mapping
    QGLShaderProgram * environmentProgram;

    // indicador de si l'entorn és visible
    bool isEnvironmentVisible;

};

#endif // SCENE_H
