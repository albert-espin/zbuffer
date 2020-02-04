#if __VERSION__<130
#define IN varying
#define OUT varying
#else
#define IN in
#define OUT out
#endif

#define PI 3.1415926535897932384626433832795

// mapa de la textura de fons de l'escena
uniform sampler2D envTexMap;


// posició, cal per calcular les coordenades de textura
IN vec4 position;



// Funció principal que es crida en iniciar el programa
void main()
{

    // càlcul de les coordenades de textura (u, v)

    // vector unitari que va del centre de la capsa mínima al vèrtex, ignorant
    // la 4a component per no distorsionar la normalització; com s'ens fa assumir
    // que la capsa mínima és al punt (0,0,0), no cal ni tan sols tenir-la en compte
    // ja que restar a la posició el punt (0,0,0) no canviaria res
    vec3 centerToVertexVect = normalize(position.xyz);

    float u = 0.5 - atan(centerToVertexVect.z, centerToVertexVect.x) / (2. * PI);
    float v = 0.5 - asin(centerToVertexVect.y) / PI;

    // encapsulació de les coordenades en un vector bidimensional
    vec2 texCoords = vec2(u, v);

    // el color final és el de la textura en la posició del píxel/fragment
    gl_FragColor = texture2D(envTexMap, texCoords);

}
