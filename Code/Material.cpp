#include "Material.h"


Material::Material() {

}

Material::Material(vec3 diffuse, vec3 ambient, vec3 specular, float shininess, float alpha)
{
    this->diffuse = diffuse;
    this->ambient = ambient;
    this->specular = specular;
    this->shininess = shininess;
    this->alpha = alpha;
}

/**
 * Passa el material de CPU a GPU
 * @brief Material::toGPU
 * @param program
 */
void Material::toGPU(QGLShaderProgram *program){

    // Per a passar els diferents atributs del shader declarem una estructura amb els identificadors de la GPU
    struct {
        GLuint idAmbient;
        GLuint idDiffuse;
        GLuint idSpecular;
        GLuint idShininess;
        GLuint idAlpha;
    } gl_IdMat;

    // Obtenció dels identificadors de la GPU
    gl_IdMat.idAmbient = program->uniformLocation("mat.ambient");
    gl_IdMat.idDiffuse = program->uniformLocation("mat.diffuse");
    gl_IdMat.idSpecular = program->uniformLocation("mat.specular");
    gl_IdMat.idShininess = program->uniformLocation("mat.shininess");
    gl_IdMat.idAlpha = program->uniformLocation("mat.alpha");


    // variables vec4 pels vectors de colors, aplicant l'alfa, per adequar-ho a com ho vol la GPU
    vec4 amb = vec4(ambient, 1);
    vec4 diff = vec4(diffuse, 1);
    vec4 spec = vec4(specular, 1);

    // Bind de les zones de memòria que corresponen a la GPU a valors de les variables de la CPU
    glUniform4fv(gl_IdMat.idAmbient, 1, amb);
    glUniform4fv(gl_IdMat.idDiffuse, 1, diff);
    glUniform4fv(gl_IdMat.idSpecular, 1, spec);
    glUniform1f(gl_IdMat.idShininess, shininess);
    glUniform1f(gl_IdMat.idAlpha, alpha);



    // obtenció de l'identificador de l'alfa per Toon a GPU (Toon només necessita
    // això del material, per això es passa sola l'alfa, per no haver de posar
    // al shader de Toon tot el material)
    GLuint gl_IdToonAlpha = program->uniformLocation("toonAlpha");

    // pas de la dada a GPU fent servir l'identificador
    glUniform1f(gl_IdToonAlpha, alpha);

}





vec3 Material::getDiffuse() const
{
    return diffuse;
}

vec3 Material::getAmbient() const
{
    return ambient;
}

vec3 Material::getSpecular() const
{
    return specular;
}

float Material::getShininess() const
{
    return shininess;
}

float Material::getAlpha() const
{
    return alpha;
}
