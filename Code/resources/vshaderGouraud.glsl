#if __VERSION__<130
#define IN attribute
#define OUT varying
#else
#define IN in
#define OUT out
#endif

// màxim nombre de llums a l'escena
#define MAX_LIGHTS 20


// nombre pi
#define PI 3.1415926535897932384626433832795


// posició d'un vèrtex
IN vec4 vPosition;

// normal del vèrtex
IN vec4 vNormal;

// VRP de la càmera, que coincideix amb el centre de la caixa mínima
uniform vec4 vrp;

// posició de l'observador
uniform vec4 observerPosition;

// estructura amb els valors de les components/dades d'un material, a usar
// per determinar el color
struct Mat {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
    float alpha;
};
uniform Mat mat;



// Estructura per representar una llum; s'indica, a més de les intensitats difusa,
// ambient i especular, el tipus de llum i valors per a diferents tipus de llums
// (posició, direcció i angle d'obertura)
struct Light
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec3 attenuationCoefficients; // a, b i c
    int type; // puntual (0), direccional (1) o focus (2)
    vec4 position;
    vec4 direction;
    float angle;
};

// vector amb les llums de l'escena
uniform Light lights[MAX_LIGHTS];


// estructura per a representar la llum d'ambient global i en particular
// la seva intensitat
struct GlobalAmbientLight
{
    vec4 intensity;
};

uniform GlobalAmbientLight globalAmbientLight;


// matrius relacionades amb els càlculs de la càmera
uniform mat4 model_view;
uniform mat4 projection;


// indicador de si la textura s'ha de visualitzar o no
uniform bool isTextureEnabled;

// indicador de si el bump map s'ha d'aplicar o no
uniform bool isBumpMappingEnabled;

// mapa de la textura difusa
uniform sampler2D texMap;

// bump map de la textura
uniform sampler2D bumpMap;

// color que surt del vertex shader
OUT vec4 color;

// posició per fer el càlcul de les coordenades de textura
OUT vec4 position;


// Normalitza un vector de 4 components sense alterar la quarta component,
// aplicant la normalització només a les tres primeres components
vec4 normalizeNo4th(vec4 vector){
    float w = vector[3];
    vec3 xyz = vec3(vector[0], vector[1], vector[2]);
    xyz = normalize(xyz);
    return vec4(xyz[0], xyz[1], xyz[2], w);
}


// Calcula la normal modificada pel bump map
vec4 calculateBumpedNormal(){

    // càlcul de les coordenades del bump map
    vec3 centerToVertexVect = normalize(vPosition.xyz - vrp.xyz);
    float u = 0.5 - atan(centerToVertexVect.z, centerToVertexVect.x) / (2. * PI);
    float v = 0.5 - asin(centerToVertexVect.y) / PI;
    vec2 texCoords = vec2(u, v);

    // càlcul del bump o pertorbació
    vec3 bump = texture2D(bumpMap, texCoords).xyz;

    // canvi de rang de valors del bump de [0,1] a [-1,1]
    bump = normalize(bump * 2. - 1.);

    // obtenció de la normal amb pertorbació (s'aplica una certa ponderació
    // trobada experimentalment per a obtenir resultats més atractius visualment)
    return 0.5 * vNormal + 0.5 * vec4(bump, 0);
}

// funció per calcular el color amb la tècnica de Blinn-Phong
vec4 blinnPhong() {


    // normal modificada amb les pertorbacions del bump map (si està activat)
    vec4 bumpedNormal;
    if (isTextureEnabled && isBumpMappingEnabled)
        bumpedNormal = calculateBumpedNormal();
    else
        bumpedNormal = vNormal;

    // aplicació de la il·luminació ambient global
    vec4 intensity = globalAmbientLight.intensity * mat.ambient;

    // vector del vèrtex a l'observador
    vec4 V = normalizeNo4th(observerPosition - vPosition);

    // cosinus de l'angle entre el vector de visió i la normal al vèrtex
    float visionAndNormalAngleCos = dot(-V, vNormal);

    // versió modificada de la component difusa del material per donar èmfasi
    // a la silueta de l'objecte al qual pertany el vèrtex
    vec4 emphasizedDiffuse = mat.diffuse * (1 - visionAndNormalAngleCos);

    // aplicació de la contribució de cada llum de l'escena
     for (int i = 0; i < MAX_LIGHTS; i++) {

        float a = lights[i].attenuationCoefficients[0];
        float b = lights[i].attenuationCoefficients[1];
        float c = lights[i].attenuationCoefficients[2];

        // vector del vèrtex a la llum
        vec4 L;

        // només hi ha atenuació si la llum és puntual o de focus (altrament, com no
        // té una posició origen definit, no té sentit el concepte de distància)
        float attenuation;

        // si la llum té posició (és puntual o de focus)
        if (lights[i].type == 0 || lights[i].type == 2){

            // com la llum té posició, podem fer-la servir per calcular L
            L = (lights[i].position - vPosition);

            // distància de la llum al vèrtex
            float d = abs(length(L));

            // normalització del vector L
            L = normalizeNo4th(L);

            // factor d'atenuació en funció de la distància
            attenuation = clamp(1. / (a + b*d + c*d*d), 0., 1.);

            // si tenim un focus, mirem que estiguem dins del con, o no contribuirà la llum
            if (lights[i].type == 2){

                // calculem l'angle entre el raig de llum i el con del focus
                float angle = degrees(acos(dot(-L, normalizeNo4th(lights[i].direction))));

                // si l'angle està més enllà del màxim (el del con del focus) no es veurà il·luminat, tret de l'ambient
                if (angle > lights[i].angle){
                  attenuation = 0.0;
                  // només apliquem la component ambient
                  intensity += lights[i].ambient * mat.ambient;
                }
            }
        }

        // cas direccional
        else{

            // el vector L és l'oposat de la direcció de la llum (sentit contrari)
            L = normalizeNo4th(-lights[i].direction);

            attenuation = 1.;
        }

        // vector H, el vector mig entre L i V
        vec4 H = normalizeNo4th(L + V);

        // aplicació de la contribució de la llum i-èssima, en ambient, difús i especular
        intensity += attenuation * (lights[i].ambient * mat.ambient + lights[i].diffuse * emphasizedDiffuse * max(0., dot(L, bumpedNormal)) + lights[i].specular * mat.specular * pow(max(0., dot(bumpedNormal, H)), mat.shininess));

    }

    // ús de l'alfa a la quarta component
    intensity = vec4(intensity[0], intensity[1], intensity[2], mat.alpha);

    return intensity;
}


// Funció principal que es crida en iniciar el programa
void main()
{
    // es calcula la posició final fent servir la posició del vèrtex i les matrius model-view i projection de la càmera
    gl_Position = projection * model_view * vPosition;

    position = vPosition;

    // Càlcul de la iluminació amb la fórmula de Blinn-Phong
    color = blinnPhong();

}
