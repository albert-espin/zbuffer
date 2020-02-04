#if __VERSION__<130
#define IN varying
#define OUT varying
#else
#define IN in
#define OUT out
#endif


// nombre pi
#define PI 3.1415926535897932384626433832795


IN vec4 color;


// posició per fer el càlcul de les coordenades de textura
IN vec4 position;

// VRP de la càmera, que coincideix amb el centre de la caixa mínima
uniform vec4 vrp;

// indicador de si la textura s'ha de visualitzar o no
uniform bool isTextureEnabled;

// mapa de la textura
uniform sampler2D texMap;


// Funció principal que es crida en iniciar el programa
void main()
{

    // NOTA: Aquí a Gouraud també es calcula el color de textura al fragment
    // shader i no al vèrtex perquè fent-ho allà la qualitat era molt baixa
    // i requereix malles poligonals amb molts vèrtexs per a resultats acceptables.
    // Això suposa que aquí el color de textura no s'aplica contra la component
    // difusa sinó contra tot el color Blinn-Phong, que arriba del vertex shader
    // ja calculat

    // el color final es veu influït per si la textura està activada o no
    if (!isTextureEnabled){

        // si està desactivada, només el material, les llums i la càmera,
        // en definitiva, el color Blinn-Phong, és el que es dóna al píxel/
        // fragment
        gl_FragColor = color;
    }

    // altrament, si la textura està activada, hi ha una ponderació entre
    // el color d'aquesta i el de Blinn-Phong per determinar el color del
    // píxel/fragment
    else{

        // càlcul de les coordenades de textura (u, v)

        // vector unitari que va del centre de la capsa mínima, coindicent amb
        // el vector VRP de la càmera, al vèrtex
        vec3 centerToVertexVect = normalize(position.xyz - vrp.xyz);

        float u = 0.5 - atan(centerToVertexVect.z, centerToVertexVect.x) / (2. * PI);
        float v = 0.5 - asin(centerToVertexVect.y) / PI;

        // encapsulació de les coordenades en un vector bidimensional
        vec2 texCoords = vec2(u, v);

        gl_FragColor = 0.4 * color + 0.6 * texture2D(texMap, texCoords);
    }
}

