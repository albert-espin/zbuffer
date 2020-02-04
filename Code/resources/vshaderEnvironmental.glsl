#if __VERSION__<130
#define IN attribute
#define OUT varying
#else
#define IN in
#define OUT out
#endif


// posició del vèrtex
IN vec4 vPosition;

// matrius relacionades amb els càlculs de la càmera
uniform mat4 model_view;
uniform mat4 projection;

// posició, cal al fragment shader per calcular les coordenades de textura
OUT vec4 position;


// Funció principal que es crida en iniciar el programa
void main()
{
    // es calcula la posició final fent servir la posició del vèrtex i les matrius model-view i projection de la càmera
    gl_Position = projection * model_view * vPosition;

    position = vPosition;
}
