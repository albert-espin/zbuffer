#include "Scene.h"


// màxim nombre de llums a l'escena
#define MAX_LIGHTS 20

/**
 * @brief Scene::Scene
 */
Scene::Scene() {
    // Inicialització de la capsa minima
    capsaMinima.pmin = vec3(-1.0, -1.0,-1.0);
    capsaMinima.a = 2;
    capsaMinima.h = 2;
    capsaMinima.p = 2;
    lightAmbientGlobal = vec3(0.1, 0.1, 0.1);

    isEnvironmentVisible = false;
}

/**
 * @brief Scene::~Scene
 */
Scene::~Scene() {
    elements.clear();
    lights.clear();
}

void Scene::createEnvironmentObject(QGLShaderProgram * environmentProgram){

    // creació de l'objecte de fons
    environmentObject = new Object(100000, "../Practica2_Base-master/resources/models/background.obj", new Material());
    //environmentObject = new Object(100000, "../Practica2_Base-master/resources/models/sphereSky.obj", new Material());

    this->environmentProgram = environmentProgram;
    environmentObject->toGPU(environmentProgram);
}

/**
 * @brief Scene::addObject
 * @param obj
 */
void Scene::addObject(Object *obj) {
    elements.push_back(obj);
    calculCapsaMinCont3DEscena();
}

/**
 * @brief Scene::draw
 */
void Scene::draw() {
    for(unsigned int i=0; i < elements.size(); i++){
        elements.at(i)->draw();
    }

    if (isEnvironmentVisible){
        environmentObject->toGPU(environmentProgram);
        environmentObject->draw();
    }
}

/**
 * @brief Scene::drawTexture
 */
void Scene::drawTexture() {
    for(unsigned int i=0; i < elements.size(); i++){
        elements.at(i)->drawTexture();
    }
    if (isEnvironmentVisible){
        environmentObject->drawTexture();
    }
}

/**
 * @brief Scene::getLightActual
 * @return
 */
Light* Scene::getLightActual() {
    // TO DO OPCIONAL: A modificar si es vol canviar el comportament de la GUI
    // Ara per ara dona com a Light actual la darrera que s'ha inserit
    return (lights[lights.size()-1]);
}

/**
 * @brief Scene::setLightActual
 * @param l
 */
void Scene::setLightActual(Light* l){
    lights[lights.size()-1]=l;
}

void Scene::toGPU(QGLShaderProgram *program, bool isTextureEnabled, bool isBumpMappingEnabled)
{
    for (Object * object : elements){
        object->toGPU(program);
    }

    lightsToGPU(program);
    setAmbientToGPU(program);

    // s'envia a GPU si volem o no activar la textura i el bump mapping

    // Obtenció dels identificadors de la GPU
    GLuint gl_IdIsTextureEnabled = program->uniformLocation("isTextureEnabled");
    GLuint gl_IdIsBumpMappingEnabled = program->uniformLocation("isBumpMappingEnabled");

    // Enviament de la dades, com enters
    glUniform1i(gl_IdIsTextureEnabled, isTextureEnabled);
    glUniform1i(gl_IdIsBumpMappingEnabled, isBumpMappingEnabled);
}



/**
 * @brief Scene::lightsToGPU
 * @param program
 */
