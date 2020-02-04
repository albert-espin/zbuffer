#if __VERSION__<130
#define IN attribute
#define OUT varying
#else
#define IN in
#define OUT out
#endif

// màxim nombre de llums a l'escena
#define MAX_LIGHTS 20


// posició del vèrtex
IN vec4 vPosition;

// normal del vèrtex
IN vec4 vNormal;

// matrius relacionades amb els càlculs de la càmera
uniform mat4 model_view;
uniform mat4 projection;

// posició (que arribarà al fragment shader interpolada pel píxel/fragment)
OUT vec4 position;

// normal (que arribarà al fragment shader interpolada pel píxel/fragment)
OUT vec4 normal;

// Funció principal que es crida en iniciar el programa
void main()
{

    // es calcula la posició final fent servir la posició del vèrtex i les matrius model-view i projection de la càmera
    gl_Position = projection * model_view * vPosition;

    position = vPosition;
    normal = vNormal;
}
