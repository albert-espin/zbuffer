#if __VERSION__<130
#define IN varying
#define OUT varying
#else
#define IN in
#define OUT out
#endif

#define PI 3.1415926535897932384626433832795

/*
 NOTA: Com Toon shading no fa servir el material ni les llums (tret de la direcció
d'una llum direccional de l'escena), algunes modificacions a fer a tots els shaders
no s'han aplicat a Toon shading de la mateixa manera que als altres pel simple fet
que aquí no té sentit fer servir el material o les llums pels càlculs. Per això, com
l'alfa és una propietat del material i a Toon no s'usen els materials, s'ha considerat
més lògic passar l'alfa sola (toonAlpha) que el material sencer al shader. En quant a
l'èmfasi de siluetes, s'ha dedicit aplicar-lo de forma diferent que als altres shadings,
on s'aplicava un càlcul a la component difusa per emfasitzar-la; aquí es dóna èmfasi
directament al color de Toon, cosa que ha requerit afegir el valor d'entrada observerPosition
*/


// VRP de la càmera, que coincideix amb el centre de la caixa mínima
uniform vec4 vrp;

// direcció de la llum
uniform vec4 toonLightDir;

// alfa a aplicar a Toon shading (la passem sola i no tot el material perquè no ens calen
// més components)
uniform float toonAlpha;

// posició de l'observador (per poder calcular el vector V per l'èmfasi)
uniform vec4 observerPosition;

IN vec4 normal;

// indicador de si la textura s'ha de visualitzar o no
uniform bool isTextureEnabled;

// indicador de si el bump map s'ha d'aplicar o no
uniform bool isBumpMappingEnabled;

// mapa de la textura difusa
uniform sampler2D texMap;

// bump map de la textura
uniform sampler2D bumpMap;

// posició, cal per calcular l'èmfasi
IN vec4 position;

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
    vec3 centerToVertexVect = normalize(position.xyz - vrp.xyz);
    float u = 0.5 - atan(centerToVertexVect.z, centerToVertexVect.x) / (2. * PI);
    float v = 0.5 - asin(centerToVertexVect.y) / PI;
    vec2 texCoords = vec2(u, v);

    // càlcul del bump o pertorbació
    vec3 bump = texture2D(bumpMap, texCoords).xyz;

    // canvi de rang de valors del bump de [0,1] a [-1,1]
    bump = normalize(bump * 2. - 1.);

    // obtenció de la normal amb pertorbació (s'aplica una certa ponderació
    // trobada experimentalment per a obtenir resultats més atractius visualment)
    return 0.5 * normal + 0.5 * vec4(bump, 0);
}


// Funció principal que es crida en iniciar el programa
void main()
{
    float intensity;
    vec4 color;
    intensity = dot(toonLightDir, normalizeNo4th(normal));

    // vector del vèrtex a l'observador
    vec4 V = normalizeNo4th(observerPosition - position);

    // normal modificada amb les pertorbacions del bump map (si està activat)
    vec4 bumpedNormal;
    if (isTextureEnabled && isBumpMappingEnabled)
        bumpedNormal = calculateBumpedNormal();
    else
        bumpedNormal = normal;

    // cosinus de l'angle entre el vector de visió i la normal al vèrtex
    float visionAndNormalAngleCos = dot(-V, bumpedNormal);

    // versió modificada de la intensitat per donar èmfasi a la silueta de
    // l'objecte al qual pertany el vèrtex
    intensity *= (1 - visionAndNormalAngleCos);

    if (intensity > 0.95)
        color = vec4(1.0,0.5,0.5, toonAlpha);
    else if (intensity > 0.5)
        color = vec4(0.6,0.3,0.3, toonAlpha);
    else if (intensity > 0.25)
        color = vec4(0.4,0.2,0.2, toonAlpha);
    else
        color = vec4(0.2,0.1,0.1, toonAlpha);


    // el color final es veu influït per si la textura està activada o no
    if (!isTextureEnabled){

        // si està desactivada, només el material, les llums i la càmera,
        // en definitiva, el color Toon, és el que es dóna al píxel/fragment
        gl_FragColor = color;
    }

    // altrament, si la textura està activada, hi ha una ponderació entre
    // el color d'aquesta i el de Toon per determinar el color del píxel/fragment
    else{

        // càlcul de les coordenades de textura (u, v)

        // vector unitari que va del centre de la capsa mínima, coindicent amb
        // el vector VRP de la càmera, al vèrtex
        vec3 centerToVertexVect = normalize(position.xyz - vrp.xyz);

        float u = 0.5 - atan(centerToVertexVect.z, centerToVertexVect.x) / (2. * PI);
        float v = 0.5 - asin(centerToVertexVect.y) / PI;

        // encapsulació de les coordenades en un vector bidimensional
        vec2 texCoords = vec2(u, v);

        // ponderació entre color Toon i color de textura per calcular
        // el color final
        gl_FragColor = 0.8 * color + 0.2 * texture2D(texMap, texCoords);
    }
}