void Scene::lightsToGPU(QGLShaderProgram *program){
// TO DO: A implementar a la fase 1 de la practica 2

    // Per a passar els diferents atributs del shader declarem una estructura amb els identificadors de la GPU
    struct gl_IdLight {
        GLuint idAmbient;
        GLuint idDiffuse;
        GLuint idSpecular;
        GLuint idAttenuationCoefficients;
        GLuint idType;
        GLuint idPosition;
        GLuint idDirection;
        GLuint idAngle;
    };

    // vector d'estructures on incloure les dades de cada llum activa de
    // l'escena per a passar-les a la GPU
    gl_IdLight gl_IdLights[MAX_LIGHTS];

    // comptador de llums amb informació ja enviada a la GPU
    int gpuLightCount = 0;

    // analitzem les llums a l'escena
    for (Light * light : lights){

        // si s'ha arribat al màxim permès, no es passen més llums a la GPU
        if (gpuLightCount >= MAX_LIGHTS) break;

        // només passarem informació a la GPU de les activades
        if (light->getEstaActivat()){

            // Preparació de les dades a enviar a GPU
            vec4 ambient = vec4(light->getIa(), 1);
            vec4 diffuse = vec4(light->getDiffuseIntensity(), 1);
            vec4 specular = vec4(light->getIs(), 1);
            vec3 attenuationCoefficients = light->getCoeficients();
            int type = (int)light->getTipusLight();

            // per respectar les funcions del codi base, tota llum té posició
            // tot i que només té sentit i s'utilitzarà realment en el cas puntual
            vec4 position = light->getLightPosition();

            // només les llums direccionals i de focus tenen direcció
            vec4 direction(0.f);
            if (dynamic_cast<DirectionalLight*>(light)){
                direction = vec4(((DirectionalLight*)light)->getDirection(), 0);
            }
            else if (dynamic_cast<SpotLight*>(light)){
                direction = vec4(((SpotLight*)light)->getDirection(), 0);
            }

            // només les llums de focus tenen angle d'obertura
            float angle = 0;
            if (dynamic_cast<SpotLight*>(light)){
                angle = ((SpotLight*)light)->getAngle();
            }


            // Obtenció dels identificadors de la GPU
            gl_IdLights[gpuLightCount].idAmbient = program->uniformLocation(QString("lights[%1].ambient").arg(gpuLightCount));
            gl_IdLights[gpuLightCount].idDiffuse = program->uniformLocation(QString("lights[%1].diffuse").arg(gpuLightCount));
            gl_IdLights[gpuLightCount].idSpecular = program->uniformLocation(QString("lights[%1].specular").arg(gpuLightCount));
            gl_IdLights[gpuLightCount].idAttenuationCoefficients = program->uniformLocation(QString("lights[%1].attenuationCoefficients").arg(gpuLightCount));
            gl_IdLights[gpuLightCount].idType = program->uniformLocation(QString("lights[%1].type").arg(gpuLightCount));
            gl_IdLights[gpuLightCount].idPosition = program->uniformLocation(QString("lights[%1].position").arg(gpuLightCount));
            gl_IdLights[gpuLightCount].idDirection = program->uniformLocation(QString("lights[%1].direction").arg(gpuLightCount));
            gl_IdLights[gpuLightCount].idAngle = program->uniformLocation(QString("lights[%1].angle").arg(gpuLightCount));


            // Bind de les zones de memòria que corresponen a la GPU a valors de les variables de la CPU
            glUniform4fv(gl_IdLights[gpuLightCount].idAmbient, 1, ambient);
            glUniform4fv(gl_IdLights[gpuLightCount].idDiffuse, 1, diffuse);
            glUniform4fv(gl_IdLights[gpuLightCount].idSpecular, 1, specular);
            glUniform3fv(gl_IdLights[gpuLightCount].idAttenuationCoefficients, 1, attenuationCoefficients);
            glUniform1i(gl_IdLights[gpuLightCount].idType, type);
            glUniform4fv(gl_IdLights[gpuLightCount].idPosition, 1, position);
            glUniform4fv(gl_IdLights[gpuLightCount].idDirection, 1, direction);
            glUniform1f(gl_IdLights[gpuLightCount].idAngle, angle);


            // augmentem el nombre de llums processades
            gpuLightCount++;
        }

    }

    // La direcció de la llum del toon shading és la de la darrera llum direccional
    // afegida (canviant el signe ja que canvia el sentit), o un valor arbitrari
    vec4 toonLightDir = vec4(.5,.3,.2,0);
    for (int i = lights.size() - 1; i >= 0; i--){
        if (dynamic_cast<DirectionalLight*>(lights[i])){
            toonLightDir = -normalizeNo4th(((DirectionalLight*)lights[i])->getDirection());
        }
    }

    // obtenció de l'identificador de la direcció de la llum per Toon a GPU
    GLuint gl_IdToonLightDir = program->uniformLocation("toonLightDir");

    // pas de la dada a GPU fent servir l'identificador
    glUniform4fv(gl_IdToonLightDir, 1, toonLightDir);
}

void Scene::addLight(Light *l) {
    lights.push_back(l);
}

void Scene::removeCurrentLight(){
    lights.pop_back();
}


/**
 * @brief Scene::setAmbientToGPU
 * @param program
 */
void Scene::setAmbientToGPU(QGLShaderProgram *program){
    // TO DO: A implementar a la fase 1 de la practica 2

    // Per a passar els diferents atributs del shader declarem una estructura amb els identificadors de la GPU
    struct {
        GLuint idIntensity;
    } gl_IdGlobalAmbientLight;

    // Obtenció dels identificadors de la GPU
    gl_IdGlobalAmbientLight.idIntensity = program->uniformLocation("globalAmbientLight.intensity");

    // Intensitat de la llum ambient global a passar a la GPU, com a vec4
    vec4 intensity = vec4(lightAmbientGlobal, 1);

    // Bind de les zones de memòria que corresponen a la GPU a valors de les variables de la CPU
    glUniform4fv(gl_IdGlobalAmbientLight.idIntensity, 1, intensity);
}



vec4 Scene::normalizeNo4th(vec4 vector){
    float w = vector[3];
    vec3 xyz = vec3(vector[0], vector[1], vector[2]);
    xyz = normalize(xyz);
    return vec4(xyz[0], xyz[1], xyz[2], w);
}


void Scene::calculCapsaMinCont3DEscena()
{
    Capsa3D c;
    vec3 pmax;

    if (elements.size()==1) {
        capsaMinima = elements[0]->calculCapsa3D();
        pmax[0] = capsaMinima.pmin[0]+capsaMinima.a;
        pmax[1] = capsaMinima.pmin[1]+capsaMinima.h;
        pmax[2] = capsaMinima.pmin[2]+capsaMinima.p;
    } else {
        capsaMinima.pmin[0]=200;
        capsaMinima.pmin[1]=200;
        capsaMinima.pmin[2]=200;
        pmax[0] = -200;
        pmax[1] = -200;
        pmax[2] = -200;
    }

    for (unsigned int i=0; i<elements.size(); i++) {
       c = elements[i]->calculCapsa3D();

       if (capsaMinima.pmin[0]>c.pmin[0]) capsaMinima.pmin[0] = c.pmin[0];
       if (capsaMinima.pmin[1]>c.pmin[1]) capsaMinima.pmin[1] = c.pmin[1];
       if (capsaMinima.pmin[2]>c.pmin[2]) capsaMinima.pmin[2] = c.pmin[2];
       if (pmax[0]<c.pmin[0]+c.a) pmax[0] = c.pmin[0]+c.a;
       if (pmax[1]<c.pmin[1]+c.h) pmax[1] = c.pmin[1]+c.h;
       if (pmax[2]<c.pmin[2]+c.p) pmax[2] = c.pmin[2]+c.p;
    }
    capsaMinima.a = pmax[0]-capsaMinima.pmin[0];
    capsaMinima.h = pmax[1]-capsaMinima.pmin[1];
    capsaMinima.p = pmax[2]-capsaMinima.pmin[2];
}
